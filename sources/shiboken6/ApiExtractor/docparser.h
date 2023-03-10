/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt for Python.
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
#ifndef DOCPARSER_H
#define DOCPARSER_H

#include "typesystem_typedefs.h"
#include "abstractmetalang_typedefs.h"

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

class AbstractMetaClass;
class DocModification;
class Documentation;

class XQuery;

struct FunctionDocumentation;

class DocParser
{
public:
    Q_DISABLE_COPY(DocParser)

    using XQueryPtr = QSharedPointer<XQuery>;

    DocParser();
    virtual ~DocParser();
    virtual void fillDocumentation(AbstractMetaClass* metaClass) = 0;

    /**
     *   Process and retrieves documentation concerning the entire
     *   module or library.
     *   \return object containing module/library documentation information
     */
    virtual Documentation retrieveModuleDocumentation() = 0;

    void setDocumentationDataDirectory(const QString& dir)
    {
        m_docDataDir = dir;
    }

    /**
     *   Informs the location of the XML data generated by the tool
     *   (e.g.: DoxyGen, qdoc) used to extract the library's documentation
     *   comment.
     *   \return the path for the directory containing the XML data created
     *   from the library's documentation beign parsed.
     */
    QString documentationDataDirectory() const
    {
        return m_docDataDir;
    }

    void setLibrarySourceDirectory(const QString& dir)
    {
        m_libSourceDir = dir;
    }
    /**
     *   Informs the location of the library being parsed. The library
     *   source code is parsed for the documentation comments.
     *   \return the path for the directory containing the source code of
     *   the library beign parsed.
     */
    QString librarySourceDirectory() const
    {
        return m_libSourceDir;
    }

    void setPackageName(const QString& packageName)
    {
        m_packageName = packageName;
    }
    /**
     *   Retrieves the name of the package (or module or library) being parsed.
     *   \return the name of the package (module/library) being parsed
     */
    QString packageName() const
    {
        return m_packageName;
    }

    /**
    *   Process and retrieves documentation concerning the entire
    *   module or library.
    *   \param name module name
    *   \return object containing module/library documentation information
    *   \todo Merge with retrieveModuleDocumentation() on next ABI change.
    */
    virtual Documentation retrieveModuleDocumentation(const QString& name) = 0;

    static bool skipForQuery(const AbstractMetaFunctionCPtr &func);

protected:
    static QString getDocumentation(const XQueryPtr &xquery,
                                    const QString &query,
                                    const DocModificationList &mods);

    static AbstractMetaFunctionCList documentableFunctions(const AbstractMetaClass *metaClass);

    static QString applyDocModifications(const DocModificationList &mods, const QString &xml);

private:
    QString m_packageName;
    QString m_docDataDir;
    QString m_libSourceDir;

    static QString execXQuery(const XQueryPtr &xquery, const QString &query) ;
};

#endif // DOCPARSER_H
