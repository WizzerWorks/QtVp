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
#include <QRect>
#include <QString>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QRubberBand>
#include <QDebug>
#include <QMutex>

// Include QtVp header files.
#include "vptypes.h"
#include "vputil.h"
#include "vpgraphics2d.h"
#include "vpgc.h"

const int VpGraphics2D::MAX_WC_EXTENT = 0x7fffffff;
const int VpGraphics2D::MIN_WC_EXTENT = -VpGraphics2D::MAX_WC_EXTENT;

VpGraphics2D::VpGraphics2D(QWidget *parent)
  : VpViewport(parent)
{
    // Set world coordinate parameters to NULL values.
    m_2dWxmin = 0;
    m_2dWymin = 0;
    m_2dWxmax = 0;
    m_2dWymax = 0;
    m_2dXScale = 0;
    m_2dYScale = 0;
    m_2dXOffset = 0;
    m_2dYOffset = 0;
    m_2dPixelWidth = 0;
    m_2dPixelHeight = 0;
    m_2dGrid = new VpGrid();

    m_painter = new QPainter();

    // Enable mouse tracking.
    setMouseTracking(true);

    // Initialize rubber-banding.
    m_rubberBand = 0;
    m_rubberBandIsShown = false;

    installEventFilter(this);

    connect(this, SIGNAL(mouseMoved(const QMouseEvent &)), this, SLOT(processCoord(const QMouseEvent &)));
}

VpGraphics2D::~VpGraphics2D()
{
    if (m_2dGrid != NULL) delete m_2dGrid;
}

// Adjust the window extent such that it fits the viewport
// without distortion.
bool VpGraphics2D::adjustExtentToViewport(
    VpGraphics2D &vp,
    double *Wc_xmin, double *Wc_ymin,
    double *Wc_xmax, double *Wc_ymax,
    double *Cc_xmin, double *Cc_ymin,
    double *Cc_xmax, double *Cc_ymax)
{
    // Declare local variables.
    double Sxmin, Symin, Sxmax, Symax;
    double d_px, d_py, d_wx, d_wy;
    double xscale, yscale, adjustment;
    double tmpWxmin, tmpWxmax, tmpWymin, tmpWymax;
    double tmpWc_xmin, tmpWc_ymin;

    // Get visible device coordinates of viewport.
    Sxmin = vp.getPxmin();
    Symin = vp.getPymin();
    Sxmax = vp.getPxmax();
    Symax = vp.getPymax();

    // Calculate the width and height of the viewport.
    d_px = Sxmax - Sxmin;
    d_py = Symax - Symin;

    // Calculate the width and height of the window.
    d_wx = *Wc_xmax - *Wc_xmin;
    d_wy = *Wc_ymax - *Wc_ymin;

    // Determine aspect ratio between viewport and window,
    // assume device has a 1:1 pixel spacing ratio.
    if (d_wx != 0) xscale = d_px / d_wx; else xscale = 1;
    if (d_wy != 0) yscale = d_py / d_wy; else yscale = 1;

    // To avoid distortion, the aspect ratio between the world
    // coordinates and the viewport must be identical.  This
    // implies that the x_scale should be equivalent to the
    // y_scale.  If they are not equal, then the window defined
    // by Wc_xmin, Wc_xmax, Wc_ymin and Wc_ymax must be extended
    // in order to preserve the 1-to-1 correspondence between the
    // x and y aspect ratios.  The smallest ratio (either in the
    // x or y direction) must be used in order to fit all of the
    // coordinates within the window into the viewport.  Therefore,
    // a "best-fit" of world coordinates to viewport is obtained.

    if ( xscale < yscale ) {
        // Fit x first and then extend y.
        yscale = xscale;
        adjustment = (d_py / yscale - d_wy) / 2;
        *Wc_ymin = *Wc_ymin - adjustment;
        *Wc_ymax = *Wc_ymax + adjustment;

        // Calculate the new aspect ratio for y.
        d_wy = *Wc_ymax - *Wc_ymin;
        if (d_wy != 0) yscale = d_py / d_wy; else yscale = 1;
    } else {
        // Fit y first and then extend x.
        xscale = yscale;
        adjustment = (d_px / xscale - d_wx) / 2;
        *Wc_xmin = *Wc_xmin - adjustment;
        *Wc_xmax = *Wc_xmax + adjustment;

        // Calculate the new aspect ratio for x.
        d_wx = *Wc_xmax - *Wc_xmin;
        if (d_wx != 0) xscale = d_px / d_wx; else xscale = 1;
    }

    // Save VISIBLE viewport for later use - set clip extent.
    tmpWxmin = *Cc_xmin = *Wc_xmin;
    tmpWxmax = *Cc_xmax = *Wc_xmax;
    tmpWymin = *Cc_ymin = *Wc_ymin;
    tmpWymax = *Cc_ymax = *Wc_ymax;

    if ((*Wc_xmin < MIN_WC_EXTENT) || (*Wc_xmax > MAX_WC_EXTENT) ||
        (*Wc_ymin < MIN_WC_EXTENT) || (*Wc_ymax > MAX_WC_EXTENT))
    {
        // Mapping causes integer overflow - unable to adjust extent.
        //log4c("Unable to adjust extent.");
        return false;
    }

    // Save world coordinates of VISIBLE viewport.
    vp.setWxmin(VpUtil::round(tmpWxmin));
    vp.setWxmax(VpUtil::round(tmpWxmax));
    vp.setWymin(VpUtil::round(tmpWymin));
    vp.setWymax(VpUtil::round(tmpWymax));

    // Calculate and save viewport offsets and scaling factors for general
    // bookkeeping of the viewport record structure - used in scaling
    // utilities.

    // For now, assume the viewport is flipped with respect to the world
    // coordinates, then the y_scale and y_offset must be recalculated.
    vp.setXScale((float)xscale);
    vp.setYScale((float)(-1 * yscale));
    vp.setXOffset((float)(Sxmin - *Wc_xmin * xscale));
    vp.setYOffset((float)(Symax - *Wc_ymin * (-1 * yscale)));

    return true;
}

