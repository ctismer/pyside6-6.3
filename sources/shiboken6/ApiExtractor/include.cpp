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

#include "include.h"
#include "textstream.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QHash>
#include <QtCore/QTextStream>

#include "qtcompat.h"

#include <algorithm>

using namespace Qt::StringLiterals;

QString Include::toString() const
{
    if (m_type == IncludePath)
        return u"#include <"_s + m_name + u'>';
    if (m_type == LocalPath)
        return u"#include \""_s + m_name + u'"';
    return u"import "_s + m_name + u';';
}

size_t qHash(const Include& inc)
{
    return qHash(inc.m_name);
}

QTextStream& operator<<(QTextStream& out, const Include& g)
{
    if (g.isValid())
        out << g.toString() << Qt::endl;
    return out;
}

TextStream& operator<<(TextStream& out, const Include& include)
{
    if (include.isValid())
        out << include.toString() << '\n';
    return out;
}

TextStream& operator<<(TextStream &out, const IncludeGroup& g)
{
    if (!g.includes.isEmpty()) {
        if (!g.title.isEmpty())
            out << "\n// " << g.title << "\n";
        auto includes = g.includes;
        std::sort(includes.begin(), includes.end());
        for (const Include &inc : qAsConst(includes))
            out << inc.toString() << '\n';
    }
    return out;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, const Include &i)
{
    QDebugStateSaver saver(d);
    d.noquote();
    d.nospace();
    d << "Include(";
    if (i.isValid())
        d << "type=" << i.type() << ", file=\"" << QDir::toNativeSeparators(i.name()) << '"';
    else
        d << "invalid";
    d << ')';
    return d;
}
#endif // !QT_NO_DEBUG_STREAM
