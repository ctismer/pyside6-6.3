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

#include <iostream>
#include "derived.h"

using namespace std;

Derived::Derived(int id) : Abstract(id)
{
}

Derived::~Derived()
{
}

Abstract*
Derived::createObject()
{
    static int id = 100;
    return new Derived(id++);
}

void
Derived::pureVirtual()
{
}

void*
Derived::pureVirtualReturningVoidPtr()
{
    return nullptr;
}

void
Derived::unpureVirtual()
{
}

bool
Derived::singleArgument(bool b)
{
    return !b;
}

double
Derived::defaultValue(int n)
{
    return ((double) n) + 0.1;
}

OverloadedFuncEnum Derived::overloaded(int, int)
{
    return OverloadedFunc_ii;
}

OverloadedFuncEnum Derived::overloaded(double)
{
    return OverloadedFunc_d;
}

Derived::OtherOverloadedFuncEnum Derived::otherOverloaded(int, int, bool, double)
{
    return OtherOverloadedFunc_iibd;
}

Derived::OtherOverloadedFuncEnum Derived::otherOverloaded(int, double)
{
    return OtherOverloadedFunc_id;
}

struct SecretClass : public Abstract {
    virtual void pureVirtual() {}
    virtual void *pureVirtualReturningVoidPtr() { return nullptr; }
    virtual PrintFormat returnAnEnum() { return Short; }
    void hideFunction(HideType*){};
private:
    virtual void pureVirtualPrivate() {}
};

Abstract* Derived::triggerImpossibleTypeDiscovery()
{
    return new SecretClass;
}

struct AnotherSecretClass : public Derived {
};

Abstract* Derived::triggerAnotherImpossibleTypeDiscovery()
{
    return new AnotherSecretClass;
}

void Derived::pureVirtualPrivate()
{
}