bool VpGraphics2D::setWorldCoords(int xmin,int ymin,int xmax,int ymax)
{
    // Declare local variables.
    double Wc_xmin,Wc_xmax,Wc_ymin,Wc_ymax;   // World coodinates.
    double Cc_xmin,Cc_xmax,Cc_ymin,Cc_ymax;   // Clip coodinates.
    double tmp;
    float pixelwidth;

    // Note: extent should be calculated in floating point arithmetic
    // using double precision.
    Wc_xmin = xmin; Wc_xmax = xmax;
    Wc_ymin = ymin; Wc_ymax = ymax;

    // Sort window coordinates.
    if (Wc_xmax < Wc_xmin) {tmp = Wc_xmin; Wc_xmin = Wc_xmax; Wc_xmax = tmp;}
    if (Wc_ymax < Wc_ymin) {tmp = Wc_ymin; Wc_ymin = Wc_ymax; Wc_ymax = tmp;}

    // Adjust extent of world coordinates to fit viewport without distortion.
    Cc_xmin = Cc_ymin = Cc_xmax = Cc_ymax = 0;
    if (! adjustExtentToViewport(*this, &Wc_xmin, &Wc_ymin, &Wc_xmax,
        &Wc_ymax, &Cc_xmin, &Cc_ymin, &Cc_xmax, &Cc_ymax))
    {
        //log4c("Unable to adjust extent.");
        qDebug("Unable to adjust extent.");
        return false;
    }

    /*
    // set virtual device coordinates (vdc) to be the same as device
    // coordinates (dc) -- for low-level mapping and clipping
    VP_VDCExtent(vp,Wc_xmin,Wc_ymin,0.0,Wc_xmax,Wc_ymax,1.0);

    // set clipping characteristics
    VP_ClipRectangle(vp,Cc_xmin,Cc_xmax,Cc_ymin,Cc_ymax);
    VP_ClipIndicator(vp,CLIP_TO_RECT);
    */

    // Calculate the size of a single pixel in world coordinates.
    pixelwidth = (float) (Wc_xmax - Wc_xmin + 1) /
        (float) (getPxmax() - getPxmin() + 1);
    setPixelWidth(pixelwidth);
    setPixelHeight(pixelwidth);

    return true;
}

void VpGraphics2D::worldToDev(int *x, int *y)
{
    // Declare local variables.
    int z = 0;
    float fx, fy, fz;

    // Note that the result is int but the calculation is float.
    fx = ((*x) * getXScale()) + getXOffset();
    fy = ((*y) * getYScale()) + getYOffset();

    *x = VpUtil::round(fx);
    *y = VpUtil::round(fy);
}

