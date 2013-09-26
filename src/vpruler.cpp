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
#include <QPainter>
#include <QSize>
#include <QMouseEvent>
#include <QDebug>

// Include QtVp header files.
#include "vpruler.h"
#include "vputil.h"

VpRuler::VpRuler(QWidget* parent, RulerType rulerType)
    : VpGraphics2D(parent), m_rulerType(rulerType), m_origin(0.), m_rulerUnit(1.), m_rulerZoom(1.),
      m_mouseTracking(false), m_drawText(false), m_extentTracking(false)
{
    setMouseTracking(true);
    QFont txtFont("Goudy Old Style", 5, 20);
    txtFont.setStyleHint(QFont::TypeWriter,QFont::PreferOutline);
    setFont(txtFont);
}

VpRuler::~VpRuler()
{
}

QSize VpRuler::minimumSizeHint() const
{
    return QSize(RULER_BREADTH, RULER_BREADTH);
}

VpRuler::RulerType VpRuler::rulerType() const
{
    return m_rulerType;
}

qreal VpRuler::origin() const
{
    return m_origin;
}

qreal VpRuler::rulerUnit() const
{
    return m_rulerUnit;
}

qreal VpRuler::rulerZoom() const
{
    return m_rulerZoom;
}

void VpRuler::setOrigin(const qreal origin)
{
    if (m_origin != origin)
    {
        m_origin = origin;
        update();
    }
}

void VpRuler::setRulerUnit(const qreal rulerUnit)
{
    if (m_rulerUnit != rulerUnit)
    {
        m_rulerUnit = rulerUnit;
        update();
    }
}

void VpRuler::setRulerZoom(const qreal rulerZoom)
{
    if (m_rulerZoom != rulerZoom)
    {
        m_rulerZoom = rulerZoom;
        update();
    }
}

void VpRuler::setCursorPos(const QPoint cursorPos)
{
    m_cursorPos = this->mapFromGlobal(cursorPos);
    m_cursorPos += QPoint(RULER_BREADTH,RULER_BREADTH);
    update();
}

void VpRuler::setMouseTrack(const bool track)
{
    if (m_mouseTracking != track)
    {
        m_mouseTracking = track;
        update();
    }
}

void VpRuler::mouseMoveEvent(QMouseEvent* event)
{
    m_cursorPos = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void VpRuler::paintEvent(QPaintEvent* event)
{
    //bool isHorzRuler = Horizontal == m_rulerType;
    //QString str;
    //if (isHorzRuler) str.append(tr("Horizontal")); else str.append(tr("Vertical"));
    //qDebug() << str << "VpRuler Physical: (" << rect().left() << "," << rect().top() << ") - (" << rect().right() << "," << rect().bottom() << ")";
    //qDebug() << str << "VpRuler World: (" << m_2dWxmin << "," << m_2dWymin << ") - (" << m_2dWxmax << "," << m_2dWymax << ")";
    //qDebug() << str << "VpRuler Origin: (" << m_Wx << "," << m_Wy << ")";

    // Create the Qt graphics context.
    QPainter *gc = m_painter;
    gc->begin(this);
    gc->setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);

    // Set world coordinate extent.
    QRect extent;
    extent.setLeft(getWxmin());
    extent.setRight(getWxmax());
    if (Horizontal == m_rulerType) {
        extent.setTop(getWymin());
        extent.setBottom(getWymax());
    } else {
        extent.setTop(getWymax());
        extent.setBottom(getWymin());
    }
    gc->setWindow(extent);

    QPen pen(Qt::black, 0); // zero width pen is cosmetic pen
    //pen.setCosmetic(true);
    gc->setPen(pen);
    // We want to work with floating point, so we are considering
    // the rect as QRectF
    QRectF rulerRect;
    rulerRect.setCoords(m_2dWxmin, m_2dWymin, m_2dWxmax, m_2dWymax);
    // First fill the rect.
    //painter.fillRect(rulerRect,QColor(220,200,180));
    gc->fillRect(rulerRect,QColor(236, 233, 216));

    // Drawing a scale of 25.
    drawAScaleMeter(gc, rulerRect, 25, (Horizontal == m_rulerType ? rulerRect.height() : rulerRect.width())/2);
    // Drawing a scale of 50.
    drawAScaleMeter(gc, rulerRect, 50, (Horizontal == m_rulerType ? rulerRect.height() : rulerRect.width())/4);
    // Drawing a scale of 100.
    m_drawText = true;
    drawAScaleMeter(gc, rulerRect, 100, 0);
    m_drawText = false;

    // Drawing the current mouse position indicator.
    gc->setOpacity(0.4);
    drawMousePosTick(gc);
    gc->setOpacity(1.0);

    // Drawing no man's land between the ruler and view.
    QPointF starPt = Horizontal == m_rulerType ? rulerRect.bottomLeft() : rulerRect.topRight();
    QPointF endPt = Horizontal == m_rulerType ? rulerRect.bottomRight() : rulerRect.bottomRight();
    gc->setPen(QPen(Qt::black,2));
    gc->drawLine(starPt,endPt);

    // Complete painting.
    gc->end();
}

