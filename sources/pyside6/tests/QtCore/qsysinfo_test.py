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

from PySide6.QtCore import QSysInfo


class TestQSysInfo(unittest.TestCase):
    newenum = sys.pyside63_option_python_enum

    def testEnumEndian(self):
        self.assertEqual(QSysInfo.BigEndian.value if self.newenum else QSysInfo.BigEndian, 0)
        self.assertEqual(QSysInfo.LittleEndian.value if self.newenum else QSysInfo.LittleEndian, 1)
        self.assertTrue((QSysInfo.ByteOrder.value if self.newenum else QSysInfo.ByteOrder) > -1)

    def testEnumSizes(self):
        self.assertTrue((QSysInfo.WordSize.value if self.newenum else QSysInfo.WordSize) > 0)


if __name__ == '__main__':
    unittest.main()
