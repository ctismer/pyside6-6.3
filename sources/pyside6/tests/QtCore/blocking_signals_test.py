#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
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

''' Test case for QObject.signalsBlocked() and blockSignal()'''

import gc
import os
import sys
from tempfile import mkstemp
import unittest

from pathlib import Path
sys.path.append(os.fspath(Path(__file__).resolve().parents[1]))
from init_paths import init_test_paths
init_test_paths(False)

from PySide6.QtCore import QObject, SIGNAL, QFile, QSignalBlocker


class TestSignalsBlockedBasic(unittest.TestCase):
    '''Basic test case for signalsBlocked'''

    def testBasic(self):
        '''QObject.signalsBlocked() and blockSignals()
        The signals aren't blocked by default.
        blockSignals returns the previous value'''
        obj = QObject()
        self.assertTrue(not obj.signalsBlocked())
        self.assertTrue(not obj.blockSignals(True))
        self.assertTrue(obj.signalsBlocked())
        self.assertTrue(obj.blockSignals(False))
        blocker = QSignalBlocker(obj)
        self.assertTrue(obj.signalsBlocked())
        blocker.unblock()
        self.assertTrue(not obj.signalsBlocked())
        blocker.reblock()
        self.assertTrue(obj.signalsBlocked())
        del blocker
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()
        self.assertTrue(not obj.signalsBlocked())

    def testContext(self):
        obj = QObject()
        self.assertTrue(not obj.signalsBlocked())
        with QSignalBlocker(obj):
            self.assertTrue(obj.signalsBlocked())
        self.assertTrue(not obj.signalsBlocked())

    def testContextWithAs(self):
        obj = QObject()
        self.assertTrue(not obj.signalsBlocked())
        with QSignalBlocker(obj) as blocker:
            self.assertTrue(obj.signalsBlocked())
            self.assertEqual(type(blocker), QSignalBlocker)
        self.assertTrue(not obj.signalsBlocked())


class TestSignalsBlocked(unittest.TestCase):
    '''Test case to check if the signals are really blocked'''

    def setUp(self):
        # Set up the basic resources needed
        self.obj = QObject()
        self.args = tuple()
        self.called = False

    def tearDown(self):
        # Delete used resources
        del self.obj
        del self.args
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()

    def callback(self, *args):
        # Default callback
        if args == self.args:
            self.called = True
        else:
            raise TypeError("Invalid arguments")

    def testShortCircuitSignals(self):
        # Blocking of Python short-circuit signals
        QObject.connect(self.obj, SIGNAL('mysignal()'), self.callback)

        self.obj.emit(SIGNAL('mysignal()'))
        self.assertTrue(self.called)

        self.called = False
        self.obj.blockSignals(True)
        self.obj.emit(SIGNAL('mysignal()'))
        self.assertTrue(not self.called)

    def testPythonSignals(self):
        # Blocking of Python typed signals
        QObject.connect(self.obj, SIGNAL('mysignal(int,int)'), self.callback)
        self.args = (1, 3)

        self.obj.emit(SIGNAL('mysignal(int,int)'), *self.args)
        self.assertTrue(self.called)

        self.called = False
        self.obj.blockSignals(True)
        self.obj.emit(SIGNAL('mysignal(int,int)'), *self.args)
        self.assertTrue(not self.called)


class TestQFileSignalBlocking(unittest.TestCase):
    '''Test case for blocking the signal QIODevice.aboutToClose()'''

    def setUp(self):
        # Set up the needed resources - A temp file and a QFile
        self.called = False
        handle, self.filename = mkstemp()
        os.close(handle)

        self.qfile = QFile(self.filename)

    def tearDown(self):
        # Release acquired resources
        os.remove(self.filename)
        del self.qfile
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()

    def callback(self):
        # Default callback
        self.called = True

    def testAboutToCloseBlocking(self):
        # QIODevice.aboutToClose() blocking

        QObject.connect(self.qfile, SIGNAL('aboutToClose()'), self.callback)

        self.assertTrue(self.qfile.open(QFile.ReadOnly))
        self.qfile.close()
        self.assertTrue(self.called)

        self.called = False
        self.qfile.blockSignals(True)

        self.assertTrue(self.qfile.open(QFile.ReadOnly))
        self.qfile.close()
        self.assertTrue(not self.called)


if __name__ == '__main__':
    unittest.main()