void VpGraphics2D::devToWorld(int *x, int *y)
{
    // Declare local variables.
    float fx, fy, fz;
    int z = 0;

    fx = ((*x) - getXOffset()) / getXScale();
    fy = ((*y) - getYOffset()) / getYScale();

    *x = VpUtil::round(fx);
    *y = VpUtil::round(fy);
}

QRect *VpGraphics2D::worldToDevRect(int xmin, int ymin, int xmax, int ymax)
{
    // Declare local variables.
    int temp;

    // Transform world coordinates to device coordinates.
    worldToDev(&xmin, &ymin);
    worldToDev(&xmax, &ymax);

    if (xmin > xmax) {
        temp = xmin; xmin = xmax; xmax = temp;
    }

    if (ymin > ymax) {
        temp = ymin; ymin = ymax; ymax = temp;
    }

    QRect *rect = new QRect(xmin, ymin, xmax, ymax);

    return rect;
}

void VpGraphics2D::scaleWorldToDev(int *x, int *y)
{
    *x = VpUtil::round((float)(*x) * getXScale());
    *y = VpUtil::round((float)(*y) * getYScale());
}

void VpGraphics2D::scaleDevToWorld(int *x, int *y)
{
    *x = VpUtil::round((float)(*x) / getXScale());
    *y = VpUtil::round((float)(*y) / getYScale());
}

bool VpGraphics2D::intersectWorld(VpGraphics2D &vp, int xll, int yll, int xur, int yur)
{
    return (! (xll > vp.getWxmax() || xur < vp.getWxmin() ||
               yll > vp.getWymax() || yur < vp.getWymin()));
}

// Grid drawing utilities.

