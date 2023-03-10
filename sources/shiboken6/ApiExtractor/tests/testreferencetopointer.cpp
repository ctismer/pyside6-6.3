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

#include "testreferencetopointer.h"
#include <QtTest/QTest>
#include "testutil.h"
#include <abstractmetafunction.h>
#include <abstractmetalang.h>
#include <typesystem.h>

void TestReferenceToPointer::testReferenceToPointerArgument()
{
    const char* cppCode ="\
    struct A {};\n\
    struct B {\n\
        void dummy(A*&);\n\
    };\n";
    const char* xmlCode = "\
    <typesystem package=\"Foo\">\n\
        <object-type name='A'/>\n\
        <object-type name='B'/>\n\
    </typesystem>\n";
    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode, false));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classB = AbstractMetaClass::findClass(classes, u"B");
    QVERIFY(classB);
    const auto func = classB->findFunction(u"dummy");
    QVERIFY(!func.isNull());
    QCOMPARE(func->arguments().constFirst().type().minimalSignature(), u"A*&");
}

QTEST_APPLESS_MAIN(TestReferenceToPointer)


