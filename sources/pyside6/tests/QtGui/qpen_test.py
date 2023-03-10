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

import os
import sys
import unittest

from pathlib import Path
sys.path.append(os.fspath(Path(__file__).resolve().parents[1]))
from init_paths import init_test_paths
init_test_paths(False)

from helper.usesqguiapplication import UsesQGuiApplication

from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QPen, QPainter, QRasterWindow


class Painting(QRasterWindow):
    def __init__(self):
        super().__init__()
        self.penFromEnum = None
        self.penFromInteger = None

    def paintEvent(self, event):
        with QPainter(self) as painter:
            painter.setPen(Qt.NoPen)
            self.penFromEnum = painter.pen()
            intVal = Qt.NoPen.value if sys.pyside63_option_python_enum else int(Qt.NoPen)
            painter.setPen(intVal)
            self.penFromInteger = painter.pen()
        QTimer.singleShot(20, self.close)


class QPenTest(UsesQGuiApplication):

    def testCtorWithCreatedEnums(self):
        '''A simple case of QPen creation using created enums.'''
        width = 0
        style = Qt.PenStyle(0)
        cap = Qt.PenCapStyle(0)
        join = Qt.PenJoinStyle(0)
        pen = QPen(Qt.blue, width, style, cap, join)

    def testSetPenWithPenStyleEnum(self):
        '''Calls QPainter.setPen with both enum and integer. Bug #511.'''
        w = Painting()
        w.show()
        w.setTitle("qpen_test")
        self.app.exec()
        self.assertEqual(w.penFromEnum.style(), Qt.NoPen)
        self.assertEqual(w.penFromInteger.style(), Qt.SolidLine)


if __name__ == '__main__':
    unittest.main()

