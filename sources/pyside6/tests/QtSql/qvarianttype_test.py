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

'''Test cases for QVariant::Type converter'''
import os
import sys
import unittest

from pathlib import Path
sys.path.append(os.fspath(Path(__file__).resolve().parents[1]))
from init_paths import init_test_paths
init_test_paths(False)

from PySide6.QtCore import QMetaType
from PySide6.QtSql import QSqlField


class QVariantTypeTest(unittest.TestCase):
    def testQVariantType(self):
        new_enum = sys.pyside63_option_python_enum
        cmp_id = QMetaType.QString.value if new_enum else QMetaType.QString

        f = QSqlField("name", QMetaType(QMetaType.QString))
        self.assertEqual(f.metaType().id(), cmp_id)

        f = QSqlField("name", QMetaType.fromName(b"QString"))
        self.assertEqual(f.metaType().id(), cmp_id)

        f = QSqlField("name", QMetaType.fromName(b"double"))
        self.assertEqual(f.metaType(), float)

        f = QSqlField("name", float)
        self.assertEqual(f.metaType(), float)

        f = QSqlField("name", int)
        self.assertEqual(f.metaType(), int)


if __name__ == '__main__':
    unittest.main()
