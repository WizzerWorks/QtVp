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
#include <QSize>

// Include QtVp header files.
#include "vpruler.h"
#include "vputil.h"

VpRuler::VpRuler(QWidget* parent, RulerType rulerType)
    : QWidget(parent), m_rulerType(rulerType), m_origin(0.), m_rulerUnit(1.), m_rulerZoom(1.),
      m_mouseTracking(false), m_drawText(false), m_extentTracking(false)
{
    setMouseTracking(true);
    QFont txtFont("Goudy Old Style", 5,20);
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
    bool isHorzRuler = Horizontal == m_rulerType;
    QString str;
    if (isHorzRuler) str.append(tr("Horizontal")); else str.append(tr("Vertical"));
    qDebug() << str << "VpRuler Physical: (" << rect().left() << "," << rect().top() << ") - (" << rect().right() << "," << rect().bottom() << ")";
    qDebug() << str << "VpRuler World: (" << m_Wxmin << "," << m_Wymin << ") - (" << m_Wxmax << "," << m_Wymax << ")";
    qDebug() << str << "VpRuler Origin: (" << m_Wx << "," << m_Wy << ")";

    QPainter painter(this);
    if (m_extentTracking) {
        painter.setWindow(m_Wxmin, m_Wymin, m_Wxmax - m_Wxmin, m_Wymax - m_Wymin);
        //painter.setWorldMatrixEnabled(true);
        //painter.setViewport(rect().left(), rect().top(), rect().width(), rect().height());
    }
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);

    QPen pen(Qt::black, 0); // zero width pen is cosmetic pen
    //pen.setCosmetic(true);
    painter.setPen(pen);
    // We want to work with floating point, so we are considering
    // the rect as QRectF
    //QRectF rulerRect = this->rect();
    QRectF rulerRect;
    //rulerRect.setX(m_Wxmin);
    //rulerRect.setY(m_Wymin);
    //rulerRect.setWidth(m_Wxmax - m_Wxmin);
    //rulerRect.setHeight(m_Wymax - m_Wymin);
    rulerRect.setCoords(m_Wxmin, m_Wymin, m_Wxmax, m_Wymax);
    // at first fill the rect
    //painter.fillRect(rulerRect,QColor(220,200,180));
    painter.fillRect(rulerRect,QColor(236, 233, 216));

    // drawing a scale of 25
    drawAScaleMeter(&painter, rulerRect, 25, (Horizontal == m_rulerType ? rulerRect.height() : rulerRect.width())/2);
    // drawing a scale of 50
    drawAScaleMeter(&painter, rulerRect, 50, (Horizontal == m_rulerType ? rulerRect.height() : rulerRect.width())/4);
    // drawing a scale of 100
    m_drawText = true;
    drawAScaleMeter(&painter, rulerRect, 100, 0);
    m_drawText = false;

    // Drawing the current mouse position indicator.
    painter.setOpacity(0.4);
    drawMousePosTick(&painter);
    painter.setOpacity(1.0);

    // Drawing no man's land between the ruler and view.
    QPointF starPt = Horizontal == m_rulerType ? rulerRect.bottomLeft() : rulerRect.topRight();
    QPointF endPt = Horizontal == m_rulerType ? rulerRect.bottomRight() : rulerRect.bottomRight();
    painter.setPen(QPen(Qt::black,2));
    painter.drawLine(starPt,endPt);
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
        qreal x = m_cursorPos.x();
        qreal y = m_cursorPos.y();
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
        m_Wxmin = size.left();
        m_Wymin = 0;
        m_Wxmax = size.right();
        m_Wymax = this->rect().bottom();
    } else
    {
        // Setting vertical world extent.
        m_Wxmin = 0;
        m_Wymin = size.top();
        m_Wxmax = this->rect().right();
        m_Wymax = size.bottom();
    }

    m_xscale = ((qreal) m_Wxmax - m_Wxmin) / ((qreal) this->rect().width());
    m_yscale = ((qreal) m_Wymax - m_Wymin) / ((qreal) this->rect().height());
    m_xoffset = (qreal) this->rect().left() - (((qreal) size.left()) * m_xscale);
    m_yoffset = (qreal) this->rect().top() - (((qreal) size.top()) * m_yscale);
    //m_xoffset = m_yoffset = 0;
}

void VpRuler::setExtentTrack(const bool track)
{
    if (m_extentTracking != track)
    {
        m_extentTracking = track;
        update();
    }
}

#if 0
void VpRuler::worldToDev(int *x, int *y)
{
    // Declare local variables.
    float fx, fy;

    // Note that the result is int but the calculation is float.
    fx = ((*x) * m_xscale) + m_xoffset;
    fy = ((*y) * m_yscale) + m_yoffset;

    *x = VpUtil::round(fx);
    *y = VpUtil::round(fy);
}

void VpRuler::devToWorld(int *x, int *y)
{
    // Declare local variables.
    float fx, fy;

    fx = ((*x) - m_xoffset) / m_xscale;
    fy = ((*y) - m_yoffset) / m_yscale;

    *x = VpUtil::round(fx);
    *y = VpUtil::round(fy);
}
#else
void VpRuler::worldToDev(qreal *x, qreal *y)
{
    // Declare local variables.
    qreal fx, fy;

    // Note that the result is int but the calculation is float.
    fx = ((*x) * m_xscale) + m_xoffset;
    fy = ((*y) * m_yscale) + m_yoffset;

    *x = fx; *y = fy;
}

void VpRuler::devToWorld(qreal *x, qreal *y)
{
    // Declare local variables.
    qreal fx, fy;

    fx = ((*x) - m_xoffset) / m_xscale;
    fy = ((*y) - m_yoffset) / m_yscale;

    *x = fx; *y = fy;
}
#endif