bool VpGraphics2D::drawGrid(VpGC *gc)
{
    // Declare local variables.
    int i, j, tmp, dx, dy;
    int halfdx, halfdy, savx, savy;
    double pixdx, pixdy;
    int xll, yll, xur, yur;
    int truexll, trueyll, truexur, trueyur;
    bool status;
    int count=0, xnum=0, ynum=0;
    GridContext *gridGC = new GridContext();

    // Set return point status for interrupts.
    /*
    status = AS_SET_RETURN_POINT();
    if (status != 0)
    {
        // Interrupted! Abort grid display.

        // Restore x and y spacing parameters - flush display.
        m_2dGrid.setXSpacing(savx);
        m_2dGrid.setYSpacing(savy);

        // Clear return points and interrupt handling.
        AS_restore_sig();
        AS_CLEAR_RETURN_POINT();
        return(true);
        // XXX - Should probably return more information here in the future.
    }
    */

    // Save spacing and use display spacing to snap boundaries.

    // The following two lines must occur before enabling the interrupt.
    // This guarantees that we have valid spacing values to restore to
    // the grid record strructure upon interrupting the display and
    // returning to the calling command (i.e. grid or window).  See
    // above return point!
    savx = m_2dGrid->getXSpacing();
    savy = m_2dGrid->getYSpacing();

    // Initialize interrupt handling routine.
    //AS_trap_to(VP_handle_grid_break);

    dx = m_2dGrid->getXSpacing() * m_2dGrid->getMultiplier();
    dy = m_2dGrid->getYSpacing() * m_2dGrid->getMultiplier();
    halfdx = dx >> 1;
    halfdy = dy >> 1;
    if (getPixelWidth() != 0)
        pixdx = (double) dx / getPixelWidth();
    else
        pixdx = 0;
    if (getPixelHeight() != 0)
        pixdy = (double) dy / getPixelHeight();
    else
        pixdy = 0;

    if (((halfdx >= getPixelWidth()) && (halfdy >= getPixelHeight())) &&
        ((pixdx >= m_2dGrid->getXResolution()) &&
         (pixdy >= m_2dGrid->getYResolution())))
    {
        switch (m_2dGrid->getStyle())
        {
            case VpGrid::STYLE_LINE:

                xll = getWxmin() - halfdx;
                yll = getWymin() - halfdy;
                xur = getWxmax() + halfdx;
                yur = getWymax() + halfdy;

                // Snap to new temporary spacing values stored
                // in the grid object.
                m_2dGrid->snapToGrid(&xll, &yll);
                m_2dGrid->snapToGrid(&xur, &yur);

                // Calculate the number of grid primitives.
                for (i = xll; i <= xur; i = i + dx )
                    xnum++;
                for (j = yll; j <= yur; j = j + dy )
                    ynum++;

                // Get true dc values (non-snapped) for clipping
                // against vp extent.
                truexll = getWxmin();
                trueyll = getWymin();
                truexur = getWxmax();
                trueyur = getWymax();
                worldToDev(&truexll, &trueyll);
                worldToDev(&truexur, &trueyur);
                if (truexur < truexll) {
                    tmp = truexll;
                    truexll = truexur;
                    truexur = tmp;
                }
                if (trueyur < trueyll) {
                    tmp = trueyll;
                    trueyll = trueyur;
                    trueyur = tmp;
                }

                // Fill-out grid extent data.
                gridGC->m_gc = gc;
                gridGC->m_xll = xll;
                gridGC->m_yll = yll;
                gridGC->m_xur = xur;
                gridGC->m_yur = yur;
                gridGC->m_truexll = truexll;
                gridGC->m_trueyll = trueyll;
                gridGC->m_truexur = truexur;
                gridGC->m_trueyur = trueyur;
                gridGC->m_xnum = xnum;
                gridGC->m_ynum = ynum;
                gridGC->m_dx = dx;
                gridGC->m_dy = dy;

                // Draw the dot grid.
                m_2dGrid->draw(*gridGC);
                break;

            case VpGrid::STYLE_DOT:

                m_2dGrid->setXSpacing(dx);
                m_2dGrid->setYSpacing(dy);
                xll = getWxmin() + halfdx;
                yll = getWymin() + halfdy;
                xur = getWxmax() - halfdx;
                yur = getWymax() - halfdy;

                // Snap to new temporary spacing values stored
                // in the grid object.
                m_2dGrid->snapToGrid(&xll, &yll);
                m_2dGrid->snapToGrid(&xur, &yur);

                // Calculate the number of grid primitives.
                for (i = xll; i <= xur; i = i + dx )
                    xnum++;
                for (j = yll; j <= yur; j = j + dy )
                    ynum++;

                // Get true dc values (non-snapped) for clipping
                // against vp extent.
                truexll = getWxmin();
                trueyll = getWymin();
                truexur = getWxmax();
                trueyur = getWymax();
                worldToDev(&truexll, &trueyll);
                worldToDev(&truexur, &trueyur);
                if (truexur < truexll) {
                    tmp = truexll;
                    truexll = truexur;
                    truexur = tmp;
                }
                if (trueyur < trueyll) {
                    tmp = trueyll;
                    trueyll = trueyur;
                    trueyur = tmp;
                }

                // Fill-out grid extent data.
                gridGC->m_gc = gc;
                gridGC->m_xll = xll;
                gridGC->m_yll = yll;
                gridGC->m_xur = xur;
                gridGC->m_yur = yur;
                gridGC->m_truexll = truexll;
                gridGC->m_trueyll = trueyll;
                gridGC->m_truexur = truexur;
                gridGC->m_trueyur = trueyur;
                gridGC->m_xnum = xnum;
                gridGC->m_ynum = ynum - 1;
                gridGC->m_dx = dx;
                gridGC->m_dy = dy;

                // Draw the dot grid.
                m_2dGrid->draw(*gridGC);
                break;

            case VpGrid::STYLE_CROSS:

                xll = getWxmin() - halfdx;
                yll = getWymin() - halfdy;
                xur = getWxmax() + halfdx;
                yur = getWymax() + halfdy;


                // Snap to new temporary spacing values stored
                // in the grid object.
                m_2dGrid->snapToGrid(&xll, &yll);
                m_2dGrid->snapToGrid(&xur, &yur);

                // Calculate the number of grid primitives.
                for (i = xll; i <= xur; i = i + dx )
                    xnum++;
                for (j = yll; j <= yur; j = j + dy )
                    ynum++;

                // Get true dc values (non-snapped) for clipping
                // against vp extent.
                truexll = getWxmin();
                trueyll = getWymin();
                truexur = getWxmax();
                trueyur = getWymax();
                worldToDev(&truexll, &trueyll);
                worldToDev(&truexur, &trueyur);
                if (truexur < truexll) {
                    tmp = truexll;
                    truexll = truexur;
                    truexur = tmp;
                }
                if (trueyur < trueyll) {
                    tmp = trueyll;
                    trueyll = trueyur;
                    trueyur = tmp;
                }

                // Fill-out grid extent data.
                gridGC->m_gc = gc;
                gridGC->m_xll = xll;
                gridGC->m_yll = yll;
                gridGC->m_xur = xur;
                gridGC->m_yur = yur;
                gridGC->m_truexll = truexll;
                gridGC->m_trueyll = trueyll;
                gridGC->m_truexur = truexur;
                gridGC->m_trueyur = trueyur;
                gridGC->m_xnum = xnum - 1;
                gridGC->m_ynum = ynum - 1;
                gridGC->m_dx = dx;
                gridGC->m_dy = dy;

                // Draw the cross grid.
                m_2dGrid->draw(*gridGC);
                break;

            case VpGrid::STYLE_UNKNOWN:
                break;

        } // switch

        // Restore x and y spacing parameter.
        m_2dGrid->setXSpacing(savx);
        m_2dGrid->setYSpacing(savy);

        // Clear return points and interrupt handling.
        //AS_CLEAR_RETURN_POINT();
        //AS_restore_sig();

        status = true;
    } else
    {
        // Restore x and y spacing parameters.
        m_2dGrid->setXSpacing(savx);
        m_2dGrid->setYSpacing(savy);

        // Clear return points and interrupt handling.
        //AS_CLEAR_RETURN_POINT();
        //AS_restore_sig();

        status = false;
    }

    delete gridGC;
    return status;
}

