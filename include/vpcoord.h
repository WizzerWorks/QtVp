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

#ifndef __VPCOORD_H_
#define __VPCOORD_H_

// Include QtVp header files.
#include "vptypes.h"

class QString;

class VpCoord
{
  public:

    // Most XY points to one command. XXX - move to another class or get rid of it.
    static const int MAX_COORDS = 256;

    /**
     * A constructor that specifies the (x,y,z) components of a
     * 3-dimensional coordinate.
     *
     * @param x The x component of the coordinate.
     * @param y The y component of the coordinate.
     * @param z The z component of the coordinate.
     */
    explicit VpCoord(int x = 0, int y = 0, int z = 0);

    /**
     * @brief Copy constructor
     *
     * @param coord The <code>AuCoord</code> to copy.
     */
    VpCoord(const VpCoord &coord);

    /**
     * @brief The destructor.
     */
    virtual ~VpCoord();

    // Accessors for setting the global resolution of the device.
    static int getResolution() { return g_resolution; }
    static void setResolution(int value) { g_resolution = value; }

    /**
     * Get the x component of the coordinate.
     *
     * @return An integer is returned representing the value of the x component
     * in world coordinate space.
     */
    int getX() { return m_x; }

    /**
     * Set the x component of the coordinate.
     *
     * @param value An integer representing the value of the x component
     * in world coordinate space.
     */
    void setX(const int value) { m_x = value; }

    /**
     * Get the y component of the coordinate.
     *
     * @return An integer is returned representing the value of the y component
     * in world coordinate space.
     */
    int getY() { return m_y; }

    /**
     * Set the y component of the coordinate.
     *
     * @param value An integer representing the value of the y component
     * in world coordinate space.
     */
    void setY(const int value) { m_y = value; }

    /**
     * Get the z component of the coordinate.
     *
     * @return An integer is returned representing the value of the z component
     * in world coordinate space.
     */
    int getZ() { return m_z; }

    /**
     * Set the z component of the coordinate.
     *
     * @param value An integer representing the value of the z component
     * in world coordinate space.
     */
    void setZ(const int value) { m_z = value; }

    /**
     * Formats the coordinate as (x,y,z,vpname).
     *
     * @return A String is returned, formatting the contents of the VpCoord
     * suitable for display.
     */
    const QString toString();

    /**
     * @brief Assigns a copy of AuCoord to this coord, and returns a reference to it.
     *
     * @param coord The <code>AuCoord</code> to assign.
     *
     * @return A reference to <b>this</b> coord is returned.
     */
    VpCoord & operator=(const VpCoord & coord);

    /**
     * @brief Equality operator.
     *
     * @param coord The <code>AuCoord</code> to test for equality.
     *
     * @return Returns <b>true</b> if this coord has the same coordinate values as <b>coord</b>;
     * otherwise returns <b>false</b>.
     */
    bool operator==(const VpCoord & coord) const;

    /**
     * @brief Inequality operator.
     *
     * @param coord The <code>AuCoord</code> to test for inequality.
     *
     * @return Returns <b>true</b> if this coord has different coordinate values from <b>coord</b>;
     * otherwise returns <b>false</b>.
     */
    bool operator!=(const VpCoord & coord) const;

    /**
     * A method for converting internal coordinates (stored)
     * to world coordinates.
     *
     * @param x The internal coordinate to convert.
     *
     * @return The world coordinate is returned.
     */
    static double internalToWorld(const int x);

    /**
     * A method for converting world coordinates to internal
     * coordinates (stored).
     *
     * @param x The world coordinate to convert.
     *
     * @return The internal coordinate is returned.
     */
    static int worldToInternal(const double x);

  protected:

    /** The x component of the coordinate. Defaults to <b>0</b>. */
    int m_x;
    /** The y component of the coordinate. Defaults to <b>0</b>. */
    int m_y;
    /** The z component of the coordinate. Defaults to <b>0</b>. */
    int m_z;
    /** The name of the associated <code>Viewport</code>. */
    QString m_vpName;

  private:

    // The resolution of a viewport.
    static int g_resolution;

};

#endif // __VPCOORD_H_
