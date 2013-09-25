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

#ifndef __VPGRAPHICSVIEW_H_
#define __VPGRAPHICSVIEW_H_

// Include Qt header files.
#include <QScrollArea>

// Include QtVp header files.
#include "qtvp_global.h"
#include "vpruler.h"

class QTVPSHARED_EXPORT VpGraphicsView : public QScrollArea
{
    Q_OBJECT

  public:

    explicit VpGraphicsView(QWidget *parent = 0);
    virtual ~VpGraphicsView();

    void init(QWidget *widget);

  signals:
    
  public slots:

    void on_newExtent(QRect size, QPoint origin);
    void on_trackExtent(bool track);
    
  private:

    /** Horizontal ruler. */
    VpRuler *m_horizontalRuler;
    /** Vertical ruler. */
    VpRuler *m_verticalRuler;

};

#endif // __VPGRAPHICSVIEW_H_