bool VpGraphics2D::drawGridReference(VpGC *gc)
{
    // Declare local variables.
    int status = true;
    GridContext *gridGC = new GridContext();

    // Set return point status for interrupts.
    /*
    status = AS_SET_RETURN_POINT();
    if (status != 0)
    {
        // Interrupted! Abort grid display.
        AS_restore_sig();
        AS_CLEAR_RETURN_POINT();
        return(true);
        // Should probably return more information here in the future.
    }

    // Initialize interrupt handling routine.
    AS_trap_to(VP_handle_grid_break);
    */

    // Fill-out grid extent data.
    gridGC->m_gc = gc;

    // Draw the dot grid.
    m_2dGrid->drawReference(*gridGC);

    // Clear return points and interrupt handling.
    //AS_CLEAR_RETURN_POINT();
    //AS_restore_sig();

    delete gridGC;
    return status;
}

bool VpGraphics2D::displayGrid(VpGC *gc)
{
    // Declare local variables.
    bool retValue = true;

    if (m_2dGrid->getState() == VpGrid::STATE_ON)
    {
        if (drawGrid(gc) == false)
        {
            QString msg(getName());
            msg.append(tr(" : grid is too fine to be displayed."));
            // TODO: The following updateStatus causes an exception to be thrown in
            // drawGridReference below. Fix it.
            emit updateStatus(msg);
            //qDebug(msg.toLocal8Bit().data());
            retValue = false;
        }
    }

    // Display the grid reference regardless of whether the grid
    // is successfully displayed.
    if ((retValue == true) && m_2dGrid->isReferenceOn())
        drawGridReference(gc);

    return(retValue);
}

bool VpGraphics2D::eraseGrid(VpGC *gc)
{
    // Declare local variables.
    bool retValue = true;

    if (m_2dGrid->getState() == VpGrid::STATE_ON)
    {
        // Erase grid pattern.

        // Note: use current pixel resolution stored in grid record!
        //       Not vp_gridxres and vp_gridyres.
        //VP_SetGridDisplay(m_2dGrid.getColor());
        if (drawGrid(gc) == false)
        {
            QString msg(getName());
            msg.append(tr(" : grid is too fine to be displayed.\n"));
            emit updateStatus(msg);
            //gDebug(msg.toLocal8Bit().data());
            retValue = false;
        }
    }

    return(retValue);
}

