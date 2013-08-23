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
#include <QMouseEvent>
#include <QRubberBand>

// Include QtVp header files.
#include "vpviewport.h"

VpViewport::VpViewport(QWidget *parent) :
    QWidget(parent), m_mode(MODE_CLOSED), m_physXMin(0), m_physXMax(0), m_physYMin(0), m_physYMax(0)
{
    // Provide a default name.
    setName(QString(tr("VpName")));
}

VpViewport::~VpViewport()
{
    // Do nothing.
}

bool VpViewport::intersectPhysical(VpViewport &vp, int xll, int yll, int xur, int yur)
{
    return (! (xll > vp.getPxmax() || xur < vp.getPxmin() ||
               yll > vp.getPymax() || yur < vp.getPymin()));
}

bool VpViewport::physCoordInViewport(VpViewport &vp, int x, int y)
{
    return ((vp.getPxmin() <= x) && (vp.getPxmax() >= x) &&
            (vp.getPymin() <= y) && (vp.getPymax() >= y));
}
