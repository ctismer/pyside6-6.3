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

from PySide6.QtCore import Qt
from PySide6.QtGui import QKeySequence

from helper.usesqguiapplication import UsesQGuiApplication


class QKeySequenceTest(UsesQGuiApplication):

    def testGetItemOperator(self):
        # bug #774
        if sys.pyside63_option_python_enum:
            # PYSIDE-1735: Remapped from Qt.Modifier to Qt.KeyboardModifier
            #              Note that Qt.(Keyboard)?Modifier will be no longer IntFlag.
            ks = QKeySequence(Qt.ShiftModifier, Qt.ControlModifier, Qt.Key_P, Qt.Key_R)
            self.assertEqual(ks[0].keyboardModifiers(), Qt.ShiftModifier)
            self.assertEqual(ks[1].keyboardModifiers(), Qt.ControlModifier)
            self.assertEqual(ks[2].key(), Qt.Key_P)
            self.assertEqual(ks[3].key(), Qt.Key_R)
        else:
            ks = QKeySequence(Qt.SHIFT, Qt.CTRL, Qt.Key_P, Qt.Key_R)
            self.assertEqual(ks[0], Qt.SHIFT)
            self.assertEqual(ks[1], Qt.CTRL)
            self.assertEqual(ks[2], Qt.Key_P)
            self.assertEqual(ks[3], Qt.Key_R)


if __name__ == '__main__':
    unittest.main()
