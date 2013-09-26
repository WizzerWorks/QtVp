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

#ifndef __VPGRID_H_
#define __VPGRID_H_

// Include Qt header files.
#include <QObject>

// Include QtVp header files.
#include "qtvp_global.h"
#include "vptypes.h"
#include "vpcoord.h"
#include "vpcolor.h"

// Forward references.
class VpGC;
class GridGC;
struct GridState;


class QTVPSHARED_EXPORT VpGrid : public QObject
{
    Q_OBJECT

    Q_ENUMS(State)
    Q_ENUMS(Style)
    Q_ENUMS(RefState)
    Q_ENUMS(RefStyle)

    Q_PROPERTY(State state READ getState WRITE setState)
    Q_PROPERTY(Style style READ getStyle WRITE setStyle)
    Q_PROPERTY(VpColor color READ getColor WRITE setColor)
    Q_PROPERTY(int xSpacing READ getXSpacing WRITE setXSpacing)
    Q_PROPERTY(int ySpacing READ getYSpacing WRITE setYSpacing)
    Q_PROPERTY(int multiplier READ getMultiplier WRITE setMultiplier)
    Q_PROPERTY(int xAlignment READ getXAlignment WRITE setXAlignment)
    Q_PROPERTY(int yAlignment READ getYAlignment WRITE setYAlignment)
    Q_PROPERTY(int xResolution READ getXResolution WRITE setXResolution)
    Q_PROPERTY(int yResolution READ getYResolution WRITE setYResolution)
    Q_PROPERTY(RefState referenceState READ getReferenceState WRITE setReferenceState)
    Q_PROPERTY(RefStyle referenceStyle READ getReferenceStyle WRITE setReferenceStyle)
    Q_PROPERTY(VpColor referenceColor READ getReferenceColor WRITE setReferenceColor)

  public:

    // Grid visibility states.
    enum State { STATE_UNKNOWN, STATE_OFF, STATE_ON, STATE_HIDDEN };
    // Grid styles.
    enum Style { STYLE_UNKNOWN, STYLE_LINE, STYLE_DOT, STYLE_CROSS };
    // Grid reference states.
    enum RefState { REFSTATE_UNKNOWN, REFSTATE_OFF, REFSTATE_ON };
    // Reference tyles.
    enum RefStyle { REFSTYLE_UNKNOWN, REFSTYLE_SQUARE, REFSTYLE_CIRCLE, REFSTYLE_CROSS};

    explicit VpGrid(QObject *parent = 0);
    virtual ~VpGrid();

    // Get/set property accessors.
    State getState() { return m_state; }
    void setState(State value) { m_state = value; }
    Style getStyle() { return m_style; }
    void setStyle(Style value) { m_style = value; }
    VpColor &getColor() { return m_color; }
    void setColor(const VpColor &value) { m_color = value; }
    int  getXSpacing() { return m_xSpacing; }
    void setXSpacing(int value) { m_xSpacing = value; }
    int  getYSpacing() { return m_ySpacing; }
    void setYSpacing(int value) { m_ySpacing = value; }
    int  getMultiplier() { return m_multiplier; }
    void setMultiplier(int value) { m_multiplier = value; }
    int  getXAlignment() { return m_xAlignment; }
    void setXAlignment(int value) { m_xAlignment = value; }
    int  getYAlignment() { return m_yAlignment; }
    void setYAlignment(int value) { m_yAlignment = value; }
    int  getXResolution() { return m_xResolution; }
    void setXResolution(int value) { m_xResolution = value; }
    int  getYResolution() { return m_yResolution; }
    void setYResolution(int value) { m_yResolution = value; }
    RefState getReferenceState() { return m_referenceState; }
    void setReferenceState(RefState value) { m_referenceState = value; }
    RefStyle getReferenceStyle() { return m_referenceStyle; }
    void setReferenceStyle(RefStyle value) { m_referenceStyle = value; }
    VpColor &getReferenceColor() { return m_referenceColor; }
    void setReferenceColor(const VpColor &value) { m_referenceColor = value; }

    /**
     * Snap the specified coordinate to a grid location.
     *
     * @param x The x component of the coordinate to snap.
     * @param y The y component of the coordinate to snap.
     *
     * @return <b>true</b> will be returned if the specified
     * coordinate is successfully snapped to the nearest grid point.
     * Otherwise <b>false</b> will be returned.
     */
    bool snapToGrid(int *x, int *y);

    /**
     * Get a coordinate based on the spacing and multiplier state
     * of the grid.
     *
     * @param x The x component of the grid coordinate.
     * @param y The y component of the grid coordinate.
      *
     * @return <b>true</b> will be returned if the coordinate
     * is successfully retrieved.
     * Otherwise <b>false</b> will be returned.
     */
    bool getGridCoord(int *x, int *y);

    /**
     * Determine if the grid reference marker is on.
     *
     * @return If the grid reference marker is on, then <b>true</b> will
     * be returned. Otherwise, <b>false</b> will be returned.
     */
    bool isReferenceOn();

    /**
     * Draw the grid with the specified context.
     *
     * @param gridGC The grid context.
     */
    void draw(GridGC &gridGC);

    /**
     * @brief Draw the grid reference.
     *
     * @param gridgc The grid context.
     */
    void drawReference(GridGC &gridGC);

    static int getGridXResolution()
    { return g_gridXResolution; }
    static void setGridXResolution(int resolution)
    { g_gridXResolution = resolution; }
    static int getGridYResolution()
    { return g_gridYResolution; }
    static void setGridYResolution(int resolution)
    { g_gridYResolution = resolution; }

    static int getGridUnit()
    { return (1 * VpCoord::getResolution()); }

    void reset() { init(); }

  signals:
    
  public slots:

  protected:

    // Global minimal resolution between horizontal grid points (in pixels).
    static int g_gridXResolution;
    // Global minimal resolution between vertical grid points (in pixels).
    static int g_gridYResolution;

    /**
     * Initialize the grid to default settings.
     *
     * @return If the grid is successfully initialized,  then <b>true</b> will
     * be returned. Otherwise, <b>false</b> will be returned.
     */
    bool init();

    /**
     * Draw the grid using lines.
     *
     * @param gridGC The grid context.
     */
    void drawLineGrid(GridGC &gridGC);
    
    /**
     * Draw the grid using dots.
     * <p>
     * The algorithm used to draw the grid entails drawing each dot individually.
     * </p>
     *
     * @param gridGC The grid context.
     */
    void drawDotGrid(GridGC &gridGC);

    /**
     * Draw the grid using crosses.
     * <p>
     * The algorithm used to draw the grid entails drawing each cross individually.
     * </p>
     *
     * @param gridGC The grid context.
     */
    void drawCrossGrid(GridGC &gridGC);

  private:

    State   m_state;
    Style   m_style;
    VpColor m_color;
    int     m_xSpacing;
    int     m_ySpacing;
    int     m_multiplier;
    int     m_xAlignment;
    int     m_yAlignment;
    int     m_xResolution;
    int     m_yResolution;

    RefState m_referenceState;
    RefStyle m_referenceStyle;
    VpColor  m_referenceColor;

};

struct GridState
{
    double           m_xSpacing;
    double           m_ySpacing;
    double           m_zSpacing;
    int              m_multiplier;
    VpGrid::State    m_state;
    VpGrid::Style    m_style;
    VpColor          m_color;
    VpGrid::RefState m_referenceState;
    VpGrid::RefStyle m_referenceStyle;
    VpColor          m_referenceColor;
    VpCoord          m_alignment;
};

#endif // __VPGRID_H_
