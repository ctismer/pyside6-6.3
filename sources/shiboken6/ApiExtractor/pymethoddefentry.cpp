/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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

#include "pymethoddefentry.h"
#include "textstream.h"

#include <QtCore/QDebug>

TextStream &operator<<(TextStream &s, const PyMethodDefEntry &e)
{
     s <<  "{\"" << e.name << "\", reinterpret_cast<PyCFunction>("
      << e.function << "), ";
    if (e.methFlags.isEmpty()) {
         s << '0';
    } else {
        for (qsizetype i = 0, size = e.methFlags.size(); i < size; ++i) {
            if (i)
                s << '|';
            s << e.methFlags.at(i);
        }
    }
    s << '}';
    return s;
}

TextStream &operator<<(TextStream &s, const PyMethodDefEntries &entries)
{
    for (const auto &e : entries)
        s << e << ",\n";
    return s;
}

QDebug operator<<(QDebug debug, const PyMethodDefEntry &e)
{
    QDebugStateSaver saver(debug);
    debug.noquote();
    debug.nospace();
    debug << "PyMethodDefEntry(\"" << e.name << "\", " << e.function
          << ", " << e.methFlags << ')';
    return debug;
}
