// COPYRIGHT_BEGIN
// The MIT License (MIT)
//
// Copyright (c) 2013 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// COPYRIGHT_END

// Include Qt header files.
#include <QString>

// Include QtVp header files.
#include "vputil.h"
#include "vpcoord.h"

int VpCoord::g_resolution = 1;


VpCoord::VpCoord(int x, int y, int z)
    : m_x(x), m_y(y), m_z(z)
{
    // Do nothing extra.
}

VpCoord::VpCoord(const VpCoord &coord)
{
    m_x = coord.m_x;
    m_y = coord.m_y;
    m_z = coord.m_z;
    m_name = coord.m_name;
}

VpCoord::~VpCoord()
{
    // Do nothing.
}

double VpCoord::internalToWorld(const int x)
{
    return (((double) x)/g_resolution);
}

int VpCoord::worldToInternal(const double x)
{
    return VpUtil::round((x) * g_resolution);
}

const QString VpCoord::toString()
{
    char buffer[64];

    if (m_name.isNull() || m_name.isEmpty()) {
        // Add the viewport name as part of the coordinate string.
        sprintf_s(buffer, "(%d,%d,%d,%s)", m_x, m_y, m_z, m_name.toUtf8());
    } else {
        // Else close off the coordinate string.
        sprintf_s(buffer, "(%d,%d,%d)", m_x, m_y, m_z);
    }

    QString *coord = new QString(buffer);
    return *coord;
}

VpCoord & VpCoord::operator=(const VpCoord & coord)
{
    if (this != &coord)
    {
        m_x = coord.m_x;
        m_y = coord.m_y;
        m_z = coord.m_z;
        m_name = coord.m_name;
    }

    // By convention, always return *this.
    return *this;
}

bool VpCoord::operator==(const VpCoord & coord) const
{
    bool retValue = true;

    if (this != &coord)
    {
        if ((m_x == coord.m_x) && (m_y == coord.m_y) &&
            (m_z == coord.m_z) && (m_name == coord.m_name))
            retValue = true;
        else
            retValue = false;
    }

    return retValue;
}

bool VpCoord::operator!=(const VpCoord & coord) const
{
    bool retValue = false;

    if (this != &coord)
    {
        if ((m_x != coord.m_x) || (m_y != coord.m_y) ||
            (m_z != coord.m_z) || (m_name != coord.m_name))
            retValue = true;
        else
            retValue = false;
    }

    return retValue;
}
