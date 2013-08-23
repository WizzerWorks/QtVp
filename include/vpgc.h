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

#ifndef __VPGC_H_
#define __VPGC_H_

class QPaintDevice;
class VpViewport;

class VpGC
{
  public:

    /**
     * @brief Default constructor.
     */
    VpGC();

    /**
     * @brief Destructor.
     */
    virtual ~VpGC();

    /**
     * Get the <code>VpViewport</code> associated with this graphics context.
     *
     * @return The associated <code>VpViewport</code> is returned.
     */
    const VpViewport *getViewport() { return m_viewport; }

    /**
     * Set the <code>VpViewport</code> to be associated with this graphics context.
     *
     * @param The <code>VpViewport</code> to be associated with this graphics context
     * is returned.
     */
    void setViewport(const VpViewport *vp) { m_viewport = vp; }

    /**
     * @brief Get the graphics context.
     *
     * @return A pointer to the <code>QPaintDevice</code> is returned. May be <b>null</b>.
     */
    QPaintDevice *getGC() { return m_gc; }

    /**
     * @brief Set the graphics context.
     *
     * @param gc A pointer to a <code>QPaintDevice</code>. May be <b>null</b>.
     */
    void setGC(QPaintDevice *gc) { m_gc = gc; }

  protected:

    /** The <code>AuViewport</code> associated with this graphics context. */
    const VpViewport *m_viewport;

    /** The Qt painter context associated with this viewport. */
    QPaintDevice *m_gc;
};

#endif // __VPGC_H_
