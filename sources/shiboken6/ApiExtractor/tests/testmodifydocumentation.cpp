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

#include "testmodifydocumentation.h"
#include "testutil.h"
#include <abstractmetalang.h>
#include <documentation.h>
#include <modifications.h>
#include <typesystem.h>
#include <qtdocparser.h>

#include <qtcompat.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QTemporaryDir>
#include <QtTest/QTest>

using namespace Qt::StringLiterals;

void TestModifyDocumentation::testModifyDocumentation()
{
    const char* cppCode ="struct B { void b(); }; class A {};\n";
    const char xmlCode[] =
R"(<typesystem package="Foo">
    <value-type name='B'>
        <modify-function signature='b()' remove='all'/>
    </value-type>
    <value-type name='A'>
    <modify-documentation xpath='description/brief'>&lt;brief>Modified Brief&lt;/brief></modify-documentation>
    <modify-documentation xpath='description/para[3]'>&lt;para>Some changed contents here&lt;/para></modify-documentation>
    </value-type>
</typesystem>
)";
    QScopedPointer<AbstractMetaBuilder> builder(TestUtil::parse(cppCode, xmlCode));
    QVERIFY(!builder.isNull());
    AbstractMetaClass *classA = AbstractMetaClass::findClass(builder->classes(), u"A");
    QVERIFY(classA);
    DocModificationList docMods = classA->typeEntry()->docModifications();
    QCOMPARE(docMods.size(), 2);
    QCOMPARE(docMods[0].code().trimmed(), u"<brief>Modified Brief</brief>");
    QCOMPARE(docMods[0].signature(), QString());
    QCOMPARE(docMods[1].code().trimmed(), u"<para>Some changed contents here</para>");
    QCOMPARE(docMods[1].signature(), QString());

    // Create a temporary directory for the documentation file since libxml2
    // cannot handle Qt resources.
    QTemporaryDir tempDir(QDir::tempPath() + u"/shiboken_testmodifydocXXXXXX"_s);
    QVERIFY2(tempDir.isValid(), qPrintable(tempDir.errorString()));
    const QString docFileName = u"a.xml"_s;
    QVERIFY(QFile::copy(u":/"_s + docFileName, tempDir.filePath(docFileName)));

    QtDocParser docParser;
    docParser.setDocumentationDataDirectory(tempDir.path());
    docParser.fillDocumentation(classA);

    const Documentation &doc = classA->documentation();
    const QString actualDocSimplified = doc.detailed().simplified();
    const QString actualBriefSimplified = doc.brief().simplified();
    QVERIFY(!actualDocSimplified.isEmpty());

const char expectedDoc[] =
R"(<?xml version="1.0"?>
<description>oi
<para>Paragraph number 1</para>
<para>Paragraph number 2</para>
<para>Some changed contents here</para>
</description>
)";
    const QString expectedDocSimplified = QString::fromLatin1(expectedDoc).simplified();
    // Check whether the first modification worked.
    QVERIFY(actualBriefSimplified.contains(u"Modified Brief"));

#ifndef HAVE_LIBXSLT
    // QtXmlPatterns is unable to handle para[3] in style sheets,
    // this only works in its XPath search.
    QEXPECT_FAIL("", "QtXmlPatterns cannot handle para[3] (QTBUG-66925)", Abort);
#endif
    QCOMPARE(actualDocSimplified, expectedDocSimplified);
}

// We expand QTEST_MAIN macro but using QCoreApplication instead of QApplication
// because this test needs an event loop but can't use QApplication to avoid a crash
// on our ARMEL/FRAMANTLE buildbot
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    TestModifyDocumentation tc;
    return QTest::qExec(&tc, argc, argv);
}
