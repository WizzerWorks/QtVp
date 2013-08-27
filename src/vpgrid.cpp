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
#include <QPoint>

// Include QtVp heaeder files.
#include "vputil.h"
#include "vpgrid.h"
#include "vpgc.h"
#include "vpgraphics2d.h"

/*   The variable g_gridXResolution is an integer which the user may set to   */
/*   specify the minumum distance (in pixels) between successive grid         */
/*   primitives along the x axis.  Similarly, the variable g_gridYResolution  */
/*   is a variables which the user may set to specify the minumum distance    */
/*   (in pixels) between successive grid primitives along the y axis.         */

int VpGrid::g_gridXResolution = 1;
int VpGrid::g_gridYResolution = 1;

VpGrid::VpGrid(QObject *parent) :
    QObject(parent)
{
    // Initialize grid to default characteristics.
    init();
}

VpGrid::~VpGrid()
{
    // Do nothing.
}

bool VpGrid::init()
{
    bool status = false;

    // Initialize this objects private data.
    setState(STATE_OFF);
    setStyle(STYLE_LINE);
    m_color.setRed(0);
    m_color.setGreen(0);
    m_color.setBlue(0);
    m_color.setAlpha(255);
    setXSpacing(VpGrid::getGridUnit());
    setYSpacing(VpGrid::getGridUnit());
    setMultiplier(1);
    setXAlignment(0);
    setYAlignment(0);
    setXResolution(1);
    setYResolution(1);
    setReferenceState(VpGrid::REFSTATE_OFF);
    setReferenceStyle(VpGrid::REFSTYLE_SQUARE);
    m_referenceColor.setRed(0);
    m_referenceColor.setGreen(0);
    m_referenceColor.setBlue(0);
    m_referenceColor.setAlpha(255);

    status = true;
    return status;
}

bool VpGrid::snapToGrid(int *x, int *y)
{
    // Declare local variables.
    int sx, sy, tx, ty;
    bool status = false;

    if (getState() != STATE_OFF)
    {
        // Snap without dead band.

        sx = getXSpacing();
        sy = getYSpacing();
        tx = getXAlignment();
        ty = getYAlignment();
        *x = VpUtil::round(((double)(*x - tx))/((double)(sx))) * sx + tx;
        *y = VpUtil::round(((double)(*y - ty))/((double)(sy))) * sy + ty;
    }

    status = true;
    return status;
}

bool VpGrid::getGridCoord(int *x, int *y)
{
    bool status = false;

    *x = getXSpacing() * getMultiplier();
    *y = getYSpacing() * getMultiplier();

    status = true;
    return status;
}

bool VpGrid::isReferenceOn()
{
    RefState state = getReferenceState();
    if (state == REFSTATE_ON)
        return true;
    else
        return false;
}

void VpGrid::draw(GridContext &gridGC)
{
    // Set up the display characteristics.

    // Draw the grid pattern.
    switch(m_style) {
        case STYLE_LINE:
            drawLineGrid(gridGC);
            break;
        case STYLE_DOT:
            drawDotGrid(gridGC);
            break;
        case STYLE_CROSS:
            drawCrossGrid(gridGC);
            break;
        default:
            drawLineGrid(gridGC);
            break;
    }
}

void VpGrid::drawLineGrid(GridContext &gridGC)
{
    int x,y;
    VpGC *vpgc = gridGC.m_gc;
    QPainter *gc = vpgc->getGC();

    // Assuming QPainter has already established begin().
    gc->setRenderHint(QPainter::Antialiasing, true);

    // Set the pen.
    QPen pen;
    pen.setColor(m_color);
    pen.setStyle(Qt::SolidLine);
    gc->setPen(pen);
    // Set the brush.
    QBrush brush;
    brush.setColor(m_color);
    brush.setStyle(Qt::SolidPattern);
    gc->setBrush(brush);

    // Draw the grid.
    for (int i = 1; i < gridGC.m_xnum; i++)
    {
        x = gridGC.m_xll + (i * gridGC.m_dx);
        y = gridGC.m_yll;
        gc->drawLine(x, gridGC.m_yll, x, gridGC.m_yur);
    }

    for (int j = 1; j < gridGC.m_ynum; j++)
    {
        x = gridGC.m_xll;
        y = gridGC.m_yll + (j * gridGC.m_dy);
        gc->drawLine(gridGC.m_xll, y, gridGC.m_xur, y);
    }

    // Flush graphics to display.
    //gc.flush();

    //delete gc;
}

