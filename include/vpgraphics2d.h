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

#ifndef __AUGRAPHICS2D_H_
#define __AUGRAPHICS2D_H_

// Include Qt header files.
#include <QObject>

// Include Viewport header files.
#include "vptypes.h"
#include "vpcoord.h"
#include "vpgrid.h"
#include "vpviewport.h"
#include "vpgc.h"
#include "vpruler.h"

// Forward declarations.
class QRect;
class QPoint;
class QRubberBand;

/**
 * The <code>VpGraphics2D</code> class is a base class used for managing the coordinate
 * space of a 2-dimensional graphics viewport.
 *
 * @author Mark S. Millard
 */
class VpGraphics2D : public VpViewport
{
    Q_OBJECT

  public:

    explicit VpGraphics2D(QWidget *parent = 0);

    /**
     * @brief The destructor.
     */
    virtual ~VpGraphics2D();

    // Accessor utilities for member variables.

    int getWxmin() { return m_2dWxmin; }
    void setWxmin(int value) { m_2dWxmin= value; }
    int getWymin() { return m_2dWymin; }
    void setWymin(int value) { m_2dWymin= value; }
    int getWxmax() { return m_2dWxmax; }
    void setWxmax(int value) { m_2dWxmax = value; }
    int getWymax() { return m_2dWymax; }
    void setWymax(int value) { m_2dWymax= value; }
    float getXScale() { return m_2dXScale; }
    void setXScale(float value) { m_2dXScale= value; }
    float getYScale() { return m_2dYScale; }
    void setYScale(float value) { m_2dYScale= value; }
    float getXOffset() { return m_2dXOffset; }
    void setXOffset(float value) { m_2dXOffset= value; }
    float getYOffset() { return m_2dYOffset; }
    void setYOffset(float value) { m_2dYOffset= value; }
    float getPixelWidth() { return m_2dPixelWidth; }
    void setPixelWidth(float value) { m_2dPixelWidth= value; }
    float getPixelHeight() { return m_2dPixelHeight; }
    void setPixelHeight(float value) { m_2dPixelHeight= value; }
    VpGrid *getGrid() { return m_2dGrid; }
    void setGrid(VpGrid *grid) { m_2dGrid = grid; }

    /**
     * Set the world coordinate space of a bounding region.
     *
     * @param xmin The minimum x component of the bounding region.
     * @param ymin The minimum y component of the bounding region.
     * @param xmax The maximum x component of the bounding region.
     * @param ymax The maximum y component of the bounding region.
     *
     * @return <b>true</b> is returned if the world coordinate
     * extent is successfully set to the bounding region. Otherwise,
     * <b>false</b> is returned.
     */
    bool setWorldCoords(int xmin,int ymin,int xmax,int ymax);

    /**
     * Convert the specified world coordinate to device coordinate.
     *
     * @param x The x component of the world coordinate.
     * @param y The y component of the world coordinate.
     */
    void worldToDev(int *x, int *y);

    /**
     * Convert the specified device coordinate to world coordinate.
     *
     * @param x The x component of the device coordinate.
     * @param y The y component of the device coordinate.
     */
    void devToWorld(int *x, int *y);

    /**
     * Convert a rectangle from world coordinate space to device coordinate
     * space. Retain the order of min and max.
     *
     * @param xmin The minimal x coordinate of the rectangle.
     * @param ymin The minimal y coordinate of the rectangle.
     * @param xmax The maximum x coordinate of the rectangle.
     * @param ymax The maximum y coordinate of the rectangle.
     *
     * @return A <code>QRect</code> is returned.
     */
    QRect *worldToDevRect(int xmin,int ymin,int xmax,int ymax);

    /**
     * Scale world coordinates to device coordinates.
     *
     * @param x The x component of the world coordinate.
     * @param y The y component of the world coordinate.
     */
    void scaleWorldToDev(int *x, int *y);

    /**
     * Scale device coordinates to world coordinates.
     *
     * @param x The x component of the device coordinate.
     * @param y The y component of the device coordinate.
     */
    void scaleDevToWorld(int *x, int *y);

    /**
     * Used to determine if a line or rectangle intersects a given
     * viewport - world coordinates.
     *
     * @param vp A 2D Graphics Viewport.
     * @param xll The lower-left x-coordinate value.
     * @param yll The lower-left y-coordinate value.
     * @param xur The upper-right x-coordinate value.
     * @param yur The upper-right y-coordinate value.
     *
     * @return If the specified coordinates intersect with the Viewport
     * in World Coordinate Space, then <b>true</b> will be returned.
     * Otherwise, <b>false</b> will be returned.
     */
    static bool intersectWorld(VpGraphics2D &vp, int xll, int yll, int xur, int yur);

