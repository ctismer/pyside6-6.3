/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of Qt for Python.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MODIFICATIONS_H
#define MODIFICATIONS_H

#include "libsamplemacros.h"
#include <utility>
#include "point.h"
#include "oddbool.h"

class ObjectType;

class LIBSAMPLE_API Modifications
{
public:
    Modifications();
    virtual ~Modifications();

    enum OverloadedModFunc {
        OverloadedNone,
        Overloaded_ibid,
        Overloaded_ibib,
        Overloaded_ibiP,
        Overloaded_ibii,
        Overloaded_ibPP
    };

    enum TestEnum {
        TestEnumValue1,
        TestEnumValue2
    };

    // those overloaded methods should be heavily modified
    // to push the overload decisor to its limits
    OverloadedModFunc overloaded(int a0, bool b0, int c0, double d0);
    OverloadedModFunc overloaded(int a1, bool b1, int c1, bool d1);
    OverloadedModFunc overloaded(int a2, bool b2, int c2, Point d2);
    OverloadedModFunc overloaded(int a3, bool b3, int c3 = 123, int d3 = 456);
    OverloadedModFunc overloaded(int a4, bool b4, Point c4, Point d4);

    void argRemoval0(int a0, bool a1, int a2 = 123, int a3 = 456);
    void argRemoval0(int a0, bool a1, int a2, bool a3);

    void argRemoval1(int a0, bool a1, Point a2 = Point(1, 2), Point a3 = Point(3, 4),
                     int a4 = 333);
    void argRemoval1(int a0, bool a1, int a2, bool a3);

    void argRemoval2(int a0, bool a1, Point a2 = Point(1, 2), Point a3 = Point(3, 4),
                     int a4 = 333);

    void argRemoval3(int a0, Point a1 = Point(1, 2), bool a2 = true, Point a3 = Point(3, 4),
                     int a4 = 333);

    void argRemoval4(int a0, Point a1, bool a2, Point a3 = Point(3, 4), int a4 = 333);

    void argRemoval5(int a0, bool a1, Point a2 = Point(1, 2), Point a3 = Point(3, 4),
                     int a4 = 333);
    void argRemoval5(int a0, bool a1, int a2, bool a3);

    // 'ok' must be removed and the return value will be changed
    // to a tuple (PyObject*) containing the expected result plus
    // the 'ok' value as a Python boolean
    std::pair<double, double> pointToPair(Point pt, bool* ok);

    // same as 'pointToPair' except that this time 'ok' is the first argument
    double multiplyPointCoordsPlusValue(bool* ok, Point pt, double value);

    // completely remove 'plus' from the Python side
    int doublePlus(int value, int plus = 0);

    // the default value for both arguments must be changed in Python
    int power(int base = 1, int exponent = 0);

    // in Python set argument default value to 10
    int timesTen(int number);

    // in Python remove the argument default value
    int increment(int number = 0);

    // don't export this method to Python
    void exclusiveCppStuff();

    // change the name of this regular method
    int cppMultiply(int a, int b);

    // change the name of this virtual method
    virtual const char* className();

    Point sumPointArray(int arraySize, const Point pointArray[]);

    // Replace 'const void*' by 'ByteArray&'.
    int getSize(const void* data, int size);

    // Mark the argument with a <no-null-pointer/> tag;
    // the test implementation must expect point never to be null.
    int sumPointCoordinates(const Point* point);

    // Modify the return value of a virtual method.
    virtual double differenceOfPointCoordinates(const Point* pt, bool* ok);
    double callDifferenceOfPointCoordinates(const Point* pt, bool* ok) { return differenceOfPointCoordinates(pt, ok); }

    // Sets an ObjectType in the argument and returns true.
    bool nonConversionRuleForArgumentWithDefaultValue(ObjectType **object = nullptr);
    ObjectType* getObject() const { return m_object; }

    // Inject code with a %CONVERTTOPYTHON that receives an user's primitive type.
    static inline OddBool passOddBool(OddBool ob) { return ob; }

    void setEnumValue(TestEnum e = TestEnumValue1);
    TestEnum enumValue() const;
    TestEnum defaultEnumValue() const;

    bool wasGetAttroCalled() const;
    void notifyGetAttroCalled();

    bool wasSetAttroCalled() const;
    void notifySetAttroCalled();

private:
    ObjectType* m_object;
    TestEnum m_enumValue = TestEnumValue1;
    bool m_getAttroCalled = false;
    bool m_setAttroCalled = false;
};

class LIBSAMPLE_API AbstractModifications : public Modifications
{
public:
    AbstractModifications() {}
    virtual ~AbstractModifications() {}

    inline bool invert(bool value) { return !value; }

    // completely remove this method in Python
    virtual void pointlessPureVirtualMethod() = 0;
};

#endif // MODIFICATIONS_H
