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

#ifndef __VPRULER_H_
#define __VPRULER_H_

// Include Qt header files.
#include <QtWidgets>

#define RULER_BREADTH 20

class VpRuler : public QWidget
{
    Q_OBJECT

    Q_ENUMS(RulerType)
    Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
    Q_PROPERTY(qreal rulerUnit READ rulerUnit WRITE setRulerUnit)
    Q_PROPERTY(qreal rulerZoom READ rulerZoom WRITE setRulerZoom)

  public:

    enum RulerType { Horizontal, Vertical };

    VpRuler(QWidget* parent, RulerType rulerType);
    virtual ~VpRuler();

    QSize minimumSizeHint() const;
    RulerType rulerType() const;
    qreal origin() const;
    qreal rulerUnit() const;
    qreal rulerZoom() const;

  public slots:

    void setOrigin(const qreal origin);
    void setRulerUnit(const qreal rulerUnit);
    void setRulerZoom(const qreal rulerZoom);
    void setCursorPos(const QPoint cursorPos);
    void setMouseTrack(const bool track);

  protected:

    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);

  private:

    void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPosition);
    void drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition);
    void drawMousePosTick(QPainter* painter);

    RulerType m_rulerType;
    qreal     m_origin;
    qreal     m_rulerUnit;
    qreal     m_rulerZoom;
    QPoint    m_cursorPos;
    bool      m_mouseTracking;
    bool      m_drawText;
};

#endif // __VPRULER_H_
