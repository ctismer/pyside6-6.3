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

import gc
import os
import sys
import unittest
import weakref

from pathlib import Path
sys.path.append(os.fspath(Path(__file__).resolve().parents[1]))
from init_paths import init_test_paths
init_test_paths(False)

from PySide6.QtWidgets import QMenu, QWidget, QMenuBar, QToolBar
from helper.usesqapplication import UsesQApplication


class TestQActionLifeCycle(UsesQApplication):
    def actionDestroyed(self, act):
        self._actionDestroyed = True

    def testMenu(self):
        self._actionDestroyed = False
        w = QWidget()
        menu = QMenu(w)
        act = menu.addAction("MENU")
        _ref = weakref.ref(act, self.actionDestroyed)
        act = None
        self.assertFalse(self._actionDestroyed)
        menu.clear()
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()
        self.assertTrue(self._actionDestroyed)

    def testMenuBar(self):
        self._actionDestroyed = False
        w = QWidget()
        menuBar = QMenuBar(w)
        act = menuBar.addAction("MENU")
        _ref = weakref.ref(act, self.actionDestroyed)
        act = None
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()
        self.assertFalse(self._actionDestroyed)
        menuBar.clear()
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()
        self.assertTrue(self._actionDestroyed)

    def testToolBar(self):
        self._actionDestroyed = False
        w = QWidget()
        toolBar = QToolBar(w)
        act = toolBar.addAction("MENU")
        _ref = weakref.ref(act, self.actionDestroyed)
        act = None
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()
        self.assertFalse(self._actionDestroyed)
        toolBar.clear()
        # PYSIDE-535: Need to collect garbage in PyPy to trigger deletion
        gc.collect()
        self.assertTrue(self._actionDestroyed)


if __name__ == "__main__":
    unittest.main()
