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

#ifndef SIZE_H
#define SIZE_H

#include "libsamplemacros.h"

class LIBSAMPLE_API Size
{
public:
    Size(double width = 0.0, double height = 0.0) : m_width(width), m_height(height) {}
    ~Size() {}

    inline double width() { return m_width; }
    inline void setWidth(double width) { m_width = width; }
    inline double height() { return m_height; }
    inline void setHeight(double height) { m_height = height; }

    inline double calculateArea() const { return m_width * m_height; }

    // Comparison Operators
    inline bool operator<(const Size& other)
    {
        return calculateArea() < other.calculateArea();
    }

    inline bool operator>(const Size& other)
    {
        // On some x86 hardware and compiler combinations, floating point
        // comparisons may fail due to a hardware bug. One workaround is to
        // simplify comparison expressions by putting partial results in
        // variables. See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=323#c109
        // for details.
        double a = calculateArea();
        double b = other.calculateArea();
        return a > b;
    }

    inline bool operator<=(const Size& other)
    {
        // See comments for operator>()
        double a = calculateArea();
        double b = other.calculateArea();
        return a <= b;
    }

    inline bool operator>=(const Size& other)
    {
        return calculateArea() >= other.calculateArea();
    }

    inline bool operator<(double area) { return calculateArea() < area; }
    inline bool operator>(double area) { return calculateArea() > area; }
    inline bool operator<=(double area) { return calculateArea() <= area; }
    inline bool operator>=(double area) { return calculateArea() >= area; }

    // Arithmetic Operators
    inline Size& operator+=(const Size& s)
    {
        m_width += s.m_width;
        m_height += s.m_height;
        return *this;
    }

    inline Size& operator-=(const Size& s)
    {
        m_width -= s.m_width;
        m_height -= s.m_height;
        return *this;
    }

    inline Size& operator*=(double mult)
    {
        m_width *= mult;
        m_height *= mult;
        return *this;
    }

    inline Size& operator/=(double div)
    {
        m_width /= div;
        m_height /= div;
        return *this;
    }

    // TODO: add ++size, size++, --size, size--

    // External operators
    friend inline bool operator==(const Size&, const Size&);
    friend inline bool operator!=(const Size&, const Size&);
    friend inline const Size operator+(const Size&, const Size&);
    friend inline const Size operator-(const Size&, const Size&);
    friend inline const Size operator*(const Size&, double);
    friend inline const Size operator*(double, const Size&);
    friend inline const Size operator/(const Size&, double);

    friend inline bool operator<(double, const Size&);
    friend inline bool operator>(double, const Size&);
    friend inline bool operator<=(double, const Size&);
    friend inline bool operator>=(double, const Size&);

    void show() const;

private:
    double m_width;
    double m_height;
};

// Comparison Operators
inline bool operator!=(const Size& s1, const Size& s2)
{
    return s1.m_width != s2.m_width || s1.m_height != s2.m_height;
}

inline bool operator==(const Size& s1, const Size& s2)
{
    return s1.m_width == s2.m_width && s1.m_height == s2.m_height;
}

inline bool operator<(double area, const Size& s)
{
    return area < s.calculateArea();
}

inline bool operator>(double area, const Size& s)
{
    return area > s.calculateArea();
}

inline bool operator<=(double area, const Size& s)
{
    return area <= s.calculateArea();
}

inline bool operator>=(double area, const Size& s)
{
    return area >= s.calculateArea();
}

// Arithmetic Operators
inline const Size operator+(const Size& s1, const Size& s2)
{
    return Size(s1.m_width + s2.m_width, s1.m_height + s2.m_height);
}

inline const Size operator-(const Size& s1, const Size& s2)
{
    return Size(s1.m_width - s2.m_width, s1.m_height - s2.m_height);
}

inline const Size operator*(const Size& s, double mult)
{
    return Size(s.m_width * mult, s.m_height * mult);
}

inline const Size operator*(double mult, const Size& s)
{
    return Size(s.m_width * mult, s.m_height * mult);
}

inline const Size operator/(const Size& s, double div)
{
    return Size(s.m_width / div, s.m_height / div);
}

using real = double;
using ushort = unsigned short;
class LIBSAMPLE_API SizeF
{
public:
    SizeF(real width, real height) : m_width(width), m_height(height) {}
    real width() { return m_width; }
    real height() { return m_height; }
    static inline ushort passTypedefOfUnsignedShort(ushort value) { return value; }
private:
    real m_width;
    real m_height;
};

#endif // SIZE_H

