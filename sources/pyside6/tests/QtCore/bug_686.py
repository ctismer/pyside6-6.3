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

from PySide6.QtCore import (QMutex, QMutexLocker, QReadLocker, QReadWriteLock,
                            QThread, QWriteLocker)


class MyWriteThread(QThread):
    def __init__(self, lock):
        super().__init__()
        self.lock = lock
        self.started = False
        self.canQuit = False

    def run(self):
        self.started = True
        while not self.lock.tryLockForWrite():
            pass
        self.canQuit = True
        self.lock.unlock()


class MyReadThread(QThread):
    def __init__(self, lock):
        super().__init__()
        self.lock = lock
        self.started = False
        self.canQuit = False

    def run(self):
        self.started = True
        while not self.lock.tryLockForRead():
            pass
        self.canQuit = True
        self.lock.unlock()


class MyMutexedThread(QThread):
    def __init__(self, mutex):
        super().__init__()
        self.mutex = mutex
        self.started = False
        self.canQuit = False

    def run(self):
        self.started = True
        while not self.mutex.tryLock():
            pass
        self.mutex.unlock()
        self.canQuit = True


class TestQMutex (unittest.TestCase):

    def testReadLocker(self):
        lock = QReadWriteLock()
        thread = MyWriteThread(lock)

        with QReadLocker(lock):
            thread.start()
            while not thread.started:
                QThread.msleep(10)
            self.assertFalse(thread.canQuit)

        self.assertTrue(thread.wait(2000))
        self.assertTrue(thread.canQuit)

    def testWriteLocker(self):
        lock = QReadWriteLock()
        thread = MyReadThread(lock)

        with QWriteLocker(lock):
            thread.start()
            while not thread.started:
                QThread.msleep(10)
            self.assertFalse(thread.canQuit)

        self.assertTrue(thread.wait(2000))
        self.assertTrue(thread.canQuit)

    def testMutexLocker(self):
        mutex = QMutex()
        thread = MyMutexedThread(mutex)

        with QMutexLocker(mutex):
            thread.start()
            while not thread.started:
                QThread.msleep(10)
            self.assertFalse(thread.canQuit)

        self.assertTrue(thread.wait(2000))
        self.assertTrue(thread.canQuit)

    def testWithAsLocker(self):
        lock = QReadWriteLock()
        with QReadLocker(lock) as locker:
            self.assertTrue(isinstance(locker, QReadLocker))
        with QWriteLocker(lock) as locker:
            self.assertTrue(isinstance(locker, QWriteLocker))
        mutex = QMutex()
        with QMutexLocker(mutex) as locker:
            self.assertTrue(isinstance(locker, QMutexLocker))
        with self.assertRaises(TypeError):
            with QMutexLocker(lock) as locker:
                pass


if __name__ == '__main__':
    unittest.main()
