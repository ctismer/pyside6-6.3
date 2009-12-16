#!/usr/bin/env python

"""Tests covering signal emission and receiving to python slots"""

import sys
import unittest

from PySide.QtCore import QObject, SIGNAL, SLOT, QProcess, QTimeLine
from PySide.QtCore import QTimer, QThread

try:
    from PySide.QtGui import QSpinBox, QPushButton
except ImportError:
    QSpinBox = object
    QPushButton = object
    QApplication = object

from helper import BasicPySlotCase, UsesQApplication, UsesQCoreApplication
from helper.decorators import requires

@requires('PySide.QtGui')
class ButtonPySlot(UsesQApplication, BasicPySlotCase):
    """Tests the connection of python slots to QPushButton signals"""

    def setUp(self):
        super(ButtonPySlot, self).setUp()

    def tearDown(self):
        super(ButtonPySlot, self).setUp()

    def testButtonClicked(self):
        """Connection of a python slot to QPushButton.clicked()"""
        button = QPushButton('Mylabel')
        QObject.connect(button, SIGNAL('clicked()'), self.cb)
        self.args = tuple()
        button.emit(SIGNAL('clicked()'))
        self.assert_(self.called)

    def testButtonClick(self):
        """Indirect qt signal emission using the QPushButton.click() method """
        button = QPushButton('label')
        QObject.connect(button, SIGNAL('clicked()'), self.cb)
        self.args = tuple()
        button.click()
        self.assert_(self.called)


@requires('PySide.QtGui')
class SpinBoxPySlot(UsesQApplication, BasicPySlotCase):
    """Tests the connection of python slots to QSpinBox signals"""

    def setUp(self):
        super(SpinBoxPySlot, self).setUp()
        self.spin = QSpinBox()

    def tearDown(self):
        del self.spin
        super(SpinBoxPySlot, self).tearDown()

    def testSpinBoxValueChanged(self):
        """Connection of a python slot to QSpinBox.valueChanged(int)"""
        QObject.connect(self.spin, SIGNAL('valueChanged(int)'), self.cb)
        self.args = [3]
        self.spin.emit(SIGNAL('valueChanged(int)'), *self.args)
        self.assert_(self.called)

    def testSpinBoxValueChangedImplicit(self):
        """Indirect qt signal emission using QSpinBox.setValue(int)"""
        QObject.connect(self.spin, SIGNAL('valueChanged(int)'), self.cb)
        self.args = [42]
        self.spin.setValue(self.args[0])
        self.assert_(self.called)

    def atestSpinBoxValueChangedFewArgs(self):
        """Emission of signals with fewer arguments than needed"""
        # XXX: PyQt4 crashes on the assertRaises
        QObject.connect(self.spin, SIGNAL('valueChanged(int)'), self.cb)
        self.args = (554,)
        self.assertRaises(TypeError, self.spin.emit, SIGNAL('valueChanged(int)'))

@requires('PySide.QtGui')
class QSpinBoxQtSlots(UsesQApplication):
    """Tests the connection to QSpinBox qt slots"""

    qapplication = True

    def testSetValueIndirect(self):
        """Indirect signal emission: QSpinBox using valueChanged(int)/setValue(int)"""
        spinSend = QSpinBox()
        spinRec = QSpinBox()

        spinRec.setValue(5)

        QObject.connect(spinSend, SIGNAL('valueChanged(int)'), spinRec, SLOT('setValue(int)'))
        self.assertEqual(spinRec.value(), 5)
        spinSend.setValue(3)
        self.assertEqual(spinRec.value(), 3)
        self.assertEqual(spinSend.value(), 3)

    def testSetValue(self):
        """Direct signal emission: QSpinBox using valueChanged(int)/setValue(int)"""
        spinSend = QSpinBox()
        spinRec = QSpinBox()

        spinRec.setValue(5)
        spinSend.setValue(42)

        QObject.connect(spinSend, SIGNAL('valueChanged(int)'), spinRec, SLOT('setValue(int)'))
        self.assertEqual(spinRec.value(), 5)
        self.assertEqual(spinSend.value(), 42)
        spinSend.emit(SIGNAL('valueChanged(int)'), 3)

        self.assertEqual(spinRec.value(), 3)
        #Direct emission shouldn't change the value of the emitter
        self.assertEqual(spinSend.value(), 42)

        spinSend.emit(SIGNAL('valueChanged(int)'), 66)
        self.assertEqual(spinRec.value(), 66)
        self.assertEqual(spinSend.value(), 42)


class ArgsOnEmptySignal(UsesQCoreApplication):
    '''Trying to emit a signal without arguments passing some arguments'''

    def testArgsToNoArgsSignal(self):
        '''Passing arguments to a signal without arguments'''
        process = QProcess()
        self.assertRaises(TypeError, process.emit, SIGNAL('started()'), 42)


class MoreArgsOnEmit(UsesQCoreApplication):
    '''Trying to pass more args than needed to emit (signals with args)'''

    def testMoreArgs(self):
        '''Passing more arguments than needed'''
        process = QProcess()
        self.assertRaises(TypeError, process.emit, SIGNAL('finished(int)'), 55, 55)

class Dummy(QObject):
    '''Dummy class'''
    pass


class PythonSignalToCppSlots(UsesQCoreApplication):
    '''Connect python signals to C++ slots'''

    def testWithoutArgs(self):
        '''Connect python signal to QTimeLine.toggleDirection()'''
        timeline = QTimeLine()
        dummy = Dummy()
        QObject.connect(dummy, SIGNAL('dummy()'),
                        timeline, SLOT('toggleDirection()'))

        orig_dir = timeline.direction()
        dummy.emit(SIGNAL('dummy()'))
        new_dir = timeline.direction()

        if orig_dir == QTimeLine.Forward:
            self.assertEqual(new_dir, QTimeLine.Backward)
        else:
            self.assertEqual(new_dir, QTimeLine.Forward)

    def testWithArgs(self):
        '''Connect python signals to QTimeLine.setCurrentTime(int)'''
        timeline = QTimeLine()
        dummy = Dummy()

        QObject.connect(dummy, SIGNAL('dummy(int)'),
                        timeline, SLOT('setCurrentTime(int)'))

        current = timeline.currentTime()
        dummy.emit(SIGNAL('dummy(int)'), current+42)
        self.assertEqual(timeline.currentTime(), current+42)

class CppSignalsToCppSlots(UsesQCoreApplication):
    '''Connection between C++ slots and signals'''

    def testWithoutArgs(self):
        '''Connect QThread.started() to QTimeLine.togglePaused()'''
        thread = QThread()
        timeline = QTimeLine()

        QObject.connect(thread, SIGNAL('started()'),
                        timeline, SLOT('toggleDirection()'))
        QObject.connect(thread, SIGNAL('started()'),
                        self.exit_app_cb)

        orig_dir = timeline.direction()

        timer = QTimer.singleShot(1000, self.exit_app_cb) # Just for safety

        thread.start()
        self.app.exec_()
        thread.wait()

        new_dir = timeline.direction()

        if orig_dir == QTimeLine.Forward:
            self.assertEqual(new_dir, QTimeLine.Backward)
        else:
            self.assertEqual(new_dir, QTimeLine.Forward)


if __name__ == '__main__':
    unittest.main()
