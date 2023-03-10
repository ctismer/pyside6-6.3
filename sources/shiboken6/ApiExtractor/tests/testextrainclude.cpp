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

#include "testextrainclude.h"
#include <QtTest/QTest>
#include "testutil.h"
#include <abstractmetalang.h>
#include <typesystem.h>

void TestExtraInclude::testClassExtraInclude()
{
    const char* cppCode ="struct A {};\n";
    const char* xmlCode = "\
    <typesystem package='Foo'>\n\
        <value-type name='A'>\n\
            <extra-includes>\n\
                <include file-name='header.h' location='global'/>\n\
            </extra-includes>\n\
        </value-type>\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode, false));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    const AbstractMetaClass *classA = AbstractMetaClass::findClass(classes, u"A");
    QVERIFY(classA);

    QList<Include> includes = classA->typeEntry()->extraIncludes();
    QCOMPARE(includes.size(), 1);
    QCOMPARE(includes.constFirst().name(), u"header.h");
}

void TestExtraInclude::testGlobalExtraIncludes()
{
    const char* cppCode ="struct A {};\n";
    const char* xmlCode = "\
    <typesystem package='Foo'>\n\
        <extra-includes>\n\
            <include file-name='header1.h' location='global'/>\n\
            <include file-name='header2.h' location='global'/>\n\
        </extra-includes>\n\
        <value-type name='A'/>\n\
    </typesystem>\n";

    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode, false));
    QVERIFY(!builder.isNull());
    AbstractMetaClassList classes = builder->classes();
    QVERIFY(AbstractMetaClass::findClass(classes, u"A"));

    TypeDatabase* td = TypeDatabase::instance();
    const TypeSystemTypeEntry *module = td->defaultTypeSystemType();
    QVERIFY(module);
    QCOMPARE(module->name(), u"Foo");

    QList<Include> includes = module->extraIncludes();
    QCOMPARE(includes.size(), 2);
    QCOMPARE(includes.constFirst().name(), u"header1.h");
    QCOMPARE(includes.constLast().name(), u"header2.h");
}

QTEST_APPLESS_MAIN(TestExtraInclude)