void VpGrid::drawDotGrid(GridContext &gridGC)
{
    int x,y;
    VpGC *vpgc = gridGC.m_gc;
    QPainter *gc = vpgc->getGC();

    // Assuming QPainter has already established begin().
    gc->setRenderHint(QPainter::Antialiasing, true);

    // Set the pen.
    QPen pen;
    pen.setColor(m_color);
    pen.setStyle(Qt::SolidLine);
    gc->setPen(pen);

    for (int i = 0; i < gridGC.m_ynum + 1; i++) {
        y = gridGC.m_yll + (i * gridGC.m_dy);
        for (int j = 0; j < gridGC.m_xnum; j++) {
            x = gridGC.m_xll + (j * gridGC.m_dx);
            gc->drawPoint(x, y);
        }
    }

    // Flush graphics to display.
    //gc.flush();

    //delete gc;
}

void VpGrid::drawCrossGrid(GridContext &gridGC)
{
    int x,y;
    VpGC *vpgc = gridGC.m_gc;
    QPainter *gc = vpgc->getGC();

    // Assuming QPainter has already established begin().
    gc->setRenderHint(QPainter::Antialiasing, true);

    // Set the pen.
    QPen pen;
    pen.setColor(m_color);
    pen.setStyle(Qt::SolidLine);
    gc->setPen(pen);

    // Create a cross Marker.
    QLine cross[2];

    for (int i = 0; i < gridGC.m_ynum + 1; i++) {
        y = gridGC.m_yll + (i * gridGC.m_dy);
        for (int j = 0; j < gridGC.m_xnum; j++) {
            x = gridGC.m_xll + (j * gridGC.m_dx);
            cross[0].setLine(x-1, y, x+1, y);
            cross[1].setLine(x, y-1, x, y+1);
            gc->drawLines(cross, 2);
        }
    }

    // Flush graphics to display.
    //gc.flush();

    //delete gc;
}

 void VpGrid::drawReference(GridContext &gridGC)
 {
     int x,y;
     VpGC *vpgc = gridGC.m_gc;
     QPainter *gc = vpgc->getGC();

     // Assuming QPainter has already established begin().
     gc->setRenderHint(QPainter::Antialiasing, true);

     if (m_referenceStyle == VpGrid::REFSTYLE_SQUARE)
     {
         // Set the pen.
         QPen pen;
         pen.setStyle(Qt::NoPen);
         gc->setPen(pen);
         // Set the brush.
         QBrush brush;
         brush.setColor(m_referenceColor);
         brush.setStyle(Qt::SolidPattern);
         gc->setBrush(brush);

         QRectF origin;
         origin.setLeft(-1.5 + m_xAlignment);
         origin.setRight(1.5 + m_xAlignment);
         origin.setBottom(-1.5 + m_yAlignment);
         origin.setTop(1.5 + m_yAlignment);

         gc->drawRect(origin);
     } else if (m_referenceStyle == VpGrid::REFSTYLE_CIRCLE)
     {
         // Set the pen.
         QPen pen;
         pen.setStyle(Qt::NoPen);
         gc->setPen(pen);
         // Set the brush.
         QBrush brush;
         brush.setColor(m_referenceColor);
         brush.setStyle(Qt::SolidPattern);
         gc->setBrush(brush);

         gc->drawEllipse(QPoint(m_xAlignment, m_yAlignment), 2, 2);
     } else
     {
         // Set the pen, no brush.
         QPen pen;
         pen.setColor(m_referenceColor);
         pen.setStyle(Qt::SolidLine);
         pen.setWidth(2);
         gc->setPen(pen);

         // Create a 'X' pattern.
         QLineF cross[2];
         cross[0].setLine(-1.5 + m_xAlignment, -1.5 + m_yAlignment, 1.5 + m_xAlignment, 1.5 + m_yAlignment);
         cross[1].setLine(-1.5 + m_xAlignment, 1.5 + m_yAlignment, 1.5 + m_xAlignment, -1.5 + m_yAlignment);
         gc->drawLines(cross, 2);
     }

     //delete gc;
 }

