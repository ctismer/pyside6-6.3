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

#include "testrefcounttag.h"
#include "testutil.h"
#include <abstractmetafunction.h>
#include <abstractmetalang.h>
#include <modifications.h>

#include <qtcompat.h>

#include <QtTest/QTest>

using namespace Qt::StringLiterals;

void TestRefCountTag::testReferenceCountTag()
{
    const char* cppCode ="\
    struct A {};\n\
    struct B {\n\
        void keepObject(B* b);\n\
    };\n";
    const char* xmlCode = "\
    <typesystem package=\"Foo\">\n\
        <object-type name='A'/>\n\
        <object-type name='B'>\n\
        <modify-function signature='keepObject(B*)'>\n\
            <modify-argument index='1'>\n\
                <reference-count action='add'/>\n\
            </modify-argument>\n\
        </modify-function>\n\
        </object-type>\n\
    </typesystem>\n";
    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode, false));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classB = AbstractMetaClass::findClass(classes, u"B");
    const auto func = classB->findFunction(u"keepObject");
    QVERIFY(!func.isNull());
    const auto refCount =
        func->modifications().constFirst().argument_mods().constFirst().referenceCounts().constFirst();
    QCOMPARE(refCount.action, ReferenceCount::Add);
}

void TestRefCountTag::testWithApiVersion()
{
    const char* cppCode ="\
    struct A {};\n\
    struct B {\n\
        void keepObject(B*, B*);\n\
    };\n";
    const char* xmlCode = "\
    <typesystem package=\"Foo\">\n\
        <object-type name='A'/>\n\
        <object-type name='B'>\n\
        <modify-function signature='keepObject(B*, B*)'>\n\
            <modify-argument index='1' since='0.1'>\n\
                <reference-count action='add'/>\n\
            </modify-argument>\n\
            <modify-argument index='2' since='0.2'>\n\
                <reference-count action='add'/>\n\
            </modify-argument>\n\
        </modify-function>\n\
        </object-type>\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode,
                                                                false, u"0.1"_s));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classB = AbstractMetaClass::findClass(classes, u"B");
    const auto func = classB->findFunction(u"keepObject");
    QVERIFY(!func.isNull());
    const auto refCount =
        func->modifications().constFirst().argument_mods().constFirst().referenceCounts().constFirst();
    QCOMPARE(refCount.action, ReferenceCount::Add);

    QCOMPARE(func->modifications().size(), 1);
}


QTEST_APPLESS_MAIN(TestRefCountTag)


