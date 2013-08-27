// COPYRIGHT_BEGIN
// COPYRIGHT_END

#ifndef __VPUTIL_H_
#define __VPUTIL_H_

class VpUtil
{
  public:

    VpUtil();

    /*
     * Round off a floating-point value;
     *
     * @param value The value to round off.
     *
     * @return A value will be returned, rounded to the nearest
     * integer.
     */
    static int round(double value);
};

#endif // __VPUTIL_H_