#if 0
void AuGrid::drawCrossGrid2(GridContext &gridGC)
{
    AuViewportGC *vpgc = gridGC.m_gc;
    QPainter *gc = new QPainter(vpgc->getGC());

    Device device = Display.getCurrent();
    Image pixelArray = new Image(device, gridGC.m_truexur - gridGC.m_truexll, 1);
    Image pixelArrayCross = new Image(device, gridGC.m_truexur - gridGC.m_truexll, 1);

    GC pixelArrayGC = new GC(pixelArray);
    GC pixelArrayCrossGC = new GC(pixelArrayCross);
    try {
        org.eclipse.swt.graphics.Color foregroundColor = new org.eclipse.swt.graphics.Color(device,
                m_color.getRed(), m_color.getGreen(), m_color.getBlue());
        Color vpBackgroundColor = gc.getBackground();
        org.eclipse.swt.graphics.Color backgroundColor = new org.eclipse.swt.graphics.Color(device,
            vpBackgroundColor.getRed(),	vpBackgroundColor.getGreen(), vpBackgroundColor.getBlue());

        pixelArrayGC.setForeground(foregroundColor);
        pixelArrayGC.setBackground(backgroundColor);
        pixelArrayGC.setLineStyle(STYLE_DOT);
        pixelArrayGC.fillRectangle(0, 0, gridGC.m_truexur - gridGC.m_truexll, 1);

        pixelArrayCrossGC.setForeground(foregroundColor);
        pixelArrayCrossGC.setBackground(backgroundColor);
        pixelArrayCrossGC.setLineStyle(STYLE_DOT);
        pixelArrayCrossGC.fillRectangle(0, 0, gridGC.m_truexur - gridGC.m_truexll, 1);
    } catch (Exception ex)
    {
        System.out.println("Unable to draw cross: " +
                ex.getMessage());
    }

    for (int i = 0; i <= gridGC.m_xnum; i++)
    {
        int x = gridGC.m_xll + (i * gridGC.m_dx);
        int y = gridGC.m_yll;

        // Convert world coordinates to device coordinates.
        VpCoord coord = ((VpGraphics2D)gc.m_viewport).worldToDev(x, y);

        if (((coord.m_x - 1) >= gridGC.m_truexll) && ((coord.m_x + 1) <= gridGC.m_truexur))
        {
            pixelArrayGC.drawOval(coord.m_x - gridGC.m_truexll - 1, 0, 1, 1);
            pixelArrayGC.drawOval(coord.m_x - gridGC.m_truexll, 0, 1, 1);
            pixelArrayGC.drawOval(coord.m_x - gridGC.m_truexll + 1, 0, 1, 1);
            pixelArrayCrossGC.drawOval(coord.m_x - gridGC.m_truexll, 0, 1, 1);
        }
    }

    for (int j = 0; j <= gridGC.m_ynum; j++)
    {
        int x = gridGC.m_xll;
        int y = gridGC.m_yll + (j * gridGC.m_dy);

        // Convert world coordinates to device coordinates.
        VpCoord coord = ((VpGraphics2D)gc.m_viewport).worldToDev(x, y);

        if (((coord.m_y - 1) >= gridGC.m_trueyll) && ((coord.m_y + 1) <= gridGC.m_trueyur))
        {
            try {
                gc.drawImage(pixelArrayCross, gridGC.m_truexll, coord.m_y - 1);
                gc.drawImage(pixelArray, gridGC.m_truexll, coord.m_y);
                gc.drawImage(pixelArrayCross, gridGC.m_truexll, coord.m_y + 1);
            } catch (Exception ex) {
                System.out.println("Unable to draw cross: " +
                    ex.getMessage());
            }
        }
    }

    // Flush graphics to display.
    //gc.flush();

    // Dispose of the cross image resources.
    pixelArrayGC.dispose();
    pixelArrayCrossGC.dispose();
    pixelArray.dispose();
    pixelArrayCross.dispose();
}
#endif /* 0 */