void VpGraphics2D::updateGrid(VpGC *gc, const GridState &dispState)
{
    // Declare local variables.
    VpGrid::State state;
    VpGrid::Style style;
    int xSpacing, ySpacing, multiplier;
    VpColor color;
    int xAlignment,yAlignment;
    bool statusChanged = false;

    if (dispState.m_state != VpGrid::STATE_UNKNOWN)
    {
        state = dispState.m_state;
        if (state != m_2dGrid->getState())
            statusChanged = true;
    } else
        state = m_2dGrid->getState();

    if (dispState.m_style != VpGrid::STYLE_UNKNOWN)
    {
        style = dispState.m_style;
        if (style != m_2dGrid->getStyle())
            statusChanged = true;
    } else
        style = m_2dGrid->getStyle();

    if (dispState.m_color != m_2dGrid->getColor())
    {
        color = dispState.m_color;
        statusChanged = true;
    } else
        color = m_2dGrid->getColor();

    if (dispState.m_xSpacing != -1)
    {
        xSpacing = VpCoord::worldToInternal(dispState.m_xSpacing);
        if (xSpacing != m_2dGrid->getXSpacing())
            statusChanged = true;
    } else
        xSpacing = m_2dGrid->getXSpacing();

    if (dispState.m_ySpacing != -1)
    {
        ySpacing = VpCoord::worldToInternal(dispState.m_ySpacing);
        if (ySpacing != m_2dGrid->getYSpacing())
            statusChanged = true;
    } else
        ySpacing = m_2dGrid->getYSpacing();

    if (dispState.m_multiplier != -1)
    {
         multiplier = dispState.m_multiplier;
         if (multiplier != m_2dGrid->getMultiplier())
             statusChanged = true;
    } else
         multiplier = m_2dGrid->getMultiplier();

    VpCoord alignment = dispState.m_alignment;
    if ((alignment.getX() != m_2dGrid->getXAlignment()) ||
        (alignment.getY() != m_2dGrid->getYAlignment()))
    {
         xAlignment = alignment.getX();
         yAlignment = alignment.getY();
         statusChanged = true;
    } else
    {
         xAlignment = m_2dGrid->getXAlignment();
         yAlignment = m_2dGrid->getYAlignment();
    }

    // Don't forget to see if the pixel resolution has been changed.
    if ((VpGrid::getGridXResolution() != m_2dGrid->getXResolution()) ||
        (VpGrid::getGridYResolution() != m_2dGrid->getYResolution()))
        statusChanged = true;

    if (statusChanged)
    {
        eraseGrid(gc);

        m_2dGrid->setState(state);
        m_2dGrid->setStyle(style);
        m_2dGrid->setColor(color);
        m_2dGrid->setMultiplier(multiplier);
        m_2dGrid->setXAlignment(xAlignment);
        m_2dGrid->setYAlignment(yAlignment);
        m_2dGrid->setXSpacing(xSpacing);
        m_2dGrid->setYSpacing(ySpacing);
        m_2dGrid->setXResolution(VpGrid::getGridXResolution());
        m_2dGrid->setYResolution(VpGrid::getGridYResolution());

        displayGrid(gc);
    }
}

void VpGraphics2D::updateGridReference(VpGC *gc, const GridState &dispState)
{
    // Declare local variables.
    VpGrid::RefState state;
    VpGrid::RefStyle style;
    VpColor color;
    bool statusChanged = false;

    if (dispState.m_referenceState != VpGrid::REFSTATE_UNKNOWN)
    {
        state = dispState.m_referenceState;
        if (state != m_2dGrid->getReferenceState())
            statusChanged = true;
    } else
        state = m_2dGrid->getReferenceState();

    if (dispState.m_referenceStyle != VpGrid::REFSTYLE_UNKNOWN)
    {
        style = dispState.m_referenceStyle;
        if (style != m_2dGrid->getReferenceStyle())
            statusChanged = true;
    } else
        style = m_2dGrid->getReferenceStyle();

    if (dispState.m_referenceColor != m_2dGrid->getReferenceColor())
    {
        color = dispState.m_referenceColor;
        if (color != m_2dGrid->getColor())
            statusChanged = true;
    } else
        color = m_2dGrid->getReferenceColor();

    if (statusChanged)
    {
        m_2dGrid->setReferenceState(state);
        m_2dGrid->setReferenceStyle(style);
        m_2dGrid->setReferenceColor(color);

        if ((m_2dGrid->getState() == VpGrid::STATE_ON) ||
            (m_2dGrid->getState() == VpGrid::STATE_HIDDEN))
            // Always draw the reference, even if the grid is hidden.
            drawGridReference(gc);
    }
}

void VpGraphics2D::snapToGrid(int *x, int *y)
{
    m_2dGrid->snapToGrid(x, y);
}

