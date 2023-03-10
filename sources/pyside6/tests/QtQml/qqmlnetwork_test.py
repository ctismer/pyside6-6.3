#############################################################################
##
## Copyright (C) 2016 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the test suite of Qt for Python.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################

'''Test cases for QQmlNetwork'''

import os
import sys
import unittest

from pathlib import Path
sys.path.append(os.fspath(Path(__file__).resolve().parents[1]))
from init_paths import init_test_paths
init_test_paths(False)

from PySide6.QtCore import QUrl, QTimer
from PySide6.QtGui import QGuiApplication, QWindow
from PySide6.QtQuick import QQuickView
from PySide6.QtQml import QQmlNetworkAccessManagerFactory
from PySide6.QtNetwork import QNetworkAccessManager

from helper.helper import quickview_errorstring
from helper.timedqguiapplication import TimedQGuiApplication


request_created = False


def check_done():
    global request_created
    if request_created:
        windows = QGuiApplication.topLevelWindows()
        if windows:
            windows[0].close()


class CustomManager(QNetworkAccessManager):
    """CustomManager (running in a different thread)"""
    def createRequest(self, op, req, data=None):
        global request_created
        print(">> createRequest ", self, op, req.url(), data)
        request_created = True
        return QNetworkAccessManager.createRequest(self, op, req, data)


class CustomFactory(QQmlNetworkAccessManagerFactory):
    def create(self, parent=None):
        return CustomManager()


class TestQQmlNetworkFactory(TimedQGuiApplication):
    def setUp(self):
        super().setUp(timeout=2000)

    def testQQuickNetworkFactory(self):
        view = QQuickView()
        self.factory = CustomFactory()
        view.engine().setNetworkAccessManagerFactory(self.factory)

        file = Path(__file__).resolve().parent / 'hw.qml'
        self.assertTrue(file.is_file())
        url = QUrl.fromLocalFile(file)

        view.setSource(url)
        self.assertTrue(view.rootObject(), quickview_errorstring(view))
        view.show()

        self.assertEqual(view.status(), QQuickView.Ready)

        timer = QTimer()
        timer.timeout.connect(check_done)
        timer.start(50)
        self.app.exec()


if __name__ == '__main__':
    unittest.main()
