#!/usr/bin/python

#############################################################################
##
## Copyright (C) 2018 The Qt Company Ltd.
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

'''Unit tests for QUuid'''

import os
import socket
import sys
import unittest

from pathlib import Path
sys.path.append(os.fspath(Path(__file__).resolve().parents[1]))
from init_paths import init_test_paths
init_test_paths(False)

from PySide6.QtCore import QCoreApplication, QSocketNotifier


class QSocketNotifierTest(unittest.TestCase):
    def testClass(self):
        app = QCoreApplication([])
        # socketpair is not available on Windows
        if os.name != "nt":
            w_sock, r_sock = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)

            self.assertIsInstance(r_sock.fileno(), int)

            notifier = QSocketNotifier(r_sock.fileno(), QSocketNotifier.Read)

            self.assertIsNotNone(notifier)

            w_sock.close()
            r_sock.close()


if __name__ == '__main__':
    unittest.main()