QString VpGraphics2D::toString()
{
    // Declare local variables.
    QString state_str;
    QString style_str;
    QString display_ref_str;
    double xspacing_d, yspacing_d;

    // Set output string for state.
    switch (m_2dGrid->getState())
    {
        case VpGrid::STATE_ON : state_str = tr("on"); break;
        case VpGrid::STATE_OFF : state_str = tr("off"); break;
        case VpGrid::STATE_HIDDEN : state_str = tr("hidden"); break;
        case VpGrid::STATE_UNKNOWN : return QString();
    }

    // Set output string for style.
    switch (m_2dGrid->getStyle())
    {
        case VpGrid::STYLE_LINE : style_str = tr("line"); break;
        case VpGrid::STYLE_DOT : style_str = tr("dot"); break;
        case VpGrid::STYLE_CROSS : style_str = tr("cross"); break;
        case VpGrid::STYLE_UNKNOWN : return QString();
    }

    // Set output string for state of grid reference marker.
    if (m_2dGrid->isReferenceOn())
        display_ref_str = tr("on");
    else
        display_ref_str = tr("off");

    // Set resolution for x and y spacing.
    xspacing_d = (double) m_2dGrid->getXSpacing();
    yspacing_d = (double) m_2dGrid->getYSpacing();

    // Output info.
    QString gridCmd(tr("vpgrid\t"));
    gridCmd.append(getName());
    gridCmd.append(tr(" -s ")); gridCmd.append(state_str);
    gridCmd.append(tr(" -r ")); gridCmd.append(display_ref_str);
    gridCmd.append(tr(" -t ")); gridCmd.append(style_str);
    gridCmd.append(tr(" -i ")); gridCmd.append(QString("%1").arg(m_2dGrid->getColor().value()));
    gridCmd.append(tr(" -x ")); gridCmd.append(QString("%1").arg(VpCoord::internalToWorld((int)xspacing_d)));
    gridCmd.append(tr(" -y ")); gridCmd.append(QString("%1").arg(VpCoord::internalToWorld((int)yspacing_d)));
    gridCmd.append(tr(" -m ")); gridCmd.append(QString("%1").arg(m_2dGrid->getMultiplier()));
    gridCmd.append(tr(" (")); gridCmd.append(QString("%1").arg(VpCoord::internalToWorld(m_2dGrid->getXAlignment())));
    gridCmd.append(tr(",")); gridCmd.append(QString("%1").arg(VpCoord::internalToWorld(m_2dGrid->getYAlignment())));
    gridCmd.append(tr(")"));

    qDebug(gridCmd.toLocal8Bit().data());

    return gridCmd;
}

void VpGraphics2D::clear()
{
    // Get the size of the available drawing area.
    QRect clientArea = rect();

    //QPainter gc(this);
    //gc.eraseRect(clientArea);
    m_painter->begin(this);
    m_painter->eraseRect(clientArea);
    m_painter->end();
}

QMutex mutex;

void VpGraphics2D::resizeEvent(QResizeEvent *event)
{
    int x_min, y_min, x_max, y_max;
    int Sx_min, Sy_min, Sx_max, Sy_max;

    //qDebug("VpGraphics2D: Resize event.");
    QMutexLocker locker(&mutex);

    Sx_min = getPxmin();
    Sx_max = getPxmax();
    Sy_min = getPymin();
    Sy_max = getPymax();

    // Initialize extent of physical coordinate system.
    QSize size = event->size();
    setPxmin(0);
    setPxmax(size.width());
    setPymin(0);
    setPymax(size.height());

    QSize oldSize = event->oldSize();
    if ((oldSize.width() == -1) && (oldSize.height() == -1))
    {
        // Set world coordinates to match resized window.

        // Initialize extent of world coordinate system.
        x_min = getPxmin() * VpCoord::getResolution();
        y_min = getPymin() * VpCoord::getResolution();
        x_max = getPxmax() * VpCoord::getResolution();
        y_max = getPymax() * VpCoord::getResolution();

        setWorldCoords(x_min, y_min, x_max, y_max);
    } else
    {
        // Set existing world coordinate extent to new
        // resized, physical coordinat extent.
        x_min = getWxmin();
        x_max = getWxmax();
        y_min = getWymin();
        y_max = getWymax();
        setWorldCoords(x_min, y_min, x_max, y_max);
    }
}

void VpGraphics2D::paintEvent(QPaintEvent *event)
{
    //qDebug("VpGraphics2D: Paint event.");
    QMutexLocker locker(&mutex);

    // Clear the viewport.
    clear();

    // Create the Qt graphics context.
    QPainter *gc = m_painter;
    gc->begin(this);

    // Set world coordinate extent.
    QRect extent;
    extent.setLeft(getWxmin());
    extent.setRight(getWxmax());
    extent.setTop(getWymax());
    extent.setBottom(getWymin());
    m_painter->setWindow(extent);

    // Set up the viewport context.
    VpGC vpgc;
    vpgc.setViewport(this);
    vpgc.setGC(gc);

    // Erase the previous grid.
    //eraseGrid(vpgc);

    // Display the grid.
    //m_vp.getGrid().setColor(new AuColor(0, 0, 255));
    if (displayGrid(&vpgc))
    {
        /*
        QString msg(getName());
        msg.append(tr(": grid displayed successfully."));
        emit updateStatus(msg);
        */
    }

    // Complete painting.
    gc->end();
}