void VpRuler::drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPosition)
{
    // Flagging whether we are horizontal or vertical only to reduce
    // to cheching many times.
    bool isHorzRuler = Horizontal == m_rulerType;
    scaleMeter  = scaleMeter * m_rulerUnit * m_rulerZoom;
    // Ruler rectangle starting mark.
    qreal rulerStartMark = isHorzRuler ? rulerRect.left() : rulerRect.top();
    // Ruler rectangle ending mark.
    qreal rulerEndMark = isHorzRuler ? rulerRect.right() : rulerRect.bottom();
    // Condition A # If origin point is between the start & end mark,
    // we have to draw both from origin to left mark & origin to right mark.
    // Condition B # If origin point is left of the start mark, we have to draw
    // from origin to end mark.
    // Condition C # If origin point is right of the end mark, we have to draw
    // from origin to start mark.
    if (m_origin >= rulerStartMark && m_origin <= rulerEndMark)
    {
        drawFromOriginTo(painter, rulerRect, m_origin, rulerEndMark, 0, scaleMeter, startPosition);
        drawFromOriginTo(painter, rulerRect, m_origin, rulerStartMark, 0, -scaleMeter, startPosition);
    } else if (m_origin < rulerStartMark)
    {
        int tickNo = int((rulerStartMark - m_origin) / scaleMeter);
        drawFromOriginTo(painter, rulerRect, m_origin + scaleMeter * tickNo,
                         rulerEndMark, tickNo, scaleMeter, startPosition);
    } else if (m_origin > rulerEndMark)
    {
        int tickNo = int((m_origin - rulerEndMark) / scaleMeter);
        drawFromOriginTo(painter, rulerRect, m_origin - scaleMeter * tickNo,
                         rulerStartMark, tickNo, -scaleMeter, startPosition);
    }
}

void VpRuler::drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition)
{
    bool isHorzRuler = Horizontal == m_rulerType;
    int iterate = 0;
    for (qreal current = startMark; (step < 0 ? current >= endMark : current <= endMark); current += step)
    {
        qreal x1 = isHorzRuler ? current : rulerRect.left() + startPosition;
        qreal y1 = isHorzRuler ? rulerRect.top() + startPosition : current;
        qreal x2 = isHorzRuler ? current : rulerRect.right();
        qreal y2 = isHorzRuler ? rulerRect.bottom() : current;
        painter->drawLine(QLineF(x1,y1,x2,y2));
        if (m_drawText)
        {
            QPainterPath txtPath;
            txtPath.addText(x1 + 1,y1 + (isHorzRuler ? 7 : -2),this->font(),QString::number(qAbs(int(step) * startTickNo++)));
            painter->drawPath(txtPath);
            iterate++;
        }
    }
}

void VpRuler::drawMousePosTick(QPainter* painter)
{
    if (m_mouseTracking)
    {
        int x = m_cursorPos.x();
        int y = m_cursorPos.y();
        devToWorld(&x, &y);

        //QPoint starPt = m_cursorPos;
        QPointF starPt;
        starPt.setX(x);
        starPt.setY(y);

        QPointF endPt;
        if (Horizontal == m_rulerType)
        {
            starPt.setY(this->rect().top());
            endPt.setX(starPt.x());
            endPt.setY(this->rect().bottom());
        }
        else
        {
            starPt.setX(this->rect().left());
            endPt.setX(this->rect().right());
            endPt.setY(starPt.y());
        }
        painter->drawLine(starPt, endPt);
    }
}

void VpRuler::setExtent(const QRect size, const QPoint origin)
{
    // If the extent is invalid, simply return.
    if ((! size.isValid()) || size.isNull())
    {
        qDebug("Ruler extent: invalid rectangle.");
        return;
    }

    m_Wx = origin.x();
    m_Wy = origin.y();

    bool isHorzRuler = Horizontal == m_rulerType;
    if (isHorzRuler)
    {
        // Setting horizontal world extent.
        int xmin = size.left();
        int ymin = 0;
        int xmax = size.right();
        int ymax = this->rect().bottom();
        setWorldCoords(xmin, ymin, xmax, ymax);
    } else
    {
        // Setting vertical world extent.
        int xmin = 0;
        int ymin = size.top();
        int xmax = this->rect().right();
        int ymax = size.bottom();
        setWorldCoords(xmin, ymin, xmax, ymax);
    }
}

void VpRuler::setExtentTrack(const bool track)
{
    if (m_extentTracking != track)
    {
        m_extentTracking = track;
        update();
    }
}