    /**
     * Draw the grid using the specified graphics context.
     *
     * @param gc The Viewport graphics context.
     *
     * @return If the grid is successfully drawn, then <b>true</b> will
     * be returned. Otherwise, <b>false</b> will be returned.
     */
    bool drawGrid(VpGC *gc);

    /**
     * Draw the grid reference.
     *
     * @param gc The Viewport graphics context.
     *
     * @return If the reference is successfully drawn, then <b>true</b> will
     * be returned. Otherwise, <b>false</b> will be returned.
     */
    bool drawGridReference(VpGC *gc);

    /**
     * Display the grid based on the context of its state.
     *
     * @param gc The Viewport graphics context.
     *
     * @return If the grid is successfully displayed, then <b>true</b>
     * will be returned. Otherwise,<b>false</b> will be returne.
     */

    bool displayGrid(VpGC *gc);
    /**
     * Erase the grid based on the context of its state.
     *
     * @param gc The Viewport graphics context.
     *
     * @return If the grid is successfully erased, then <b>true</b>
     * will be returned. Otherwise,<b>false</b> will be returne.
     */
    bool eraseGrid(VpGC *gc);

    /**
     * Update the state of the grid and draw the result.
     *
     * @param gc The Viewport graphics context.
     * @param dispState The GridState to update the grid from.
     */
    void updateGrid(VpGC *gc, const GridState &dispState);

    /**
     * Update the state of the grid reference marker and draw
     * the result.
     *
     * @param gc The Viewport graphics context.
     * @param dispState The GridState to update the grid reference
     * marker from.
     */
    void updateGridReference(VpGC *gc, const GridState &dispState);

    /**
     * @brief Clear the widget using the current background.
     */
    void clear();

    /**
     * Snap the specified coordinate to the nearest grid coordinate.
     *
     * @param x The x component of the coordinate to snap.
     * @param y The y component of the coordinate to snap.
     */
    void snapToGrid(int *x, int *y);

    /**
     * Retrieve the state of the grid as a string.
     */
    QString toString();

  signals:

    /**
     * Emit a mouse moved signal.
     *
     * @param event The event to signal.
     */
    void mouseMoved(const QMouseEvent &event);

    /**
     * Emit a mouse pressed signal.
     *
     * @param event The event to signal.
     */
    void mousePressed(const QMouseEvent &event);

    /**
     * Emit a mouse released signal.
     *
     * @param event The event to signal.
     */
    void mouseReleased(const QMouseEvent &event);

    /**
     * @brief Signal that world coordinate has changed.
     *
     * @param coord The world coordinate in the form "(x,y,vpname)".
     * <b>x</b> is the x position of the coordinate. <b>y</b> is the
     * y position of the coordinate. <b>vpname</b> is the name of
     * the viewport.
     */
    void coordChanged(const QString &coord);

    void updateStatus(const QString &coord);

  protected slots:

    /**
     * Process the coordinate.
     */
    void processCoord(const QMouseEvent &event);

  protected:

    static bool adjustExtentToViewport(VpGraphics2D &vp,
        double *Wc_xmin, double *Wc_ymin, double *Wc_xmax, double *Wc_ymax,
        double *Cc_xmin, double *Cc_ymin, double *Cc_xmax, double *Cc_ymax);

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    /**
     * The handler for a mouse moved event.
     *
     * @param event The mouse event.
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * The handler for a mouse pressed event.
     *
     * @param event The mouse event.
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * The handler for a mouse released event.
     *
     * @param event The mouse event.
     */
    void mouseReleaseEvent(QMouseEvent *event);

    bool eventFilter(QObject *obj, QEvent *ev);

  private:

    int   m_2dWxmin;
    int   m_2dWymin;
    int   m_2dWxmax;
    int   m_2dWymax;
    float m_2dXScale;
    float m_2dYScale;
    float m_2dXOffset;
    float m_2dYOffset;
    float m_2dPixelWidth;
    float m_2dPixelHeight;
    VpGrid *m_2dGrid;

    static const int MAX_WC_EXTENT;
    static const int MIN_WC_EXTENT;

    // The rubber-band.
    QRubberBand *m_rubberBand;
    // The origin of the rubber-band;
    QPoint m_rubberBandOrigin;
    // Flag indicating if currently rubber-banding.
    bool m_rubberBandIsShown;

    QPainter *m_painter;

};

#endif // __AUGRAPHICS2D_H_