bool VpGraphics2D::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == this)
    {
        if (ev->type() == QEvent::Leave)
        {
            QString coord("");
            emit coordChanged(coord);
            return true;
        } else {
            return false;
        }
    } else {
        // Pass the event on to the parent class.
        return QWidget::eventFilter(obj, ev);
    }
}

void VpGraphics2D::mousePressEvent(QMouseEvent *event)
{
    int scrx, scry;

    //qDebug("VpGraphics2D: Mouse press event.");

    // Get device coordinate from event.
    scrx = event->x();
    scry = event->y();

    if (m_2dGrid->getState() != VpGrid::STATE_OFF)
    {
        // Translate device coodinate into world coordinate.
        devToWorld(&scrx, &scry);

        // Snap to the nearest grid coordinate.
        snapToGrid(&scrx, &scry);

        // Translate world coordinate back into device coordinate.
        worldToDev(&scrx, &scry);
    }

    if (event->button() == Qt::LeftButton)
    {
        // Start rubber-banding mode.
        QPoint pos;
        pos.setX(scrx);
        pos.setY(scry);
        m_rubberBandOrigin = pos;
        if (! m_rubberBand)
            // Create rubber-band if necessary.
            m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        m_rubberBand->setGeometry(QRect(m_rubberBandOrigin, QSize()));
        m_rubberBand->show();
        m_rubberBandIsShown = true;

        // Set the cursor.
        setCursor(Qt::CrossCursor);
    }

    // Send the signal by forwarding the event.
    emit mousePressed(*event);

    // Make sure events don't propagate to the parent.
    event->accept();
}

void VpGraphics2D::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_rubberBandIsShown)
    {
        m_rubberBand->hide();

        // Determine selection, for example using QRect::intersects()
        // and QRect::contains().

        m_rubberBandIsShown = false;

        // Unset the cursor.
        unsetCursor();
    }

    // Send the signal by forwarding the event.
    emit mouseReleased(*event);

    // Make sure events don't propagate to the parent.
    event->accept();
}

void VpGraphics2D::mouseMoveEvent(QMouseEvent *event)
{
   int scrx, scry;

    // Get device coordinate from event.
    scrx = event->x();
    scry = event->y();

    if (m_2dGrid->getState() != VpGrid::STATE_OFF)
    {
        // Translate device coodinate into world coordinate.
        devToWorld(&scrx, &scry);

        // Snap to the nearest grid coordinate.
        snapToGrid(&scrx, &scry);

        // Translate world coordinate back into device coordinate.
        worldToDev(&scrx, &scry);
    }

    QPoint pos;
    pos.setX(scrx);
    pos.setY(scry);
    if (m_rubberBandIsShown)
        m_rubberBand->setGeometry(QRect(m_rubberBandOrigin, pos).normalized());

    // Send the signal by forwarding the event.
    emit mouseMoved(*event);

    // Make sure events don't propagate to the parent.
    event->accept();
}

void VpGraphics2D::processCoord(const QMouseEvent &event)
{
    static char coord_buf[100];
    int scrx, scry;
    float f_scrx, f_scry;

    //qDebug("VpGraphics2D: Processing viewport coordinate.");

    // Get device coordinate from event.
    scrx = event.x();
    scry = event.y();

    // Translate device coodinate into world coordinate.
    devToWorld(&scrx, &scry);

    if (m_2dGrid->getState() != VpGrid::STATE_OFF)
        snapToGrid(&scrx, &scry);

    f_scrx = ((float) scrx)/((float) VpCoord::getResolution());
    f_scry = ((float) scry)/((float) VpCoord::getResolution());
    QString name = getName();
    if ((name.isNull()) || (name.isEmpty())) {
        sprintf_s(coord_buf,"(%.2lf,%.2lf)", f_scrx, f_scry);
    } else {
        QByteArray ba = name.toLocal8Bit();
        sprintf_s(coord_buf,"(%.2lf,%.2lf,%s)", f_scrx, f_scry, ba.data());
    }
    QString coord(coord_buf);

    emit coordChanged(coord);
}
