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
#include <QGridLayout>

// Include QtVp header files.
#include "vpgraphicsview.h"

VpGraphicsView::VpGraphicsView(QWidget *parent)
  :  QScrollArea(parent)
{
}

VpGraphicsView::~VpGraphicsView()
{
    if (m_horizontalRuler != NULL) delete m_horizontalRuler;
    if (m_verticalRuler != NULL) delete m_verticalRuler;
}

void VpGraphicsView::init(QWidget *widget)
{
    setViewportMargins(RULER_BREADTH, RULER_BREADTH, 0, 0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    m_horizontalRuler = new VpRuler(this, VpRuler::Horizontal);
    m_verticalRuler = new VpRuler(this, VpRuler::Vertical);

    m_horizontalRuler->setMouseTrack(true);
    m_verticalRuler->setMouseTrack(true);

    QWidget* placeholder = new QWidget();
    placeholder->setBackgroundRole(QPalette::Window);
    placeholder->setFixedSize(RULER_BREADTH, RULER_BREADTH);
    gridLayout->addWidget(placeholder, 0, 0);
    gridLayout->addWidget(m_horizontalRuler, 0, 1);
    gridLayout->addWidget(m_verticalRuler, 1, 0);
    gridLayout->addWidget(widget, 1, 1);

    setLayout(gridLayout);
}
