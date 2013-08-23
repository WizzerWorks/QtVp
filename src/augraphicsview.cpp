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
#include "augraphicsview.h"

AuGraphicsView::AuGraphicsView()
{
}

AuGraphicsView::~AuGraphicsView()
{
    if (m_horizontalRuler) delete m_horizontalRuler;
    if (m_verticalRuler) delete m_verticalRuler;
    if (m_coordPosLabel) delete m_coordPosLabel;
}


void AuGraphicsView::init()
{
    setViewportMargins(RULER_BREADTH, RULER_BREADTH, 0, 0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    m_horizontalRuler = new AuRuler(this, AuRuler::Horizontal);
    m_verticalRuler = new AuRuler(this, AuRuler::Vertical);

    m_horizontalRuler->setMouseTrack(true);
    m_verticalRuler->setMouseTrack(true);

    m_coordPosLabel = new QLabel();

    QWidget* placeholder = new QWidget();
    placeholder->setBackgroundRole(QPalette::Window);
    placeholder->setFixedSize(RULER_BREADTH, RULER_BREADTH);
    gridLayout->addWidget(placeholder, 0, 0);
    gridLayout->addWidget(m_horizontalRuler, 0, 1);
    gridLayout->addWidget(m_verticalRuler, 1, 0);
    gridLayout->addWidget(this->viewport(), 1, 1);

    this->setLayout(gridLayout);
}

void AuGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    handleMouseMoved(*event);
}

void AuGraphicsView::handleMouseMoved(const QMouseEvent &event)
{
    char buffer[64];

    int x = event.x();
    int y = event.y();

    if (event.buttons() == Qt::LeftButton)
    {
        sprintf_s(buffer, "Left Button (%d, %d)", x, y);
    } else if (event.buttons() == Qt::RightButton)
    {
        sprintf_s(buffer, "Right Button (%d, %d)", x, y);
    } else
    {
        sprintf_s(buffer, "(%d,%d)", x, y);
    }

    m_coordPosLabel->setText(buffer);
}
