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

#ifndef __GRIDGC_H_
#define __GRIDGC_H_

// Include QtVp header files.
#include "qtvp_global.h"
#include "vpgc.h"

class QTVPSHARED_EXPORT GridGC
{
  public:
    GridGC();
    virtual ~GridGC();

    VpGC *m_gc;
    int   m_xll;
    int   m_yll;
    int   m_xur;
    int   m_yur;
    int   m_truexll;
    int   m_trueyll;
    int   m_truexur;
    int   m_trueyur;
    int   m_xnum;
    int   m_ynum;
    int   m_dx;
    int   m_dy;
};

#endif // __GRIDGC_H_
