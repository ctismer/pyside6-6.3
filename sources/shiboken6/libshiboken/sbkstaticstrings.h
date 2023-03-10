/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt for Python.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SBKSTATICSTRINGS_H
#define SBKSTATICSTRINGS_H

#include "sbkpython.h"
#include "shibokenmacros.h"

namespace Shiboken
{
// Some often-used strings
namespace PyName
{
LIBSHIBOKEN_API PyObject *co_name();
LIBSHIBOKEN_API PyObject *dumps();
LIBSHIBOKEN_API PyObject *fget();
LIBSHIBOKEN_API PyObject *fset();
LIBSHIBOKEN_API PyObject *f_code();
LIBSHIBOKEN_API PyObject *f_lineno();
LIBSHIBOKEN_API PyObject *im_func();
LIBSHIBOKEN_API PyObject *im_self();
LIBSHIBOKEN_API PyObject *loads();
LIBSHIBOKEN_API PyObject *multi();
LIBSHIBOKEN_API PyObject *name();
LIBSHIBOKEN_API PyObject *result();
LIBSHIBOKEN_API PyObject *select_id();
LIBSHIBOKEN_API PyObject *value();
LIBSHIBOKEN_API PyObject *values();
LIBSHIBOKEN_API PyObject *qtStaticMetaObject();
} // namespace PyName

namespace PyMagicName
{
LIBSHIBOKEN_API PyObject *class_();
LIBSHIBOKEN_API PyObject *dict();
LIBSHIBOKEN_API PyObject *doc();
LIBSHIBOKEN_API PyObject *ecf();
LIBSHIBOKEN_API PyObject *file();
LIBSHIBOKEN_API PyObject *func();
LIBSHIBOKEN_API PyObject *get();
LIBSHIBOKEN_API PyObject *members();
LIBSHIBOKEN_API PyObject *module();
LIBSHIBOKEN_API PyObject *name();
LIBSHIBOKEN_API PyObject *property_methods();
LIBSHIBOKEN_API PyObject *qualname();
LIBSHIBOKEN_API PyObject *self();
LIBSHIBOKEN_API PyObject *opaque_container();
LIBSHIBOKEN_API PyObject *code();
LIBSHIBOKEN_API PyObject *rlshift();
LIBSHIBOKEN_API PyObject *rrshift();
} // namespace PyMagicName
} // namespace Shiboken

#endif // SBKSTATICSTRINGS_H
