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

#include "testconversionoperator.h"
#include "testutil.h"
#include <abstractmetafunction.h>
#include <abstractmetalang.h>
#include <typesystem.h>

#include <qtcompat.h>

#include <QtTest/QTest>

using namespace Qt::StringLiterals;

void TestConversionOperator::testConversionOperator()
{
    const char cppCode[] = "\
    struct A {\n\
    };\n\
    struct B {\n\
        operator A() const;\n\
    };\n\
    struct C {\n\
        operator A() const;\n\
    };\n";
    const char xmlCode[] = "\
    <typesystem package=\"Foo\">\n\
        <value-type name='A'/>\n\
        <value-type name='B'/>\n\
        <value-type name='C'/>\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classA = AbstractMetaClass::findClass(classes, u"A");
    const AbstractMetaClass *classB = AbstractMetaClass::findClass(classes, u"B");
    const AbstractMetaClass *classC = AbstractMetaClass::findClass(classes, u"C");
    QVERIFY(classA);
    QVERIFY(classB);
    QVERIFY(classC);
    QCOMPARE(classA->functions().size(), 2);
    QCOMPARE(classB->functions().size(), 3);
    QCOMPARE(classC->functions().size(), 3);
    QCOMPARE(classA->externalConversionOperators().size(), 2);

    AbstractMetaFunctionCPtr convOp;
    for (const auto &func : classB->functions()) {
        if (func->isConversionOperator()) {
            convOp = func;
            break;
        }
    }
    QVERIFY(!convOp.isNull());
    QVERIFY(classA->externalConversionOperators().contains(convOp));
}

void TestConversionOperator::testConversionOperatorOfDiscardedClass()
{
    const char cppCode[] = "\
    struct A {\n\
    };\n\
    struct B {\n\
        operator A() const;\n\
    };\n";
    const char xmlCode[] = "\
    <typesystem package=\"Foo\">\n\
        <value-type name='A' />\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classA = AbstractMetaClass::findClass(classes, u"A");
    QVERIFY(classA);
    QCOMPARE(classA->externalConversionOperators().size(), 0);
}

void TestConversionOperator::testRemovedConversionOperator()
{
    const char cppCode[] = "\
    struct A {\n\
    };\n\
    struct B {\n\
        operator A() const;\n\
    };\n";
    const char xmlCode[] = "\
    <typesystem package=\"Foo\">\n\
        <value-type name='A' />\n\
        <value-type name='B'>\n\
            <modify-function signature='operator A() const' remove='all'/>\n\
        </value-type>\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classA = AbstractMetaClass::findClass(classes, u"A");
    const AbstractMetaClass *classB = AbstractMetaClass::findClass(classes, u"B");
    QVERIFY(classA);
    QVERIFY(classB);
    QCOMPARE(classA->functions().size(), 2);
    QCOMPARE(classB->functions().size(), 3);
    QCOMPARE(classA->externalConversionOperators().size(), 0);
    QCOMPARE(classA->implicitConversions().size(), 0);
}

void TestConversionOperator::testConversionOperatorReturningReference()
{
    const char cppCode[] = "\
    struct A {};\n\
    struct B {\n\
        operator A&() const;\n\
    };\n";
    const char xmlCode[] = "\
    <typesystem package='Foo'>\n\
        <value-type name='A'/>\n\
        <value-type name='B'/>\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classA = AbstractMetaClass::findClass(classes, u"A");
    const AbstractMetaClass *classB = AbstractMetaClass::findClass(classes, u"B");
    QVERIFY(classA);
    QVERIFY(classB);
    QCOMPARE(classA->functions().size(), 2);
    QCOMPARE(classB->functions().size(), 3);
    QCOMPARE(classA->externalConversionOperators().size(), 1);
    QCOMPARE(classA->externalConversionOperators().constFirst()->type().cppSignature(),
             u"A");
    QCOMPARE(classA->externalConversionOperators().constFirst()->ownerClass()->name(),
             u"B");
    QCOMPARE(classA->implicitConversions().size(), 1);
    QCOMPARE(classA->implicitConversions().constFirst()->type().cppSignature(),
             u"A");
    QCOMPARE(classA->implicitConversions().constFirst()->ownerClass()->name(),
             u"B");
}

void TestConversionOperator::testConversionOperatorReturningConstReference()
{
    const char cppCode[] = "\
    struct A {};\n\
    struct B {\n\
        operator const A&() const;\n\
    };\n";
    const char xmlCode[] = "\
    <typesystem package='Foo'>\n\
        <value-type name='A'/>\n\
        <value-type name='B'/>\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classA = AbstractMetaClass::findClass(classes, u"A");
    const AbstractMetaClass *classB = AbstractMetaClass::findClass(classes, u"B");
    QVERIFY(classA);
    QVERIFY(classB);
    QCOMPARE(classA->functions().size(), 2);
    QCOMPARE(classB->functions().size(), 3);
    QCOMPARE(classA->externalConversionOperators().size(), 1);
    QCOMPARE(classA->externalConversionOperators().constFirst()->type().cppSignature(),
             u"A"_s);
    QCOMPARE(classA->externalConversionOperators().constFirst()->ownerClass()->name(),
             u"B"_s);
    QCOMPARE(classA->implicitConversions().size(), 1);
    QCOMPARE(classA->implicitConversions().constFirst()->type().cppSignature(),
             u"A"_s);
    QCOMPARE(classA->implicitConversions().constFirst()->ownerClass()->name(),
             u"B"_s);
}

QTEST_APPLESS_MAIN(TestConversionOperator)
