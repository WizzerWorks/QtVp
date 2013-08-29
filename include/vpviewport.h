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

#ifndef __VPVIEWPORT_H_
#define __VPVIEWPORT_H_

// Include Qt header files.
#include <QWidget>

class QString;

/**
 * The VpViewport class is a base class used for managing the coordinate
 * space of a graphics viewport.
 *
 * @author Mark S. Millard
 */
class VpViewport : public QWidget
{
    Q_OBJECT

  public:

    // IO control states.
    enum Mode { MODE_CLOSED, MODE_OUTINDEV, MODE_OUTDEV, MODE_INDEV };

    explicit VpViewport(QWidget *parent = 0);
    virtual ~VpViewport();

    // Accessor utilities for member variables.
    QString getName() { return m_name; }
    void setName(QString name) { m_name = name; }
    int getPxmin() { return m_physXMin; }
    void setPxmin(int value) { m_physXMin = value; }
    int getPymin() { return m_physYMin; }
    void setPymin(int value) { m_physYMin = value; }
    int getPxmax() { return m_physXMax; }
    void setPxmax(int value) { m_physXMax = value; }
    int getPymax() { return m_physYMax; }
    void setPymax(int value) { m_physYMax = value; }
    Mode getMode() { return m_mode; }
    void setMode(Mode mode) { m_mode = mode; }

    /**
     * Used to determine if a line or rectangle intersects a given
     * viewport - physical coords.
     */
    static bool intersectPhysical(VpViewport &vp, int xll, int yll, int xur, int yur);

    /**
     * Returns <b>true</b> if (x,y) is located in specified viewport.
     */
    static bool physCoordInViewport(VpViewport &vp, int x, int y);

  signals:

  public slots:

  protected:

    Mode m_mode;    // The IO mode the Viewport is configured for.

    QString m_name;       // Name of viewport.
    int     m_physXMin;   // Physical device coordinate, x min.
    int     m_physYMin;   // Physical device coordinate, y min.
    int     m_physXMax;   // Physical device coordinate, x max.
    int     m_physYMax;   // Physical device coordinate, y max.

};

#endif // __VPVIEWPORT_H_
