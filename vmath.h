/* -*- C++ -*- */
/** @page license License
 * vmath, set of classes for computer graphics mathematics.
 * Copyright (c) 2005-2011, Jan Bartipan < barzto at gmail dot com >
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * - Neither the names of its contributors may be used to endorse or
 *   promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @mainpage Intro
 * Vector mathematics for computer graphics - VoxelStorm version
 *
 * @section Features
 * <ul>
 *    <li> basic arithmetic operations - using operators </li>
 *    <li> basic linear algebra operations - such as transpose, dot product, etc. </li>
 *    <li> aliases for vertex coordinates - it means:
 *    <pre>
 *  Vector3f v;
 *  // use vertex coordinates
 *  v.x = 1; v.y = 2; v.z = -1;
 *
 *  // use texture coordinates
 *  v.s = 0; v.t = 1; v.u = 0.5;
 *  // use color coordinates
 *  v.r = 1; v.g = 0.5; v.b = 0;
 *    </pre>
 *    </li>
 *    <li> conversion constructor and assign operators - so you can assign a value of Vector3&lt;T1&gt; type
 *    to a variable of Vector3&lt;T2&gt; type for any convertible T1, T2 type pairs. In other words, you can do this:
 *    <pre>
 *
 *  Vector3f f3; Vector3d d3 = f3;
 *  ...
 *  f3 = d3;
 *    </pre>
 *    </li>
 * </ul>
 *
 * @section Predefined types
 * <ul>
 *  <li>(Vector2) Two dimensional vector
 *    <ul>
 *      <li>float &mdash; Vector2f</li>
 *      <li>double &mdash; Vector2d</li>
 *      <li>int &mdash; Vector2i</li>
 *    </ul>
 *  </li>
 *  <li>(Vector3) Three dimensional vector
 *    <ul>
 *      <li>float &mdash; Vector3f</li>
 *      <li>double &mdash; Vector3d</li>
 *      <li>int &mdash; Vector3i</li>
 *    </ul>
 *  </li>
 *  <li>(Vector4) Four dimensional vector
 *    <ul>
 *      <li>float &mdash; Vector4f</li>
 *      <li>double &mdash; Vector4d</li>
 *      <li>int &mdash; Vector4i</li>
 *    </ul>
 *  </li>
 *  <li>(Matrix3) Matrix 3x3
 *    <ul>
 *      <li>float &mdash; Matrix3f</li>
 *      <li>double &mdash; Matrix3d</li>
 *      <li>int &mdash; Matrix3i</li>
 *    </ul>
 *  </li>
 *  <li>(Matrix4) Matrix 4x4
 *    <ul>
 *      <li>float &mdash; Matrix4f</li>
 *      <li>double &mdash; Matrix4d</li>
 *      <li>int &mdash; Matrix4i</li>
 *    </ul>
 *  </li>
  *   <li> Quaternion
 *    <ul>
 *      <li>float &mdash; Quatf</li>
 *      <li>double &mdash; Quatd</li>
 *    </ul>
 *    </li>
 *  </li>
 * </ul>
 */

// Modified 2011-06-12, Davide Bacchet (davide.bacchet at gmail dot com)
//                      added () operators with standard matrix notation (opposite wrt at() ).

// Modified 2013-2015, Eugene Hopkinson for VoxelStorm Ltd
//                     various expansions and conversions
//                     Version 2.0: C++11 specific optimisations, including constexpr
//                     Version 2.1: C++14 optimisations, intersection algorithms

#ifndef __vmath_Header_File__
#define __vmath_Header_File__

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <array>

#ifdef VMATH_NAMESPACE
namespace VMATH_NAMESPACE {
#endif

#ifndef VMATH_NO_BOOST
#include <boost/math/constants/constants.hpp>
// use boost's constants if available
#else  // VMATH_NO_BOOST
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif // M_PI
#endif // VMATH_NO_BOOST

// note: use VMATH_SOFT_COMPARE to enable fuzzy matching for different types,
// with epsilon used to match within a range of rounding error - but doing this
// can be very expensive in some cases.
double constexpr epsilon = 4.37114e-05;
#define EPSILON epsilon
#define DEG2RAD deg2rad
#define RAD2DEG rad2deg

template<class T>
inline static T constexpr const deg2rad(T const angle_deg) {
  #ifndef VMATH_NO_BOOST
    //return (angle_deg * boost::math::constants::pi<T>()) / 180.0;
    return angle_deg * boost::math::constants::degree<T>();
  #else  // VMATH_NO_BOOST
    return (angle_deg * M_PI) / 180.0;
  #endif // VMATH_NO_BOOST
}

template<class T>
inline static T constexpr const rad2deg(T const angle_rad) {
  #ifndef VMATH_NO_BOOST
    return angle_rad * boost::math::constants::radian<T>();
  #else  // VMATH_NO_BOOST
    return (angle_rad * 180.0) / M_PI;
  #endif // VMATH_NO_BOOST
}

template<class T> class Vector2;  // forward declarations
template<class T> class Vector3;
template<class T> class Vector4;
template<class T> class Matrix3;
template<class T> class Matrix4;
template<class T> class Quaternion;
template<class T> class Aabb2;
template<class T> class Aabb3;

/**
 * Class for two dimensional vector.
 * There are three ways of accessing vector components.
 * Let's have <code>Vector2f v</code>, you can either:
 * <ul>
 *  <li>access as position(x,y) &mdash; <code>v.x = v.y = 3;</code></li>
 *  <li>access as texture coordinate (s,t) &mdash; <code>v.s = v.t = 3;</code></li>
 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = 3;</code></li>
 * </ul>
 */
template<class T>
class Vector2 {
  public:
    union {
      /**
       * First element of vector, alias for X-coordinate.
       */
      T x;

      /**
       * First element of vector, alias for S-coordinate.
       * For textures notation.
       */
      T s;
    };

    union {
      /**
       * Second element of vector, alias for Y-coordinate.
       */
      T y;

      /**
       * Second element of vector, alias for Z-coordinate (for x-z 2D grids).
       */
      T z;

      /**
       * Second element of vector, alias for T-coordinate.
       * For textures notation.
       */
      T t;
    };

    //----------------[ constructors ]--------------------------
    /**
     * Creates and sets to (0,0)
     */
    inline constexpr Vector2() __attribute__((__always_inline__))
      : x(0), y(0) {
    }

    /**
     * Creates and sets to (x,y)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     */
    inline constexpr Vector2(T nx, T ny) __attribute__((__always_inline__))
      : x(nx), y(ny) {
    }

    /**
     * Copy constructor.
     * @param src Source of data for new created instance.
     */
    inline constexpr Vector2(Vector2<T> const &src) __attribute__((__always_inline__))
      : x(src.x), y(src.y) {
    }

    /**
     * Copy casting constructor.
     * @param src Source of data for new created instance.
     */
    template<class FromT>
    inline constexpr Vector2(Vector2<FromT> const &src)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)) {
    }

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (x,y)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     */
    inline void assign(T nx = 0, T ny = 0) __attribute__((__always_inline__)) {
      x = nx;
      y = ny;
    }

    //----------------[ access operators ]-------------------
    /**
     * Copy casting operator
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Vector2<T> &operator=(Vector2<FromT> const &rhs) {
      x = static_cast<T>(rhs.x);
      y = static_cast<T>(rhs.y);
      return *this;
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x = rhs.x;
      y = rhs.y;
      return *this;
    }

    /**
     * Array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, else reference to y
     * y coordinate.
     */
    inline T &operator[](int n) __attribute__((__always_inline__)) {
      return n == 0 ? x : y;
    }

    /**
     * Constant array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, else reference to y
     * y coordinate.
     */
    inline T constexpr const &operator[](int n) const __attribute__((__always_inline__)) {
      return n == 0 ? x : y;
    }

    //---------------[ vector aritmetic operator ]--------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator+(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x + rhs.x, y + rhs.y);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator-(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x - rhs.x, y - rhs.y);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator*(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x * rhs.x, y * rhs.y);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator/(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x / rhs.x, y / rhs.y);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator+=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }

    /**
     * Substraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator-=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x -= rhs.x;
      y -= rhs.y;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator*=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x *= rhs.x;
      y *= rhs.y;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator/=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
      return *this;
    }

    /**
     * Dot product of two vectors.
     * @param rhs Right hand side argument of binary operator.
     */
    inline T constexpr dotProduct(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return x * rhs.x + y * rhs.y;
    }

    /**
     * Cross product of two vectors
     * @param rhs Right hand side argument of binary operator.
     */
    inline T constexpr crossProduct(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return x * rhs.y - y * rhs.x;
    }

    //--------------[ scalar vector operator ]--------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x + rhs, y + rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x - rhs, y - rhs);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x * rhs, y * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x / rhs, y / rhs);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator+=(T rhs) __attribute__((__always_inline__)) {
      x += rhs;
      y += rhs;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator-=(T rhs) __attribute__((__always_inline__)) {
      x -= rhs;
      y -= rhs;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator*=(T rhs) __attribute__((__always_inline__)) {
      x *= rhs;
      y *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator/=(T rhs) __attribute__((__always_inline__)) {
      x /= rhs;
      y /= rhs;
      return *this;
    }

    //--------------[ equality operator ]------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold epsilon value. To be two
     * values equal, must satisfy this condition | lhs.x - rhs.y | < epsilon,
     * same for y-coordinate.
     */
    inline bool constexpr operator==(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      #ifdef VMATH_SOFT_COMPARE
        return (std::abs(x - rhs.x) < static_cast<T>(epsilon)) &&
               (std::abs(y - rhs.y) < static_cast<T>(epsilon));
      #else
        return x == rhs.x &&
               y == rhs.y;
      #endif
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated vector
     */
    inline Vector2<T> constexpr operator-() const __attribute__((__always_inline__)) {
      return Vector2<T>(-x, -y);
    }

    //-------------[ size operations ]---------------------------
    /**
     * Get length of vector.
     * @return length of vector
     */
    inline T constexpr length() const __attribute__((__always_inline__)) {
      return static_cast<T>(std::sqrt(x * x + y * y));
    }

    /**
     * Normalize vector
     */
    inline void normalize() __attribute__((__always_inline__)) {
      T const temp = length();
      x /= temp;
      y /= temp;
    }
    inline void normalise() {  // proper english
      normalize();
    }
    inline Vector2<T> constexpr normalize_copy() const __attribute__((__always_inline__)) {
      return Vector2<T>(x / length(), y / length());
    }
    inline Vector2<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {  // proper english
      return normalize_copy();
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two vector can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
      return x * x + y * y;
    }

    //-------------[ other operations ]--------------------------
    /**
     * Rotate 2D vector clockwise.
     * @param aa Angle (in degrees) to be rotated.
     */
    inline void rotate(T aa) __attribute__((__always_inline__)) {
      rotate_rad(deg2rad(aa));
    }

    /**
     * Rotate 2D vector clockwise, radian version.
     * @param aa Angle (in radians) to be rotated.
     */
    inline void rotate_rad(T aa) __attribute__((__always_inline__)) {
      T const a = std::cos(-aa);
      T const b = std::sin(-aa);
      T const nx = x * a - y * b;
      T const ny = x * b + y * a;
      x = nx;
      y = ny;
    }

    /**
     * Linear interpolation of two vectors
     * @param fact Factor of interpolation. For translation from position
     * of this vector to vector r, values of factor goes from 0.0 to 1.0.
     * @param r Second Vector for interpolation
     * @note However values of fact parameter are reasonable only in interval
     * [0.0 , 1.0], you can pass also values outside of this interval and you
     * can get result (extrapolation?)
     */
    inline Vector2<T> constexpr lerp(T fact, Vector2<T> const &r) const __attribute__((__always_inline__)) {
      return (*this) + (r - (*this)) * fact;
    }

    //-------------[ conversion ]-----------------------------
    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector2<T>)
     * used for passing Vector2<T> values to gl*2[fd] functions.
     */
    inline operator T*() __attribute__((__always_inline__)) {
      return reinterpret_cast<T*>(this);
    }
    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Vector2<T>)
     * used for passing Vector2<T> values to gl*2[fd] functions.
     */
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
      return reinterpret_cast<T const*>(this);
    }

    //-------------[ output operator ]------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      lhs << "[" << rhs.x << "," << rhs.y << "]";
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline constexpr std::string toString() const __attribute__((__always_inline__)) {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

    /**
     * Check whether two lines intersect, and if so obtain the intersection point
     * and store it in this vector.
     * @param line1start The start coordinates of the first line
     * @param line1end The end coordinates of the first line
     * @param line2start The start coordinates of the first line
     * @param line2end The end coordinates of the first line
     */
    template<class FromT>
    inline bool get_line_intersection(Vector2<FromT> const &line1start,
                                      Vector2<FromT> const &line1end,
                                      Vector2<FromT> const &line2start,
                                      Vector2<FromT> const &line2end) {
      Vector2<T> const temp_s1(line1end.x - line1start.x,
                          line1end.y - line1start.y);
      Vector2<T> const temp_s2(line2end.x - line2start.x,
                          line2end.y - line2start.y);
      T const temp_s = (-temp_s1.y * (line1start.x - line2start.x) + temp_s1.x * (line1start.y - line2start.y)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
      T const temp_t = ( temp_s2.x * (line1start.y - line2start.y) - temp_s2.y * (line1start.x - line2start.x)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
      if(temp_s >= 0 && temp_s <= 1 && temp_t >= 0 && temp_t <= 1) {
        x = line1start.x + (temp_t * temp_s1.x);
        y = line1start.y + (temp_t * temp_s1.y);
        return true;
      }
      return false;
    }

    /**
     * Free function to check whether two lines intersect.
     * @param line1start The start coordinates of the first line
     * @param line1end The end coordinates of the first line
     * @param line2start The start coordinates of the first line
     * @param line2end The end coordinates of the first line
     */
    template<class FromT>
    inline static bool constexpr do_lines_intersect(Vector2<FromT> const &line1start,
                                                    Vector2<FromT> const &line1end,
                                                    Vector2<FromT> const &line2start,
                                                    Vector2<FromT> const &line2end) {
      Vector2<T> constexpr temp_s1(line1end.x - line1start.x,
                              line1end.y - line1start.y);
      Vector2<T> constexpr temp_s2(line2end.x - line2start.x,
                              line2end.y - line2start.y);
      T constexpr temp_s = (-temp_s1.y * (line1start.x - line2start.x) + temp_s1.x * (line1start.y - line2start.y)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
      T constexpr temp_t = ( temp_s2.x * (line1start.y - line2start.y) - temp_s2.y * (line1start.x - line2start.x)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
      if(temp_s >= 0 && temp_s <= 1 && temp_t >= 0 && temp_t <= 1) {
        return true;
      }
      return false;
    }
};

//--------------------------------------
// Typedef shortcuts for 2D vector
//-------------------------------------
/// Two dimensional Vector of floats
using Vector2f = Vector2<float>;
/// Two dimensional Vector of doubles
using Vector2d = Vector2<double>;
/// Two dimensional Vector of long doubles
using Vector2ld = Vector2<long double>;
/// Two dimensional Vector of ints
using Vector2i = Vector2<int>;

/**
 * Class for three dimensional vector.
 * There are four ways of accessing vector components.
 * Let's have <code>Vector3f v</code>, you can either:
 * <ul>
 *  <li>access as position (x,y,z) &mdash; <code>v.x = v.y = v.z = 1;</code></li>
 *  <li>access as texture coordinate (s,t,u) &mdash; <code>v.s = v.t = v.u = 1;</code></li>
 *  <li>access as color (r,g,b) &mdash; <code>v.r = v.g = v.b = 1;</code></li>
 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = 1;</code></li>
 * </ul>
 */
template<class T>
class Vector3 {
  public:
    //T x, y, z;
    union {
      /**
       * First element of vector, alias for X-coordinate.
       */
      T x;

      /**
       * First element of vector, alias for S-coordinate.
       * For textures notation.
       */
      T s;

      /**
       * First element of vector, alias for R-coordinate.
       * For color notation.
       */
      T r;
    };

    union {
      /**
       * Second element of vector, alias for Y-coordinate.
       */
      T y;
      /**
       * Second element of vector, alias for T-coordinate.
       * For textures notation.
       */
      T t;
      /**
       * Second element of vector, alias for G-coordinate.
       * For color notation.
       */
      T g;
    };

    union {
      /**
       * Third element of vector, alias for Z-coordinate.
       */
      T z;

      /**
       * Third element of vector, alias for U-coordinate.
       * For textures notation.
       */
      T u;
      /**
       * Third element of vector, alias for B-coordinate.
       * For color notation.
       */
      T b;
    };

    //----------------[ constructors ]--------------------------
    /**
     * Creates and sets to (0,0,0)
     */
    inline constexpr Vector3() __attribute__((__always_inline__))
      : x(0), y(0), z(0) {
    }

    /**
     * Creates and sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     */
    inline constexpr Vector3(T nx, T ny, T nz) __attribute__((__always_inline__))
      : x(nx), y(ny), z(nz) {
    }

    /**
     * Copy constructor.
     * @param src Source of data for new created Vector3 instance.
     */
    inline constexpr Vector3(Vector3<T> const &src) __attribute__((__always_inline__))
      : x(src.x), y(src.y), z(src.z) {
    }

    /**
     * Copy casting constructor.
     * @param src Source of data for new created Vector3 instance.
     */
    template<class FromT>
    inline constexpr Vector3(Vector3<FromT> const &src)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)) {
    }

    inline constexpr Vector3(Vector2<T> const &src, T z)
      : x(src.x), y(src.y), z(z) {
    }

    template<class FromT>
    inline constexpr Vector3(Vector2<FromT> const &src, FromT z)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(z)) {
    }

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     */
    inline void assign(T nx = 0, T ny = 0, T nz = 0) __attribute__((__always_inline__)) {
      x = nx;
      y = ny;
      z = nz;
    }

    //----------------[ access operators ]-------------------
    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> operator=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
      return *this;
    }

    /**
     * Copy casting operator.
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Vector3<T> operator=(Vector3<FromT> const &rhs) {
      x = static_cast<T>(rhs.x);
      y = static_cast<T>(rhs.y);
      z = static_cast<T>(rhs.z);
      return *this;
    }

    /**
     * Copy operator to Vector3
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> operator=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
      return *this;
    }

    /**
     * Copy casting operator to Vector3
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Vector3<T> operator=(Vector4<FromT> const &rhs) {
      x = static_cast<T>(rhs.x);
      y = static_cast<T>(rhs.y);
      z = static_cast<T>(rhs.z);
      return *this;
    }

    /**
     * Array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, n = 1
     * reference to y, else reference to z
     * y coordinate.
     */
    inline T &operator[](int n) __attribute__((__always_inline__)) {
      return n == 0 ? x : (n == 1 ? y : z);
    }

    /**
     * Constant array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, n = 1
     * reference to y, else reference to z
     * y coordinate.
     */
    inline T constexpr const &operator[](int n) const __attribute__((__always_inline__)) {
      return n == 0 ? x : (n == 1 ? y : z);
    }

    //---------------[ vector arithmetic operator ]--------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator+(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator-(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator/(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator+=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator-=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator*=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator/=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
      z /= rhs.z;
      return *this;
    }

    /**
     * Dot product of two vectors.
     * @param rhs Right hand side argument of binary operator.
     */
    inline T constexpr dotProduct(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    /**
     * Cross product of two vectors
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr crossProduct(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
    }

    //--------------[ rotation with quaternions ]-----------------
    // see http://mollyrocket.com/forums/viewtopic.php?t=833&sid=3a84e00a70ccb046cfc87ac39881a3d0
    /**
     * Multiplication by quaternion operator (rotation by quaternion)
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
      return *this + (rhs.v.crossProduct(*this) * 2 * rhs.w) + rhs.v.crossProduct(rhs.v.crossProduct(*this) * 2);
    }

    /**
     * Multiplication by quaternion operator (rotation by quaternion)
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator*=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
      Vector3<T> const temp = rhs.v.crossProduct(*this) * 2;
      *this += (temp * rhs.w) + rhs.v.crossProduct(temp);
      return *this;
    }

    //--------------[ scalar vector operator ]--------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x + rhs, y + rhs, z + rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x - rhs, y - rhs, z - rhs);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x * rhs, y * rhs, z * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x / rhs, y / rhs, z / rhs);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator+=(T rhs) __attribute__((__always_inline__)) {
      x += rhs;
      y += rhs;
      z += rhs;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator-=(T rhs) __attribute__((__always_inline__)) {
      x -= rhs;
      y -= rhs;
      z -= rhs;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator*=(T rhs) __attribute__((__always_inline__)) {
      x *= rhs;
      y *= rhs;
      z *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator/=(T rhs) __attribute__((__always_inline__)) {
      x /= rhs;
      y /= rhs;
      z /= rhs;
      return *this;
    }

    //-------------[ equality operator ]-------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold epsilon value. To be two
     * values equal, must satisfy this condition | lhs.x - rhs.y | < epsilon,
     * same for y-coordinate, and z-coordinate.
     */
    inline bool constexpr operator==(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      #ifdef VMATH_SOFT_COMPARE
        return std::fabs(x - rhs.x) < static_cast<T>(epsilon) &&
               std::fabs(y - rhs.y) < static_cast<T>(epsilon) &&
               std::fabs(z - rhs.z) < static_cast<T>(epsilon);
      #else
        return x == rhs.x &&
               y == rhs.y &&
               z == rhs.z;
      #endif // VMATH_SOFT_COMPARE
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated vector
     */
    inline Vector3<T> constexpr operator-() const __attribute__((__always_inline__)) {
      return Vector3<T>(-x, -y, -z);
    }

    //-------------[ size operations ]---------------------------
    /**
     * Get length of vector.
     * @return length of vector
     */
    inline T constexpr length() const __attribute__((__always_inline__)) {
      return static_cast<T>(std::sqrt(x * x + y * y + z * z));
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two vector can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
      return x * x + y * y + z * z;
    }

    /**
     * Normalize vector
     */
    inline void normalize() __attribute__((__always_inline__)) {
      T const temp = length();
      x /= temp;
      y /= temp;
      z /= temp;
    }
    inline void normalise() __attribute__((__always_inline__)) {  // proper english
      normalize();
    }
    inline Vector3<T> constexpr normalize_copy() const __attribute__((__always_inline__)) {
      return Vector3<T>(x / length(), y / length(), z / length());
    }
    inline Vector3<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {  // proper english
      return normalize_copy();
    }

    //-------------[ other operations ]--------------------------
    /**
     * Rotate vector around three axis.
     * @param ax Angle (in degrees) to be rotated around X-axis.
     * @param ay Angle (in degrees) to be rotated around Y-axis.
     * @param az Angle (in degrees) to be rotated around Z-axis.
     */
    inline void rotate(T ax, T ay, T az) __attribute__((__always_inline__)) {
      rotate_rad(deg2rad(ax), deg2rad(ay), deg2rad(az));
    }

    /**
     * Rotate vector around three axis, radian version.
     * @param ax Angle (in radians) to be rotated around X-axis.
     * @param ay Angle (in radians) to be rotated around Y-axis.
     * @param az Angle (in radians) to be rotated around Z-axis.
     */
    inline void rotate_rad(T ax, T ay, T az) __attribute__((__always_inline__)) {
      T const a = std::cos(ax);
      T const b = std::sin(ax);
      T const c = std::cos(ay);
      T const d = std::sin(ay);
      T const e = std::cos(az);
      T const f = std::sin(az);
      T const nx = c * e * x - c * f * y + d * z;
      T const ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
      T const nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
      x = nx;
      y = ny;
      z = nz;
    }
    /**
     * Rotate vector by a quaternion.
     * @param ax Quaternion to rotate by.
     */
    inline void rotate(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
      *this *= rhs;
    }

    /**
     * Linear interpolation of two vectors
     * @param fact Factor of interpolation. For translation from positon
     * of this vector to vector r, values of factor goes from 0.0 to 1.0.
     * @param r Second Vector for interpolation
     * @note However values of fact parameter are reasonable only in interval
     * [0.0 , 1.0], you can pass also values outside of this interval and you
     * can get result (extrapolation?)
     */
    inline Vector3<T> constexpr lerp(T fact, Vector3<T> const &r) const __attribute__((__always_inline__)) {
      return (*this) + (r - (*this)) * fact;
    }

    //-------------[ conversion ]--------------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector3<T>)
     * used for passing Vector3<T> values to gl*3[fd] functions.
     */
    inline operator T*() __attribute__((__always_inline__)) {
      return reinterpret_cast<T*>(this);
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Vector3<T>)
     * used for passing Vector3<T> values to gl*3[fd] functions.
     */
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
      return reinterpret_cast<T const*>(this);
    }

    //-------------[ output ]------------------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "]";
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

    /**
     * Gets a 2D vector equivalent using the X and Y axes
     */
    inline Vector2<T> constexpr to_2D_XY() const __attribute__((__always_inline__)) {
      return Vector2<T>(x, y);
    }

    /**
     * Gets a 2D vector equivalent using the X and Z axes
     */
    inline Vector2<T> constexpr to_2D_XZ() const __attribute__((__always_inline__)) {
      return Vector2<T>(x, z);
    }
};

/// Three dimensional Vector of floats
using Vector3f = Vector3<float>;
/// Three dimensional Vector of doubles
using Vector3d = Vector3<double>;
/// Three dimensional Vector of long doubles
using Vector3ld = Vector3<long double>;
/// Three dimensional Vector of ints
using Vector3i = Vector3<int>;

/**
 * Class for four dimensional vector.
 * There are four ways of accessing vector components.
 * Let's have <code>Vector4f v</code>, you can either:
 * <ul>
 *  <li>access as position in projective space (x,y,z,w) &mdash; <code>v.x = v.y = v.z = v.w = 1;</code></li>
 *  <li>access as texture coordinate (s,t,u,v) &mdash; <code>v.s = v.t = v.u = v.v = 1;</code></li>
 *  <li>access as color (r,g,b,a) &mdash; <code>v.r = v.g = v.b = v.a = 1;</code></li>
 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = v[3] = 1;</code></li>
 * </ul>
 */
template<class T>
class Vector4 {
  public:

    union {
      /**
       * First element of vector, alias for R-coordinate.
       * For color notation.
       */
      T r
      /**
       * First element of vector, alias for X-coordinate.
       */;
      T x;
    };

    union {
      /**
       * Second element of vector, alias for G-coordinate.
       * For color notation.
       */
      T g;
      /**
       * Second element of vector, alias for Y-coordinate.
       */
      T y;
    };

    union {
      /**
       * Third element of vector, alias for B-coordinate.
       * For color notation.
       */
      T b;
      /**
       * Third element of vector, alias for Z-coordinate.
       */
      T z;
    };

    union {
      /**
       * Fourth element of vector, alias for A-coordinate.
       * For color notation. This represnt aplha chanell
       */
      T a;
      /**
       * First element of vector, alias for W-coordinate.
       * @note For vectors (such as normals) should be set to 0.0
       * For vertices should be set to 1.0
       */
      T w;
    };

    //----------------[ constructors ]--------------------------
    /**
     * Creates and sets to (0,0,0,0)
     */
    inline constexpr Vector4()
      : x(0), y(0), z(0), w(0) {
    }

    /**
     * Creates and sets to (x,y,z,z)
     * @param nx initial x-coordinate value (R)
     * @param ny initial y-coordinate value (G)
     * @param nz initial z-coordinate value (B)
     * @param nw initial w-coordinate value (Alpha)
     */
    inline constexpr Vector4(T nx, T ny, T nz, T nw) __attribute__((__always_inline__))
      : x(nx), y(ny), z(nz), w(nw) {
    }

    /**
     * Copy constructor.
     * @param src Source of data for new created Vector4 instance.
     */
    inline constexpr Vector4(Vector4<T> const &src) __attribute__((__always_inline__))
      : x(src.x), y(src.y), z(src.z), w(src.w) {
    }

    /**
     * Copy casting constructor.
     * @param src Source of data for new created Vector4 instance.
     */
    template<class FromT>
    inline constexpr Vector4(Vector4<FromT> const &src)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(src.w)) {
    }

    inline constexpr Vector4(Vector3<T> const &src, T w) __attribute__((__always_inline__))
      : x(src.x), y(src.y), z(src.z), w(w) {
    }

    template<class FromT>
    inline constexpr Vector4(Vector3<FromT> const &src, FromT w)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(w)) {
    }

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     * @param nz initial w-coordinate value
     */
    inline void assign(T nx = 0, T ny = 0, T nz = 0, T nw = 0) __attribute__((__always_inline__)) {
      x = nx;
      y = ny;
      z = nz;
      w = nw;
    }

    //----------------[ access operators ]-------------------
    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> operator=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
      w = rhs.w;
      return *this;
    }

    /**
     * Copy casting operator
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Vector4<T> operator=(Vector4<FromT> const &rhs) {
      x = static_cast<T>(rhs.x);
      y = static_cast<T>(rhs.y);
      z = static_cast<T>(rhs.z);
      w = static_cast<T>(rhs.w);
      return *this;
    }

    /**
     * Copy operator to Vector4
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> operator=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
      w = 0.0;
      return *this;
    }

    /**
     * Copy casting operator to Vector4
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Vector4<T> operator=(Vector3<FromT> const &rhs) {
      x = static_cast<T>(rhs.x);
      y = static_cast<T>(rhs.y);
      z = static_cast<T>(rhs.z);
      w = 0.0;
      return *this;
    }

    /**
     * Array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, n = 1
     * reference to y coordinate, n = 2 reference to z,
     * else reference to w coordinate.
     */
    inline T &operator[](int n) __attribute__((__always_inline__)) {
      return n == 0 ? x : (n == 1 ? y : (n == 2 ? z : w));
    }

    /**
     * Array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, n = 1
     * reference to y coordinate, n = 2 reference to z,
     * else reference to w coordinate.
     */
    inline T constexpr const &operator[](int n) const __attribute__((__always_inline__)) {
      return n == 0 ? x : (n == 1 ? y : (n == 2 ? z : w));
    }

    //---------------[ vector aritmetic operator ]--------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator+(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator-(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator/(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator+=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
      w += rhs.w;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator-=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;
      w -= rhs.w;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator*=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
      w *= rhs.w;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator/=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
      z /= rhs.z;
      w /= rhs.w;
      return *this;
    }

    //--------------[ equiality operator ]------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold epsilon value. To be two
     * values equal, must satisfy this condition | lhs.x - rhs.y | < epsilon,
     * same for y-coordinate, z-coordinate, and w-coordinate.
     */
    inline bool constexpr operator==(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      #ifdef VMATH_SOFT_COMPARE
        return std::fabs(x - rhs.x) < static_cast<T>(epsilon) &&
               std::fabs(y - rhs.y) < static_cast<T>(epsilon) &&
               std::fabs(z - rhs.z) < static_cast<T>(epsilon) &&
               std::fabs(w - rhs.w) < static_cast<T>(epsilon);
      #else
        return x == rhs.x &&
               y == rhs.y &&
               z == rhs.z &&
               w == rhs.w;
      #endif // VMATH_SOFT_COMPARE
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated vector
     */
    inline Vector4<T> constexpr operator-() const __attribute__((__always_inline__)) {
      return Vector4<T>(-x, -y, -z, -w);
    }

    //--------------[ scalar vector operator ]--------------------

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator+=(T rhs) __attribute__((__always_inline__)) {
      x += rhs;
      y += rhs;
      z += rhs;
      w += rhs;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator-=(T rhs) __attribute__((__always_inline__)) {
      x -= rhs;
      y -= rhs;
      z -= rhs;
      w -= rhs;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator*=(T rhs) __attribute__((__always_inline__)) {
      x *= rhs;
      y *= rhs;
      z *= rhs;
      w *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator/=(T rhs) __attribute__((__always_inline__)) {
      x /= rhs;
      y /= rhs;
      z /= rhs;
      w /= rhs;
      return *this;
    }

    //-------------[ size operations ]---------------------------
    /**
     * Get length of vector.
     * @return length of vector
     */
    inline T constexpr length() const __attribute__((__always_inline__)) {
      return static_cast<T>(std::sqrt(x * x + y * y + z * z + w * w));
    }

    /**
     * Normalize vector
     */
    inline void normalize() __attribute__((__always_inline__)) {
      T const temp = length();
      x /= temp;
      y /= temp;
      z /= temp;
      w /= temp;
    }
    inline void normalise() __attribute__((__always_inline__)) {  // proper english
      normalize();
    }
    inline Vector4<T> constexpr normalize_copy() const __attribute__((__always_inline__)) {
      return Vector4<T>(x / length(), y / length(), z / length(), w / length());
    }
    inline Vector4<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {  // proper english
      return normalize_copy();
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two vector can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
      return x * x + y * y + z * z + w * w;
    }

    //--------------[ misc. operations ]-----------------------
    /**
     * Linear interpolation of two vectors
     * @param fact Factor of interpolation. For translation from position
     * of this vector to vector r, values of factor goes from 0.0 to 1.0.
     * @param r Second Vector for interpolation
     * @note However values of fact parameter are reasonable only in interval
     * [0.0 , 1.0], you can pass also values outside of this interval and you
     * can get result (extrapolation?)
     */
    inline Vector4<T> constexpr lerp(T fact, Vector4<T> const &r) const __attribute__((__always_inline__)) {
      return (*this) + (r - (*this)) * fact;
    }

    //-------------[ conversion ]-----------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector4<T>)
     * used for passing Vector4<T> values to gl*4[fd] functions.
     */
    inline operator T*() __attribute__((__always_inline__)) {
      return reinterpret_cast<T*>(this);
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Vector4<T>)
     * used for passing Vector4<T> values to gl*4[fd] functions.
     */
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
      return reinterpret_cast<T const*>(this);
    }

    /**
     * Gets 3D vector. Note that the output is divided by w coordinate to apply projection
     * transform. If the w coordinate is equal to zero, the result is not divided.
     * @return (x/w, y/w, z/w) iff w != 0 otherwise (x,y,z)
     */
    inline constexpr Vector3<T> xyz() const __attribute__((__always_inline__)) {
      return w == 0 ? Vector3<T>(x, y, z) : Vector3<T>(x / w, y / w, z / w);
    }

    //-------------[ output operator ]------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << "]";
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

};

/// Four dimensional Vector of floats
using Vector4f = Vector4<float>;
/// Four dimensional Vector of doubles
using Vector4d = Vector4<double>;
/// Four dimensional Vector of long doubles
using Vector4ld = Vector4<long double>;
/// Four dimensional Vector of ints
using Vector4i = Vector4<int>;

/**
 * Class for matrix 3x3.
 * @note Data stored in this matrix are in column major order. This arrangement suits OpenGL.
 * If you're using row major matrix, consider using fromRowMajorArray as way for construction
 * Matrix3<T> instance.
 */
template<class T>
class Matrix3 {
  public:
    /// Data stored in column major order
    std::array<T, 9> data;

    //--------------------------[ constructors ]-------------------------------
    /**
     * Creates identity matrix
     */
    inline constexpr Matrix3()
      : data{1, 0, 0,
             0, 1, 0,
             0, 0, 1} {
    }

    /**
     * Copy matrix values from array (these data must be in column
     * major order!)
     */
    inline constexpr Matrix3(T const *dt)
      : data{dt[0], dt[1], dt[2],
             dt[3], dt[4], dt[5],
             dt[6], dt[7], dt[8]} {
    }
    inline constexpr Matrix3(T *dt)
      : data{dt[0], dt[1], dt[2],
             dt[3], dt[4], dt[5],
             dt[6], dt[7], dt[8]} {
    }

    /**
     * Copy constructor.
     * @param src Data source for new created instance of Matrix3
     */
    inline constexpr Matrix3(Matrix3<T> const &src)
      : data{src.data[0], src.data[1], src.data[2],
             src.data[3], src.data[4], src.data[5],
             src.data[6], src.data[7], src.data[8]} {
    }

    /**
     * Copy casting constructor.
     * @param src Data source for new created instance of Matrix3
     */
    template<class FromT>
    inline constexpr Matrix3(Matrix3<FromT> const &src)
      : data{static_cast<T>(src.data[0]), static_cast<T>(src.data[1]), static_cast<T>(src.data[2]),
             static_cast<T>(src.data[3]), static_cast<T>(src.data[4]), static_cast<T>(src.data[5]),
             static_cast<T>(src.data[6]), static_cast<T>(src.data[7]), static_cast<T>(src.data[8])} {
    }

    /**
     * Variadic initialisation constructor
     * @param dt Initialiser list containing raw data for each element in order.
     */
    //inline constexpr Matrix3(std::initializer_list<T> dt)
    //  : data(dt) __attribute__((__always_inline__)) {
    //}
    // see http://stackoverflow.com/a/5549918/1678468
    template<class... FromT>
    inline constexpr Matrix3(FromT... dt)
      : data{dt...} {
    }

    /**
     * Resets matrix to be identity matrix
     */
    inline void identity() __attribute__((__always_inline__)) {
      data = {1, 0, 0,
              0, 1, 0,
              0, 0, 1};
    }

    /**
     * Creates rotation matrix by rotation around three axes.
     * @param xDeg Angle (in degrees) of rotation around axis X.
     * @param yDeg Angle (in degrees) of rotation around axis Y.
     * @param zDeg Angle (in degrees) of rotation around axis Z.
     */
    inline static Matrix3<T> constexpr createRotationAroundAxis(T xDeg, T yDeg, T zDeg) __attribute__((__always_inline__)) {
      return createRotationAroundAxis_rad(deg2rad(xDeg), deg2rad(yDeg), deg2rad(zDeg));
    }

    /**
     * Creates rotation matrix by rotation around three axes, radian version.
     * @param xRads Angle (in radians) of rotation around axis X.
     * @param yRads Angle (in radians) of rotation around axis Y.
     * @param zRads Angle (in radians) of rotation around axis Z.
     */
    inline static Matrix3<T> constexpr createRotationAroundAxis_rad(T xRads, T yRads, T zRads) __attribute__((__always_inline__)) {
      // the static cast is to avoid narrowing conversion warnings when used with ints
      return Matrix3<T>({static_cast<T>(std::cos(zRads) * std::cos(yRads)),
                         static_cast<T>(std::cos(zRads) * std::sin(yRads) * std::sin(xRads) - std::sin(zRads) * std::cos(xRads)),
                         static_cast<T>(std::cos(zRads) * std::sin(yRads) * std::cos(xRads) + std::sin(zRads) * std::sin(xRads)),

                         static_cast<T>(std::sin(zRads) * std::cos(yRads)),
                         static_cast<T>(std::sin(zRads) * std::sin(yRads) * std::sin(xRads) + std::cos(zRads) * std::cos(xRads)),
                         static_cast<T>(std::sin(zRads) * std::sin(yRads) * std::cos(xRads) - std::cos(zRads) * std::sin(xRads)),

                         static_cast<T>(std::sin(yRads)),
                         static_cast<T>(std::cos(yRads) * std::sin(xRads)),
                         static_cast<T>(std::cos(yRads) * std::cos(xRads))});
    }

    /**
     * Creates rotation matrix by rotation around an axis.
     * @param axis Axis to rotate around.
     * @param angle Angle (in degrees) of rotation around axis.
     */
    inline static Matrix3<T> constexpr createRotationAroundAxis(Vector3<T> const &axis, T angle) __attribute__((__always_inline__)) {
      return createRotationAroundAxis_rad(axis, angle);
    }

    /**
     * Creates rotation matrix by rotation around an axis, radian version.
     * @param axis Axis to rotate around.
     * @param angle Angle (in radians) of rotation around axis.
     */
    inline static Matrix3<T> constexpr createRotationAroundAxis_rad(Vector3<T> const &axis, T angle) __attribute__((__always_inline__)) {
      // adapted from Inigo Quilez: http://www.iquilezles.org/www/articles/noacos/noacos.htm
      // the static cast is to avoid narrowing conversion warnings when used with ints
      return Matrix3<T>({static_cast<T>(axis.x * axis.x - std::cos(angle) + std::cos(angle)),
                         static_cast<T>(axis.y * axis.x - std::cos(angle) - std::sin(angle) * axis.z),
                         static_cast<T>(axis.z * axis.x - std::cos(angle) + std::sin(angle) * axis.y),

                         static_cast<T>(axis.x * axis.y - std::cos(angle) + std::sin(angle) * axis.z),
                         static_cast<T>(axis.y * axis.y - std::cos(angle) + std::cos(angle)),
                         static_cast<T>(axis.z * axis.y - std::cos(angle) - std::sin(angle) * axis.x),

                         static_cast<T>(axis.x * axis.z - std::cos(angle) - std::sin(angle) * axis.y),
                         static_cast<T>(axis.y * axis.z - std::cos(angle) + std::sin(angle) * axis.x),
                         static_cast<T>(axis.z * axis.z - std::cos(angle) + std::cos(angle))});
    }

    /**
     * Creates rotation matrix by aligning one vector to another.
     * Adapted from http://www.iquilezles.org/www/articles/noacos/noacos.htm
     * @param from Vector to rotate from.
     * @param to Vector to rotate to.
     * @return An instance of Matrix3<T> representing rotation between the two vectors.
     */
    inline static Matrix3<T> constexpr createRotationBetweenVectors(Vector3<T> from, Vector3<T> to) __attribute__((__always_inline__)) {
      // the static cast is to avoid narrowing conversion warnings when used with ints
      return Matrix3<T>({static_cast<T>(to.crossProduct(from).x * to.crossProduct(from).x * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) + to.dotProduct(from)),
                         static_cast<T>(to.crossProduct(from).y * to.crossProduct(from).x * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) - to.crossProduct(from).z),
                         static_cast<T>(to.crossProduct(from).z * to.crossProduct(from).x * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) + to.crossProduct(from).y),

                         static_cast<T>(to.crossProduct(from).x * to.crossProduct(from).y * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) + to.crossProduct(from).z),
                         static_cast<T>(to.crossProduct(from).y * to.crossProduct(from).y * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) + to.dotProduct(from)),
                         static_cast<T>(to.crossProduct(from).z * to.crossProduct(from).y * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) - to.crossProduct(from).x),

                         static_cast<T>(to.crossProduct(from).x * to.crossProduct(from).z * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) - to.crossProduct(from).y),
                         static_cast<T>(to.crossProduct(from).y * to.crossProduct(from).z * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) + to.crossProduct(from).x),
                         static_cast<T>(to.crossProduct(from).z * to.crossProduct(from).z * (static_cast<T>(1.0) / (static_cast<T>(1.0) + to.dotProduct(from))) + to.dotProduct(from))});
    }

    /**
     * Creates rotation matrix from ODE Matrix.
     */
    template<class It>
    inline static Matrix3<T> constexpr fromOde(const It *mat) {
      return Matrix3<T>({static_cast<T>(mat[0]), static_cast<T>(mat[4]), static_cast<T>(mat[8]),
                         static_cast<T>(mat[1]), static_cast<T>(mat[5]), static_cast<T>(mat[9]),
                         static_cast<T>(mat[2]), static_cast<T>(mat[6]), static_cast<T>(mat[10])});
    }

    /**
     * Creates new matrix 3x3 from array that represents such matrix 3x3
     * as array of tightly packed elements in row major order.
     * @param arr An array of elements for 3x3 matrix in row major order.
     * @return An instance of Matrix3<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix3<T> constexpr fromRowMajorArray(const FromT *arr) {
      return Matrix3<T>({static_cast<T>(arr[0]), static_cast<T>(arr[3]), static_cast<T>(arr[6]),
                         static_cast<T>(arr[1]), static_cast<T>(arr[4]), static_cast<T>(arr[7]),
                         static_cast<T>(arr[2]), static_cast<T>(arr[5]), static_cast<T>(arr[8])});
    }

    /**
     * Creates new matrix 3x3 from array that represents such matrix 3x3
     * as array of tightly packed elements in column major order.
     * @param arr An array of elements for 3x3 matrix in column major order.
     * @return An instance of Matrix3<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix3<T> constexpr fromColumnMajorArray(const FromT *arr) {
      return Matrix3<T>({static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]),
                         static_cast<T>(arr[3]), static_cast<T>(arr[4]), static_cast<T>(arr[5]),
                         static_cast<T>(arr[6]), static_cast<T>(arr[7]), static_cast<T>(arr[8])});
    }

    //---------------------[ equiality operators ]------------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold epsilon value. To be two
     * values equal, must satisfy this condition all elements of matrix
     * | lhs[i] - rhs[i] | < epsilon,
     * same for y-coordinate, z-coordinate, and w-coordinate.
     */
    inline bool constexpr operator==(Matrix3<T> const &rhs) const __attribute__((__always_inline__)) {
      #ifdef VMATH_SOFT_COMPARE
        return std::fabs(data[0] - rhs.data[0]) < static_cast<T>(epsilon) &&
               std::fabs(data[1] - rhs.data[1]) < static_cast<T>(epsilon) &&
               std::fabs(data[2] - rhs.data[2]) < static_cast<T>(epsilon) &&
               std::fabs(data[3] - rhs.data[3]) < static_cast<T>(epsilon) &&
               std::fabs(data[4] - rhs.data[4]) < static_cast<T>(epsilon) &&
               std::fabs(data[5] - rhs.data[5]) < static_cast<T>(epsilon) &&
               std::fabs(data[6] - rhs.data[6]) < static_cast<T>(epsilon) &&
               std::fabs(data[7] - rhs.data[7]) < static_cast<T>(epsilon) &&
               std::fabs(data[8] - rhs.data[8]) < static_cast<T>(epsilon);
      #else
        return data[0] == rhs.data[0] &&
               data[1] == rhs.data[1] &&
               data[2] == rhs.data[2] &&
               data[3] == rhs.data[3] &&
               data[4] == rhs.data[4] &&
               data[5] == rhs.data[5] &&
               data[6] == rhs.data[6] &&
               data[7] == rhs.data[7] &&
               data[8] == rhs.data[8];
      #endif // VMATH_SOFT_COMPARE
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Matrix3<T> const &rhs) const __attribute__((__always_inline__)) {
      return !(*this == rhs);
    }

    //---------------------[ access operators ]---------------------------------
    /**
     * Get reference to element at position (x,y).
     * @param x Number of column (0..2)
     * @param y Number of row (0..2)
     */
    inline T &at(int x, int y) __attribute__((__always_inline__)) {
      return data[x * 3 + y];
    }

    /**
     * Get constant reference to element at position (x,y).
     * @param x Number of column (0..2)
     * @param y Number of row (0..2)
     */
    inline T constexpr const &at(int x, int y) const __attribute__((__always_inline__)) {
      return data[x * 3 + y];
    }

    /**
     * Get reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..3)
     * @param j Number of column (1..3)
     */
    inline T &operator()(int i, int j) __attribute__((__always_inline__)) {
      return data[(j - 1) * 3 + i - 1];
    }

    /**
     * Get constant reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..3)
     * @param j Number of column (1..3)
     */
    inline T constexpr const &operator()(int i, int j) const __attribute__((__always_inline__)) {
      return data[(j - 1) * 3 + i - 1];
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> &operator=(Matrix3<T> const &rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs.data.data(), sizeof(T) * 9);
      return *this;
    }

    /**
     * Copy casting operator
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Matrix3<T> &operator=(Matrix3<FromT> const &rhs) {
      for(int i = 0; i != 9; ++i) {
        data[i] = static_cast<T>(rhs.data[i]);
      }
      return *this;
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> &operator=(T const *rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs, sizeof(T) * 9);
      return *this;
    }

    /*inline Matrix3<T> & operator=(const T *rhs)
     {
     for(int i = 0; i != 9; ++i) __attribute__((__always_inline__)) {
       data[i] = (T)rhs[i];
     }
     return *this;
     }*/

    //--------------------[ matrix with matrix operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator+(Matrix3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[0] + rhs.data[0], data[1] + rhs.data[1], data[2] + rhs.data[2],
                         data[3] + rhs.data[3], data[4] + rhs.data[4], data[5] + rhs.data[5],
                         data[6] + rhs.data[6], data[7] + rhs.data[7], data[8] + rhs.data[8]});
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator-(Matrix3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[0] - rhs.data[0], data[1] - rhs.data[1], data[2] - rhs.data[2],
                         data[3] - rhs.data[3], data[4] - rhs.data[4], data[5] - rhs.data[5],
                         data[6] - rhs.data[6], data[7] - rhs.data[7], data[8] - rhs.data[8]});
    }

    //--------------------[ matrix with scalar operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[0] + rhs, data[1] + rhs, data[2] + rhs,
                         data[3] + rhs, data[4] + rhs, data[5] + rhs,
                         data[6] + rhs, data[7] + rhs, data[8] + rhs});
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[0] - rhs, data[1] - rhs, data[2] - rhs,
                         data[3] - rhs, data[4] - rhs, data[5] - rhs,
                         data[6] - rhs, data[7] - rhs, data[8] - rhs});

    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[0] * rhs, data[1] * rhs, data[2] * rhs,
                         data[3] * rhs, data[4] * rhs, data[5] * rhs,
                         data[6] * rhs, data[7] * rhs, data[8] * rhs});
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[0] / rhs, data[1] / rhs, data[2] / rhs,
                         data[3] / rhs, data[4] / rhs, data[5] / rhs,
                         data[6] / rhs, data[7] / rhs, data[8] / rhs});
    }

    //--------------------[ multiply operators ]--------------------------------
    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
                        data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
                        data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     * Enabling Vector4 rotation as if it's a Vector3, maintaining W
     */
    inline Vector4<T> constexpr operator*(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
                        data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
                        data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z,
                        rhs.w);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator*(Matrix3<T> rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>({rhs.data[0] * data[0] + rhs.data[1] * data[3] + rhs.data[2] * data[6],
                         rhs.data[0] * data[1] + rhs.data[1] * data[4] + rhs.data[2] * data[7],
                         rhs.data[0] * data[2] + rhs.data[1] * data[5] + rhs.data[2] * data[8],

                         rhs.data[3] * data[0] + rhs.data[4] * data[3] + rhs.data[5] * data[6],
                         rhs.data[3] * data[1] + rhs.data[4] * data[4] + rhs.data[5] * data[7],
                         rhs.data[3] * data[2] + rhs.data[4] * data[5] + rhs.data[5] * data[8],

                         rhs.data[6] * data[0] + rhs.data[7] * data[3] + rhs.data[8] * data[6],
                         rhs.data[6] * data[1] + rhs.data[7] * data[4] + rhs.data[8] * data[7],
                         rhs.data[6] * data[2] + rhs.data[7] * data[5] + rhs.data[8] * data[8]});
    }

    //---------------------------[ misc operations ]----------------------------
    /**
     * Transpose matrix.
     */
    inline Matrix3<T> constexpr transpose() const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[0], data[3], data[6],
                         data[1], data[4], data[7],
                         data[2], data[5], data[8]});
    }

    /**
     * Linear interpolation of two matrices
     * @param fact Factor of interpolation. For translation from positon
     * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
     * @param rhs Second Matrix for interpolation
     * @note However values of fact parameter are reasonable only in interval
     * [0.0 , 1.0], you can pass also values outside of this interval and you
     * can get result (extrapolation?)
     */
    inline Matrix3<T> constexpr lerp(T fact, Matrix3<T> const &rhs) const __attribute__((__always_inline__)) {
      return (*this) + (rhs - (*this)) * fact;
    }

    inline T constexpr det() const __attribute__((__always_inline__)) {
      return   data[0] * data[4] * data[8] + data[1] * data[5] * data[6] + data[2] * data[3] * data[7]
             - data[0] * data[5] * data[7] - data[1] * data[3] * data[8] - data[2] * data[4] * data[6];
    }

    /**
     * Computes inverse matrix
     * @return Inverse matrix of this matrix.
     */
    inline Matrix3<T> constexpr inverse() const __attribute__((__always_inline__)) {
      return Matrix3<T>({data[4] * data[8] - data[7] * data[5],
                         data[7] * data[2] - data[1] * data[8],
                         data[1] * data[5] - data[4] * data[2],
                         data[6] * data[5] - data[3] * data[8],
                         data[0] * data[8] - data[6] * data[2],
                         data[3] * data[2] - data[0] * data[5],
                         data[3] * data[7] - data[6] * data[4],
                         data[6] * data[1] - data[0] * data[7],
                         data[0] * data[4] - data[3] * data[1]}) / det();
    }

    //-------------[ conversion ]-----------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Matrix3<T>)
     * used for passing Matrix3<T> values to gl*[fd]v functions.
     */
    inline operator T*() __attribute__((__always_inline__)) {
      return reinterpret_cast<T*>(data.data());
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Matrix3<T>)
     * used for passing Matrix3<T> values to gl*[fd]v functions.
     */
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
      return reinterpret_cast<T const*>(data.data());
    }

    //----------[ output operator ]----------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator <<(std::ostream &lhs, Matrix3<T> const &rhs) __attribute__((__always_inline__)) {
      for(int i = 0; i != 3; ++i) {
        lhs << "|\t";
        for(int j = 0; j != 3; ++j) {
          lhs << rhs.at(j, i) << "\t";
        }
        lhs << "|" << std::endl;
      }
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }
};

/// Matrix 3x3 of floats
using Matrix3f = Matrix3<float>;
/// Matrix 3x3 of doubles
using Matrix3d = Matrix3<double>;
/// Matrix 3x3 of long doubles
using Matrix3ld = Matrix3<long double>;
/// Matrix 3x3 of int
using Matrix3i = Matrix3<int>;

/**
 * Class for matrix 4x4
 * @note Data stored in this matrix are in column major order. This arrangement suits OpenGL.
 * If you're using row major matrix, consider using fromRowMajorArray as way for construction
 * Matrix4<T> instance.
 */
template<class T>
class Matrix4 {
  public:
    /// Data stored in column major order
    std::array<T, 16> data;

    //--------------------------[ constructors ]-------------------------------
    /**
     *Creates identity matrix
     */
    inline constexpr Matrix4()
      : data{1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1} {
    }

    /**
     * Copy matrix values from array (these data must be in column
     * major order!)
     */
    inline constexpr Matrix4(T const *dt)
      : data{dt[ 0], dt[ 1], dt[ 2], dt[ 3],
             dt[ 4], dt[ 5], dt[ 6], dt[ 7],
             dt[ 8], dt[ 9], dt[10], dt[11],
             dt[12], dt[13], dt[14], dt[15]} {
    }
    inline constexpr Matrix4(T *dt)
      : data{dt[ 0], dt[ 1], dt[ 2], dt[ 3],
             dt[ 4], dt[ 5], dt[ 6], dt[ 7],
             dt[ 8], dt[ 9], dt[10], dt[11],
             dt[12], dt[13], dt[14], dt[15]} {
    }

    /**
     * Copy constructor.
     * @param src Data source for new created instance of Matrix4.
     */
    inline constexpr Matrix4(Matrix4<T> const &src)
      : data{src.data[ 0], src.data[ 1], src.data[ 2], src.data[ 3],
             src.data[ 4], src.data[ 5], src.data[ 6], src.data[ 7],
             src.data[ 8], src.data[ 9], src.data[10], src.data[11],
             src.data[12], src.data[13], src.data[14], src.data[15]} {
    }

    /**
     * Copy casting constructor.
     * @param src Data source for new created instance of Matrix4.
     */
    template<class FromT>
    inline constexpr Matrix4(Matrix4<FromT> const &src)
      : data{static_cast<T>(src.data[ 0]), static_cast<T>(src.data[ 1]), static_cast<T>(src.data[ 2]), static_cast<T>(src.data[ 3]),
             static_cast<T>(src.data[ 4]), static_cast<T>(src.data[ 5]), static_cast<T>(src.data[ 6]), static_cast<T>(src.data[ 7]),
             static_cast<T>(src.data[ 8]), static_cast<T>(src.data[ 9]), static_cast<T>(src.data[10]), static_cast<T>(src.data[11]),
             static_cast<T>(src.data[12]), static_cast<T>(src.data[13]), static_cast<T>(src.data[14]), static_cast<T>(src.data[15])} {
    }

    /**
     * Variadic initialisation constructor
     * @param dt Initialiser list containing raw data for each element in order.
     */
    //inline constexpr Matrix4(std::initializer_list<T> dt)
    //  : data(dt) __attribute__((__always_inline__)) {
    //}
    // see http://stackoverflow.com/a/5549918/1678468
    template<class... FromT>
    inline constexpr Matrix4(FromT... dt)
      : data{dt...} {
    }
    //template<class... FromT>
    //inline constexpr Matrix4(FromT&&... dt)
    //  : data{std::forward<FromT>(dt)...} {
    //}

    /**
     * Resets matrix to be identity matrix
     */
    inline void identity() __attribute__((__always_inline__)) {
      data = {1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1};
    }

    /**
     * Creates rotation matrix by rotation around three axes.
     * @param xDeg Angle (in degrees) of rotation around axis X.
     * @param yDeg Angle (in degrees) of rotation around axis Y.
     * @param zDeg Angle (in degrees) of rotation around axis Z.
     */
    inline static Matrix4<T> constexpr createRotationAroundAxis(T xDeg, T yDeg, T zDeg) __attribute__((__always_inline__)) {
      return createRotationAroundAxis_rad(deg2rad(xDeg), deg2rad(yDeg), deg2rad(zDeg));
    }

    /**
     * Creates rotation matrix by rotation around three axes, radian version.
     * @param xRads Angle (in radians) of rotation around axis X.
     * @param yRads Angle (in radians) of rotation around axis Y.
     * @param zRads Angle (in radians) of rotation around axis Z.
     */
    inline static Matrix4<T> constexpr createRotationAroundAxis_rad(T xRads, T yRads, T zRads) __attribute__((__always_inline__)) {
      // static_casts are used to avoid narrowing conversion warnings when called with ints
      return Matrix4<T>({static_cast<T>( std::cos(zRads) * std::cos(yRads)),
                         static_cast<T>(-std::sin(zRads) * std::cos(xRads) + std::cos(zRads) * std::sin(yRads) * std::sin(xRads)),
                         static_cast<T>( std::sin(zRads) * std::sin(xRads) + std::cos(zRads) * std::sin(yRads) * std::cos(xRads)),
                         static_cast<T>(0),

                         static_cast<T>( std::sin(zRads) * std::cos(yRads)),
                         static_cast<T>( std::cos(zRads) * std::cos(xRads) + std::sin(zRads) * std::sin(yRads) * std::sin(xRads)),
                         static_cast<T>(-std::cos(zRads) * std::sin(xRads) + std::sin(zRads) * std::sin(yRads) * std::cos(xRads)),
                         static_cast<T>(0),

                         static_cast<T>(-std::sin(yRads)),
                         static_cast<T>( std::cos(yRads) * std::sin(xRads)),
                         static_cast<T>( std::cos(yRads) * std::cos(xRads)),
                         static_cast<T>(0),

                         static_cast<T>(0),
                         static_cast<T>(0),
                         static_cast<T>(0),
                         static_cast<T>(1)});
    }

    /**
     * Creates rotation matrix by rotation around an axis.
     * @param axis Axis to rotate around.
     * @param angle Angle (in degrees) of rotation around axis.
     */
    inline static Matrix4<T> constexpr createRotationAroundAxis(Vector3<T> const &axis, T angle) __attribute__((__always_inline__)) {
      return createRotationAroundAxis_rad(axis, angle);
    }

    /**
     * Creates rotation matrix by rotation around an axis, radian version.
     * @param axis Axis to rotate around.
     * @param angle Angle (in radians) of rotation around axis.
     */
    inline static Matrix4<T> constexpr createRotationAroundAxis_rad(Vector3<T> const &axis, T angle) __attribute__((__always_inline__)) {
      // adapted from Inigo Quilez: http://www.iquilezles.org/www/articles/noacos/noacos.htm
      // static_casts are used to avoid narrowing conversion warnings when called with ints
      return Matrix4<T>({static_cast<T>(axis.x * axis.x - std::cos(angle) + std::cos(angle)),
                         static_cast<T>(axis.y * axis.x - std::cos(angle) - std::sin(angle) * axis.z),
                         static_cast<T>(axis.z * axis.x - std::cos(angle) + std::sin(angle) * axis.y),
                         static_cast<T>(0),

                         static_cast<T>(axis.x * axis.y - std::cos(angle) + std::sin(angle) * axis.z),
                         static_cast<T>(axis.y * axis.y - std::cos(angle) + std::cos(angle)),
                         static_cast<T>(axis.z * axis.y - std::cos(angle) - std::sin(angle) * axis.x),
                         static_cast<T>(0),

                         static_cast<T>(axis.x * axis.z - std::cos(angle) - std::sin(angle) * axis.y),
                         static_cast<T>(axis.y * axis.z - std::cos(angle) + std::sin(angle) * axis.x),
                         static_cast<T>(axis.z * axis.z - std::cos(angle) + std::cos(angle)),
                         static_cast<T>(0),

                         static_cast<T>(0),
                         static_cast<T>(0),
                         static_cast<T>(0),
                         static_cast<T>(1)});
    }

    /// Creates translation matrix
    /**
     * Creates translation matrix.
     * @param x X-direction translation
     * @param y Y-direction translation
     * @param z Z-direction translation
     * @param w for W-coordinate translation (implicitly set to 1)
     */
    inline static Matrix4<T> constexpr createTranslation(T x, T y, T z, T w = 1) __attribute__((__always_inline__)) {
      return Matrix4<T>({static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
                         static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
                         static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
                         x,                 y,                 z,                 w});
    }

    /**
     * Create scale matrix with @a sx, @a sy, and @a sz
     * being values of matrix main diagonal.
     * @param sx Scale in X-axis
     * @param sy Scale in Y-axis
     * @param sz Scale in Z-axis
     * @return Transform matrix 4x4 with scale transformation.
     */
    inline static Matrix4<T> constexpr createScale(T sx, T sy, T sz) __attribute__((__always_inline__)) {
      return Matrix4<T>({sx,                static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
                         static_cast<T>(0), sy,                static_cast<T>(0), static_cast<T>(0),
                         static_cast<T>(0), static_cast<T>(0), sz,                static_cast<T>(0),
                         static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)});
    }

    /**
     * Creates new view matrix to look from specified position @a eyePos to specified position @a centerPos
     * @param eyePos A position of camera
     * @param centerPos A position where camera looks-at
     * @param upDir Direction of up vector
     * @return Resulting view matrix that looks from and at specific position.
     */
    inline static Matrix4<T> constexpr createLookAt(Vector3<T> const &eyePos, Vector3<T> const &centerPos, Vector3<T> const &upDir) __attribute__((__always_inline__)) {
      /*
      Vector3<T> forward = centerPos - eyePos;
      forward.normalize();

      // Side = forward x up
      Vector3<T> side = forward.crossProduct(upDir);
      side.normalize();

      // Recompute up as: up = side x forward
      Vector3<T> const up = side.crossProduct(forward);

      return Matrix4<T>({side.x,
                         up.x,
                         -forward.x,
                         0.0f,

                         side.y,
                         up.y,
                         -forward.y,
                         0.0f,

                         side.z,
                         up.z,
                         -forward.z,
                         0.0f,

                         0.0f,
                         0.0f,
                         0.0f,
                         1.0f}) * Matrix4<T>::createTranslation(-eyePos.x, -eyePos.y, -eyePos.z);
      */
      // constexpr-suitable return-only alternative, may turn out much slower when computed at runtime:
      return Matrix4<T>({ (centerPos - eyePos).normalize_copy().crossProduct(upDir).normalize_copy().x,
                          (centerPos - eyePos).normalize_copy().crossProduct(upDir).normalize_copy().crossProduct((centerPos - eyePos).normalize_copy()).x,
                         -(centerPos - eyePos).normalize_copy().x,
                          static_cast<T>(0),

                          (centerPos - eyePos).normalize_copy().crossProduct(upDir).normalize_copy().y,
                          (centerPos - eyePos).normalize_copy().crossProduct(upDir).normalize_copy().crossProduct((centerPos - eyePos).normalize_copy()).y,
                         -(centerPos - eyePos).normalize_copy().y,
                          static_cast<T>(0),

                          (centerPos - eyePos).normalize_copy().crossProduct(upDir).normalize_copy().z,
                          (centerPos - eyePos).normalize_copy().crossProduct(upDir).normalize_copy().crossProduct((centerPos - eyePos).normalize_copy()).z,
                         -(centerPos - eyePos).normalize_copy().z,
                          static_cast<T>(0),

                          static_cast<T>(0),
                          static_cast<T>(0),
                          static_cast<T>(0),
                          static_cast<T>(1)}) * Matrix4<T>::createTranslation(-eyePos.x, -eyePos.y, -eyePos.z);
    }

    /**
     * Creates OpenGL compatible perspective projection according to specified frustum parameters.
     *
     * @param left Specify the coordinate for the left vertical clipping plane,
     * @param right Specify the coordinate for the right vertical clipping plane.
     * @param bottom Specify the coordinate for the bottom horizontal clipping plane,
     * @param top Specify the coordinate for the top horizontal clipping plane.
     * @param zNear Specify the distance to the near clipping plane.  Distance must be positive.
     * @param zFar Specify the distance to the far depth clipping plane.  Distance must be positive.
     *
     * @return Projection matrix for specified frustum.
     */
    inline static Matrix4<T> constexpr createFrustum(T left, T right, T bottom, T top, T zNear, T zFar) __attribute__((__always_inline__)) {
      /*
       *    2 zNear
       *  ------------       0              A              0
       *  right - left
       *
       *                  2 zNear
       *      0         ------------        B              0
       *                top - bottom
       *
       *      0              0              C              D
       *
       *      0              0              -1             0
       *
       *  A =   (right + left) / (right - left)
       *  B =   (top + bottom) / (top - bottom)
       *  C = - (zFar + zNear) / (zFar - zNear)
       *  D = - (2 zFar zNear) / (zFar - zNear)
       */
      return Matrix4<T>({static_cast<T>(2) * zNear / (right - left),
                         static_cast<T>(0),
                         static_cast<T>(0),
                         static_cast<T>(0),

                         static_cast<T>(0),
                         static_cast<T>(2) * zNear / (top - bottom),
                         static_cast<T>(0),
                         static_cast<T>(0),

                          (right + left) / (right - left),
                          (top + bottom) / (top - bottom),
                         -(zFar + zNear) / (zFar - zNear),
                         static_cast<T>(-1),

                         static_cast<T>(0),
                         static_cast<T>(0),
                         static_cast<T>(-2) * zNear * zFar / (zFar - zNear),
                         static_cast<T>(1)});
    }

    /**
     * Creates OpenGL compatible orthographic projection matrix.
     * @param left Specify the coordinate for the left vertical clipping plane,
     * @param right Specify the coordinate for the right vertical clipping plane.
     * @param bottom Specify the coordinate for the bottom horizontal clipping plane,
     * @param top Specify the coordinate for the top horizontal clipping plane.
     * @param zNear Specify the distance to the nearer depth clipping plane.
     *       This value is negative if the plane is to be behind the viewer,
     * @param zFar Specify the distance to the farther depth clipping plane.
     *       This value is negative if the plane is to be behind the viewer.
     * @return Othrographic projection matrix.
     */
    inline static Matrix4<T> constexpr createOrtho(T left, T right, T bottom, T top, T zNear, T zFar) __attribute__((__always_inline__)) {
      /*      2
       *  ------------       0              0              tx
       *  right - left
       *                     2
       *      0         ------------        0              ty
       *                top - bottom
       *                                    -2
       *      0              0         ------------        tz
       *                                zFar-zNear
       *
       *      0              0              0              1
       *
       *    tx = - (right + left) / (right - left)
       *    ty = - (top + bottom) / (top - bottom)
       *    tz = - (zFar + zNear) / (zFar - zNear)
       */
      return Matrix4<T>({static_cast<T>(2) / (right - left), static_cast<T>(0),                  static_cast<T>(0),                   static_cast<T>(0),
                         static_cast<T>(0),                  static_cast<T>(2) / (top - bottom), static_cast<T>(0),                   static_cast<T>(0),
                         static_cast<T>(0),                  static_cast<T>(0),                  static_cast<T>(-2) / (zFar - zNear), static_cast<T>(0),
                         -(right + left)   / (right - left), -(top + bottom)   / (top - bottom), -(zFar + zNear)    / (zFar - zNear), static_cast<T>(1)});
    }

    /**
     * Creates new matrix 4x4 from array that represents such matrix 4x4
     * as array of tightly packed elements in row major order.
     * @param arr An array of elements for 4x4 matrix in row major order.
     * @return An instance of Matrix4<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix4<T> constexpr fromRowMajorArray(const FromT *arr) {
      return Matrix4<T>({static_cast<T>(arr[0]), static_cast<T>(arr[4]), static_cast<T>(arr[8]),  static_cast<T>(arr[12]),
                         static_cast<T>(arr[1]), static_cast<T>(arr[5]), static_cast<T>(arr[9]),  static_cast<T>(arr[13]),
                         static_cast<T>(arr[2]), static_cast<T>(arr[6]), static_cast<T>(arr[10]), static_cast<T>(arr[14]),
                         static_cast<T>(arr[3]), static_cast<T>(arr[7]), static_cast<T>(arr[11]), static_cast<T>(arr[15])});
    }

    /**
     * Creates new matrix 4x4 from array that represents such matrix 4x4
     * as array of tightly packed elements in column major order.
     * @param arr An array of elements for 4x4 matrix in column major order.
     * @return An instance of Matrix4<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix4<T> constexpr fromColumnMajorArray(const FromT *arr) {
      return Matrix4<T>({static_cast<T>(arr[0]),  static_cast<T>(arr[1]),  static_cast<T>(arr[2]),  static_cast<T>(arr[3]),
                         static_cast<T>(arr[4]),  static_cast<T>(arr[5]),  static_cast<T>(arr[6]),  static_cast<T>(arr[7]),
                         static_cast<T>(arr[8]),  static_cast<T>(arr[9]),  static_cast<T>(arr[10]), static_cast<T>(arr[11]),
                         static_cast<T>(arr[12]), static_cast<T>(arr[13]), static_cast<T>(arr[14]), static_cast<T>(arr[15])});
    }

    //---------------------[ Equality operators ]------------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold epsilon value. To be two
     * values equal, must satisfy this condition all elements of matrix
     * | lhs[i] - rhs[i] | < epsilon,
     * same for y-coordinate, z-coordinate, and w-coordinate.
     */
    inline bool constexpr operator==(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
      #ifdef VMATH_SOFT_COMPARE
        return std::fabs(data[ 0] - rhs.data[ 0]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 1] - rhs.data[ 1]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 2] - rhs.data[ 2]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 3] - rhs.data[ 3]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 4] - rhs.data[ 4]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 5] - rhs.data[ 5]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 6] - rhs.data[ 6]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 7] - rhs.data[ 7]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 8] - rhs.data[ 8]) < static_cast<T>(epsilon) &&
               std::fabs(data[ 9] - rhs.data[ 9]) < static_cast<T>(epsilon) &&
               std::fabs(data[10] - rhs.data[10]) < static_cast<T>(epsilon) &&
               std::fabs(data[11] - rhs.data[11]) < static_cast<T>(epsilon) &&
               std::fabs(data[12] - rhs.data[12]) < static_cast<T>(epsilon) &&
               std::fabs(data[13] - rhs.data[13]) < static_cast<T>(epsilon) &&
               std::fabs(data[14] - rhs.data[14]) < static_cast<T>(epsilon) &&
               std::fabs(data[15] - rhs.data[15]) < static_cast<T>(epsilon);
      #else
        return data[ 0] == rhs.data[ 0] &&
               data[ 1] == rhs.data[ 1] &&
               data[ 2] == rhs.data[ 2] &&
               data[ 3] == rhs.data[ 3] &&
               data[ 4] == rhs.data[ 4] &&
               data[ 5] == rhs.data[ 5] &&
               data[ 6] == rhs.data[ 6] &&
               data[ 7] == rhs.data[ 7] &&
               data[ 8] == rhs.data[ 8] &&
               data[ 9] == rhs.data[ 9] &&
               data[10] == rhs.data[10] &&
               data[11] == rhs.data[11] &&
               data[12] == rhs.data[12] &&
               data[13] == rhs.data[13] &&
               data[14] == rhs.data[14] &&
               data[15] == rhs.data[15];
      #endif // VMATH_SOFT_COMPARE
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
      return !(*this == rhs);
    }

    //---------------------[ access operators ]---------------------------------
    /**
     * Get reference to element at postion (x,y).
     * @param x Number of column (0..3)
     * @param y Number of row (0..3)
     */
    inline T &at(int x, int y) __attribute__((__always_inline__)) {
      return data[x * 4 + y];
    }

    /**
     * Get constant reference to element at position (x,y).
     * @param x Number of column (0..3)
     * @param y Number of row (0..3)
     */
    inline T constexpr const &at(int x, int y) const __attribute__((__always_inline__)) {
      return data[x * 4 + y];
    }

    /**
     * Get reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..4)
     * @param j Number of column (1..4)
     */
    inline T &operator()(int i, int j) __attribute__((__always_inline__)) {
      return data[(j - 1) * 4 + i - 1];
    }

    /**
     * Get constant reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..4)
     * @param j Number of column (1..4)
     */
    inline T constexpr const &operator()(int i, int j) const __attribute__((__always_inline__)) {
      return data[(j - 1) * 4 + i - 1];
    }

    /**
     * Sets translation part of matrix.
     *
     * @param v Vector of translation to be set.
     */
    inline void setTranslation(Vector3<T> const &v) __attribute__((__always_inline__)) {
      data[12] = v.x;
      data[13] = v.y;
      data[14] = v.z;
      data[15] = 1;
    }

    inline Vector3<T> constexpr getTranslation() __attribute__((__always_inline__)) {
      return Vector3<T>(data[12], data[13], data[14]);
    }

    /**
     * Sets rotation part (matrix 3x3) of matrix.
     *
     * @param m Rotation part of matrix
     */
    inline void setRotation(Matrix3<T> const &m) __attribute__((__always_inline__)) {
      for(int i = 0; i != 3; ++i) {
        for(int j = 0; j != 3; ++j) {
          at(i, j) = m.at(i, j);
        }
      }
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> &operator=(Matrix4<T> const &rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs.data.data(), sizeof(T) * 16);
      return *this;
    }

    /**
     * Copy casting operator
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Matrix4<T> &operator=(Matrix4<FromT> const &rhs) {
      for(int i = 0; i != 16; ++i) {
        data[i] = static_cast<T>(rhs.data[i]);
      }
      return *this;
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> &operator=(T const *rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs, sizeof(T) * 16);
      return *this;
    }

    /*inline Matrix4<T> & operator=(T const *rhs)
     {
     for(int i = 0; i != 16; ++i) {
       data[i] = (T)rhs[i];
     }
     return *this;
     }*/

    //--------------------[ matrix with matrix operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator+(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[ 0] + rhs.data[ 0], data[ 1] + rhs.data[ 1], data[ 2] + rhs.data[ 2], data[ 3] + rhs.data[ 3],
                         data[ 4] + rhs.data[ 4], data[ 5] + rhs.data[ 5], data[ 6] + rhs.data[ 6], data[ 7] + rhs.data[ 7],
                         data[ 8] + rhs.data[ 8], data[ 9] + rhs.data[ 9], data[10] + rhs.data[10], data[11] + rhs.data[11],
                         data[12] + rhs.data[12], data[13] + rhs.data[13], data[14] + rhs.data[14], data[15] + rhs.data[15]});
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator-(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[ 0] - rhs.data[ 0], data[ 1] - rhs.data[ 1], data[ 2] - rhs.data[ 2], data[ 3] - rhs.data[ 3],
                         data[ 4] - rhs.data[ 4], data[ 5] - rhs.data[ 5], data[ 6] - rhs.data[ 6], data[ 7] - rhs.data[ 7],
                         data[ 8] - rhs.data[ 8], data[ 9] - rhs.data[ 9], data[10] - rhs.data[10], data[11] - rhs.data[11],
                         data[12] - rhs.data[12], data[13] - rhs.data[13], data[14] - rhs.data[14], data[15] - rhs.data[15]});
    }

    //--------------------[ matrix with scalar operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[ 0] + rhs, data[ 1] + rhs, data[ 2] + rhs, data[ 3] + rhs,
                         data[ 4] + rhs, data[ 5] + rhs, data[ 6] + rhs, data[ 7] + rhs,
                         data[ 8] + rhs, data[ 9] + rhs, data[10] + rhs, data[11] + rhs,
                         data[12] + rhs, data[13] + rhs, data[14] + rhs, data[15] + rhs});
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[ 0] - rhs, data[ 1] - rhs, data[ 2] - rhs, data[ 3] - rhs,
                         data[ 4] - rhs, data[ 5] - rhs, data[ 6] - rhs, data[ 7] - rhs,
                         data[ 8] - rhs, data[ 9] - rhs, data[10] - rhs, data[11] - rhs,
                         data[12] - rhs, data[13] - rhs, data[14] - rhs, data[15] - rhs});
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[ 0] * rhs, data[ 1] * rhs, data[ 2] * rhs, data[ 3] * rhs,
                         data[ 4] * rhs, data[ 5] * rhs, data[ 6] * rhs, data[ 7] * rhs,
                         data[ 8] * rhs, data[ 9] * rhs, data[10] * rhs, data[11] * rhs,
                         data[12] * rhs, data[13] * rhs, data[14] * rhs, data[15] * rhs});
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[ 0] / rhs, data[ 1] / rhs, data[ 2] / rhs, data[ 3] / rhs,
                         data[ 4] / rhs, data[ 5] / rhs, data[ 6] / rhs, data[ 7] / rhs,
                         data[ 8] / rhs, data[ 9] / rhs, data[10] / rhs, data[11] / rhs,
                         data[12] / rhs, data[13] / rhs, data[14] / rhs, data[15] / rhs});
    }

    //--------------------[ multiply operators ]--------------------------------
    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(data[0] * rhs.x + data[4] * rhs.y + data[ 8] * rhs.z + data[12] * rhs.w,
                        data[1] * rhs.x + data[5] * rhs.y + data[ 9] * rhs.z + data[13] * rhs.w,
                        data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
                        data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(data[0] * rhs.x + data[4] * rhs.y + data[8]  * rhs.z,
                        data[1] * rhs.x + data[5] * rhs.y + data[9]  * rhs.z,
                        data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator*(Matrix4<T> rhs) const __attribute__((__always_inline__)) {
      return Matrix4<T>({rhs.data[ 0] * data[ 0] + rhs.data[ 1] * data[ 4] + rhs.data[ 2] * data[ 8] + rhs.data[ 3] * data[12],
                         rhs.data[ 0] * data[ 1] + rhs.data[ 1] * data[ 5] + rhs.data[ 2] * data[ 9] + rhs.data[ 3] * data[13],
                         rhs.data[ 0] * data[ 2] + rhs.data[ 1] * data[ 6] + rhs.data[ 2] * data[10] + rhs.data[ 3] * data[14],
                         rhs.data[ 0] * data[ 3] + rhs.data[ 1] * data[ 7] + rhs.data[ 2] * data[11] + rhs.data[ 3] * data[15],

                         rhs.data[ 4] * data[ 0] + rhs.data[ 5] * data[ 4] + rhs.data[ 6] * data[ 8] + rhs.data[ 7] * data[12],
                         rhs.data[ 4] * data[ 1] + rhs.data[ 5] * data[ 5] + rhs.data[ 6] * data[ 9] + rhs.data[ 7] * data[13],
                         rhs.data[ 4] * data[ 2] + rhs.data[ 5] * data[ 6] + rhs.data[ 6] * data[10] + rhs.data[ 7] * data[14],
                         rhs.data[ 4] * data[ 3] + rhs.data[ 5] * data[ 7] + rhs.data[ 6] * data[11] + rhs.data[ 7] * data[15],

                         rhs.data[ 8] * data[ 0] + rhs.data[ 9] * data[ 4] + rhs.data[10] * data[ 8] + rhs.data[11] * data[12],
                         rhs.data[ 8] * data[ 1] + rhs.data[ 9] * data[ 5] + rhs.data[10] * data[ 9] + rhs.data[11] * data[13],
                         rhs.data[ 8] * data[ 2] + rhs.data[ 9] * data[ 6] + rhs.data[10] * data[10] + rhs.data[11] * data[14],
                         rhs.data[ 8] * data[ 3] + rhs.data[ 9] * data[ 7] + rhs.data[10] * data[11] + rhs.data[11] * data[15],

                         rhs.data[12] * data[ 0] + rhs.data[13] * data[ 4] + rhs.data[14] * data[ 8] + rhs.data[15] * data[12],
                         rhs.data[12] * data[ 1] + rhs.data[13] * data[ 5] + rhs.data[14] * data[ 9] + rhs.data[15] * data[13],
                         rhs.data[12] * data[ 2] + rhs.data[13] * data[ 6] + rhs.data[14] * data[10] + rhs.data[15] * data[14],
                         rhs.data[12] * data[ 3] + rhs.data[13] * data[ 7] + rhs.data[14] * data[11] + rhs.data[15] * data[15]});
    }

    //---------------------------[ misc operations ]----------------------------

    /**
     * Computes determinant of matrix
     * @return Determinant of matrix
     * @note This function does 3 * 4 * 6 mul, 3 * 6 add.
     */
    inline T constexpr det() const __attribute__((__always_inline__)) {
      return   data[12] * data[9] * data[6]  * data[3]  - data[8] * data[13] * data[6]  * data[3]
             - data[12] * data[5] * data[10] * data[3]  + data[4] * data[13] * data[10] * data[3]

             + data[8]  * data[5] * data[14] * data[3]  - data[4] * data[9]  * data[14] * data[3]
             - data[12] * data[9] * data[2]  * data[7]  + data[8] * data[13] * data[2]  * data[7]

             + data[12] * data[1] * data[10] * data[7]  - data[0] * data[13] * data[10] * data[7]
             - data[8]  * data[1] * data[14] * data[7]  + data[0] * data[9]  * data[14] * data[7]

             + data[12] * data[5] * data[2]  * data[11] - data[4] * data[13] * data[2]  * data[11]
             - data[12] * data[1] * data[6]  * data[11] + data[0] * data[13] * data[6]  * data[11]

             + data[4]  * data[1] * data[14] * data[11] - data[0] * data[5]  * data[14] * data[11]
             - data[8]  * data[5] * data[2]  * data[15] + data[4] * data[9]  * data[2]  * data[15]

             + data[8]  * data[1] * data[6]  * data[15] - data[0] * data[9]  * data[6]  * data[15]
             - data[4]  * data[1] * data[10] * data[15] + data[0] * data[5]  * data[10] * data[15];
    }

    /**
     * Computes inverse matrix
     * @return Inverse matrix of this matrix.
     * @note This is a little bit time consuming operation
     * (16 * 6 * 3 mul, 16 * 5 add + det() + mul() functions)
     */
    inline Matrix4<T> constexpr inverse() const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[9]  * data[14] * data[7]  - data[13] * data[10] * data[7]  + data[13] * data[6]  * data[11] -
                         data[5]  * data[14] * data[11] - data[9]  * data[6]  * data[15] + data[5]  * data[10] * data[15],
                         data[13] * data[10] * data[3]  - data[9]  * data[14] * data[3]  - data[13] * data[2]  * data[11] +
                         data[1]  * data[14] * data[11] + data[9]  * data[2]  * data[15] - data[1]  * data[10] * data[15],
                         data[5]  * data[14] * data[3]  - data[13] * data[6]  * data[3]  + data[13] * data[2]  * data[7] -
                         data[1]  * data[14] * data[7]  - data[5]  * data[2]  * data[15] + data[1]  * data[6]  * data[15],
                         data[9]  * data[6]  * data[3]  - data[5]  * data[10] * data[3]  - data[9]  * data[2]  * data[7] +
                         data[1]  * data[10] * data[7]  + data[5]  * data[2]  * data[11] - data[1]  * data[6]  * data[11],
                         data[12] * data[10] * data[7]  - data[8]  * data[14] * data[7]  - data[12] * data[6]  * data[11] +
                         data[4]  * data[14] * data[11] + data[8]  * data[6]  * data[15] - data[4]  * data[10] * data[15],
                         data[8]  * data[14] * data[3]  - data[12] * data[10] * data[3]  + data[12] * data[2]  * data[11] -
                         data[0]  * data[14] * data[11] - data[8]  * data[2]  * data[15] + data[0]  * data[10] * data[15],
                         data[12] * data[6]  * data[3]  - data[4]  * data[14] * data[3]  - data[12] * data[2]  * data[7] +
                         data[0]  * data[14] * data[7]  + data[4]  * data[2]  * data[15] - data[0]  * data[6]  * data[15],
                         data[4]  * data[10] * data[3]  - data[8]  * data[6]  * data[3]  + data[8]  * data[2]  * data[7] -
                         data[0]  * data[10] * data[7]  - data[4]  * data[2]  * data[11] + data[0]  * data[6]  * data[11],
                         data[8]  * data[13] * data[7]  - data[12] * data[9]  * data[7]  + data[12] * data[5]  * data[11] -
                         data[4]  * data[13] * data[11] - data[8]  * data[5]  * data[15] + data[4]  * data[9]  * data[15],
                         data[12] * data[9]  * data[3]  - data[8]  * data[13] * data[3]  - data[12] * data[1]  * data[11] +
                         data[0]  * data[13] * data[11] + data[8]  * data[1]  * data[15] - data[0]  * data[9]  * data[15],
                         data[4]  * data[13] * data[3]  - data[12] * data[5]  * data[3]  + data[12] * data[1]  * data[7] -
                         data[0]  * data[13] * data[7]  - data[4]  * data[1]  * data[15] + data[0]  * data[5]  * data[15],
                         data[8]  * data[5]  * data[3]  - data[4]  * data[9]  * data[3]  - data[8]  * data[1]  * data[7] +
                         data[0]  * data[9]  * data[7]  + data[4]  * data[1]  * data[11] - data[0]  * data[5]  * data[11],
                         data[12] * data[9]  * data[6]  - data[8]  * data[13] * data[6]  - data[12] * data[5]  * data[10] +
                         data[4]  * data[13] * data[10] + data[8]  * data[5]  * data[14] - data[4]  * data[9]  * data[14],
                         data[8]  * data[13] * data[2]  - data[12] * data[9]  * data[2]  + data[12] * data[1]  * data[10] -
                         data[0]  * data[13] * data[10] - data[8]  * data[1]  * data[14] + data[0]  * data[9]  * data[14],
                         data[12] * data[5]  * data[2]  - data[4]  * data[13] * data[2]  - data[12] * data[1]  * data[6] +
                         data[0]  * data[13] * data[6]  + data[4]  * data[1]  * data[14] - data[0]  * data[5]  * data[14],
                         data[4]  * data[9]  * data[2]  - data[8]  * data[5]  * data[2]  + data[8]  * data[1]  * data[6] -
                         data[0]  * data[9]  * data[6]  - data[4]  * data[1]  * data[10] + data[0]  * data[5]  * data[10]}) / det();
    }

    /**
     * Transpose matrix.
     */
    inline Matrix4<T> constexpr transpose() const __attribute__((__always_inline__)) {
      return Matrix4<T>({data[0], data[4], data[8],  data[12],
                         data[1], data[5], data[9],  data[13],
                         data[2], data[6], data[10], data[14],
                         data[3], data[7], data[11], data[15]});
    }

    /**
     * Linear interpolation of two matrices
     * @param fact Factor of interpolation. For translation from positon
     * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
     * @param rhs Second Matrix for interpolation
     * @note However values of fact parameter are reasonable only in interval
     * [0.0 , 1.0], you can pass also values outside of this interval and you
     * can get result (extrapolation?)
     */
    inline Matrix4<T> constexpr lerp(T fact, Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
      return (*this) + (rhs - (*this)) * fact;
    }

    //-------------[ conversion ]-----------------------------
    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Matrix4<T>)
     * used for passing Matrix4<T> values to gl*[fd]v functions.
     */
    inline operator T*() __attribute__((__always_inline__)) {
      return reinterpret_cast<T*>(data.data());
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Matrix4<T>)
     * used for passing Matrix4<T> values to gl*[fd]v functions.
     */
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
      return reinterpret_cast<T const*>(data.data());
    }

    //----------[ output operator ]----------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator <<(std::ostream &lhs, Matrix4<T> const &rhs) __attribute__((__always_inline__)) {
      for(int i = 0; i != 4; ++i) {
        lhs << "|\t";
        for(int j = 0; j != 4; ++j) {
          lhs << rhs.at(j, i) << "\t";
        }
        lhs << "|" << std::endl;
      }
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }
};

/// Matrix 4x4 of floats
using Matrix4f = Matrix4<float>;
/// Matrix 4x4 of doubles
using Matrix4d = Matrix4<double>;
/// Matrix 4x4 of long doubles
using Matrix4ld = Matrix4<long double>;
/// Matrix 4x4 of int
using Matrix4i = Matrix4<int>;

/**
 * Quaternion class implementing some quaternion algebra operations.
 * Quaternion is kind of complex number it consists of its real part (w)
 * and its complex part v. This complex part has three elements, so we
 * can express it as xi + yj + zk . Note that coordinates of (x,y,z) are
 * hold inside v field.
 */
template<class T>
class Quaternion {
  public:

    union {
      /**
       * Real part of quaternion.
       */
      T w;
      T real;
      T scalar;
    };

    union {
      /**
       * Imaginary part of quaternion.
       */
      Vector3<T> v;
      Vector3<T> imaginary;
      Vector3<T> vector;
    };

    /**
     * Quaternion constructor, sets quaternion to (0 + 0i + 0j + 0k).
     */
    inline constexpr Quaternion() __attribute__((__always_inline__))
      : w(0), v(0, 0, 0) {
    }

    /**
     * Copy constructor.
     */
    inline constexpr Quaternion(Quaternion<T> const &q) __attribute__((__always_inline__))
      : w(q.w), v(q.v) {
    }

    /**
     * Copy casting constructor.
     */
    template<class FromT>
    inline constexpr Quaternion(Quaternion<FromT> const &q)
      : w(static_cast<T>(q.w)), v(q.v) {
    }

    /**
     * Creates quaternion object from real part w_ and complex part v_.
     * @param w_ Real part of quaternion.
     * @param v_ Complex part of quaternion (xi + yj + zk).
     */
    inline constexpr Quaternion(T w_, Vector3<T> const &v_) __attribute__((__always_inline__))
      : w(w_), v(v_) {
    }

    /**
     * Creates quaternion object from value (w_ + xi + yj + zk).
     * @param w_ Real part of quaternion.
     * @param x Complex coefficient for i complex constant.
     * @param y Complex coefficient for j complex constant.
     * @param z Complex coefficient for k complex constant.
     */
    inline constexpr Quaternion(T w_, T x, T y, T z) __attribute__((__always_inline__))
      : w(w_), v(x, y, z) {
    }

    /**
     * Construct quaternion from rotation matrix.
     * @return Rotation matrix expressing this quaternion.
     */
    inline Quaternion(Matrix4<T> const &matrix) __attribute__((__always_inline__)) {
      // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
      // article "Quaternion Calculus and Fast Animation".
      T const trace = matrix.at(1, 1) + matrix.at(2, 2) + matrix.at(3, 3);
      if(trace > 0) {
        // |w| > 1/2, may as well choose w > 1/2
        T root = std::sqrt(trace + static_cast<T>(1.0));  // 2w
        w = static_cast<T>(0.5) * root;
        root = static_cast<T>(0.5) / root;  // 1/(4w)
        v.x = (matrix.at(3, 2) - matrix.at(2, 3)) * root;
        v.y = (matrix.at(1, 3) - matrix.at(3, 1)) * root;
        v.z = (matrix.at(2, 1) - matrix.at(1, 2)) * root;
      } else {
        // |w| <= 1/2
        static int constexpr next[3] = {2, 3, 1};

        int i = 1;
        if(matrix.at(2, 2) > matrix.at(1, 1)) {
          i = 2;
        }
        if(matrix.at(3, 3) > matrix.at(i, i)) {
          i = 3;
        }
        int j = next[i];
        int k = next[j];

        T root = std::sqrt(matrix.at(i, i) - matrix.at(j, j) - matrix.at(k, k) + static_cast<T>(1.0));
        T *quaternion[3] = {&v.x, &v.y, &v.z};
        *quaternion[i] = static_cast<T>(0.5) * root;
        root = static_cast<T>(0.5) / root;
        w = (matrix.at(k, j) - matrix.at(j, k)) * root;
        *quaternion[j] = (matrix.at(j, i) + matrix.at(i, j)) * root;
        *quaternion[k] = (matrix.at(k, i) + matrix.at(i, k)) * root;
      }
    }

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (w_ + xi + yj + zk).
     * @param w_ Real part of quaternion.
     * @param x Complex coefficient for i complex constant.
     * @param y Complex coefficient for j complex constant.
     * @param z Complex coefficient for k complex constant.
     */
    inline void assign(T w_ = 0, T x = 0, T y = 0, T z = 0) __attribute__((__always_inline__)) {
      w = w_;
      v.assign(x, y, z);
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
      v = rhs.v;
      w = rhs.w;
      return *this;
    }

    /**
     * Copy convert operator
     * @param rhs Right hand side argument of binary operator.
     */
    template<class FromT>
    inline Quaternion<T> &operator=(Quaternion<FromT> const &rhs) {
      v = rhs.v;
      w = static_cast<T>(rhs.w);
      return *this;
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator+(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
      return Quaternion<T>(w + rhs.w, v + rhs.v);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator*(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
      return Quaternion<T>(w * rhs.w   - v.x * rhs.v.x - v.y * rhs.v.y - v.z * rhs.v.z,
                           w * rhs.v.x + v.x * rhs.w   + v.y * rhs.v.z - v.z * rhs.v.y,
                           w * rhs.v.y - v.x * rhs.v.z + v.y * rhs.w   + v.z * rhs.v.x,
                           w * rhs.v.z + v.x * rhs.v.y - v.y * rhs.v.x + v.z * rhs.w);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
      return Quaternion<T>(w * rhs, v * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Quaternion<T>(w / rhs, v / rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator-(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
      return Quaternion<T>(w - rhs.w, v - rhs.v);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator+=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
      w += rhs.w;
      v += rhs.v;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator-=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
      w -= rhs.w;
      v -= rhs.v;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator*=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
      Quaternion q = (*this) * rhs;
      v = q.v;
      w = q.w;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator*=(T rhs) __attribute__((__always_inline__)) {
      w *= rhs;
      v *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator/=(T rhs) __attribute__((__always_inline__)) {
      w /= rhs;
      v /= rhs;
      return *this;
    }

    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold epsilon value. To be two
     * values equal, must satisfy this condition | lhs - rhs | < epsilon,
     * for all quaternion coordinates.
     */
    inline bool constexpr operator==(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
      #ifdef VMATH_SOFT_COMPARE
        return (std::fabs(w - rhs.w) < static_cast<T>(epsilon)) && v == rhs.v;
      #else
        return w == rhs.w && v == rhs.v;
      #endif // VMATH_SOFT_COMPARE
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated quaternion
     */
    inline Quaternion<T> constexpr operator-() const __attribute__((__always_inline__)) {
      return Quaternion<T>(-w, -v);
    }

    /**
     * Unary conjugate operator
     * @return conjugated quaternion
     */
    inline Quaternion<T> constexpr operator~() const __attribute__((__always_inline__)) {
      return Quaternion<T>(w, -v);
    }

    /**
     * Get length of quaternion.
     * @return Length of quaternion.
     */
    inline T constexpr length() const __attribute__((__always_inline__)) {
      return static_cast<T>(std::sqrt(w * w + v.lengthSq()));
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two quaternion can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
      return w * w + v.lengthSq();
    }

    /**
     * Normalize quaternion
     */
    inline void normalize() __attribute__((__always_inline__)) {
      T len = length();
      w /= len;
      v /= len;
    }
    inline void normalise() __attribute__((__always_inline__)) {  // proper english
      normalize();
    }
    inline Quaternion<T> constexpr normalize_copy() const __attribute__((__always_inline__)) {
      return Quaternion<T>(w / length(), v / length());
    }
    inline Quaternion<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {  // proper english
      return normalize_copy();
    }

    inline void conjugate() __attribute__((__always_inline__)) {
      v = -v;
    }

    inline Quaternion<T> constexpr conjugate_copy() const __attribute__((__always_inline__)) {
      return Quaternion<T>(w, -v);
    }

    /**
     * @brief Computes the inverse of this quaternion.
     *
     * @note This is a general inverse.  If you know a priori
     * that you're using a unit quaternion (i.e., norm() == 1),
     * it will be significantly faster to use conjugate() instead.
     *
     * @return The quaternion q such that q * (*this) == (*this) * q
     * == [ 0 0 0 1 ]<sup>T</sup>.
     */
    inline void invert() __attribute__((__always_inline__)) {
      T l = length();
      conjugate();
      (*this) /= l;
    }

    inline Quaternion<T> constexpr invert_copy() const __attribute__((__always_inline__)) {
      return conjugate_copy() / length();
    }

    /**
     * Creates quaternion for Euler angles.
     * @param x Rotation around x axis (in degrees).
     * @param y Rotation around y axis (in degrees).
     * @param z Rotation around z axis (in degrees).
     * @return Quaternion object representing transformation.
     */
    inline static Quaternion<T> constexpr fromEulerAngles(T x, T y, T z) __attribute__((__always_inline__)) {
      return Quaternion<T>(fromAxisRot(Vector3<T>(1, 0, 0), x) *
                           fromAxisRot(Vector3<T>(0, 1, 0), y) *
                           fromAxisRot(Vector3<T>(0, 0, 1), z));
    }

    /**
     * Creates quaternion for Euler angles, rad version.
     * @param x Rotation around x axis (in radians).
     * @param y Rotation around y axis (in radians).
     * @param z Rotation around z axis (in radians).
     * @return Quaternion object representing transformation.
     */
    inline static Quaternion<T> constexpr fromEulerAngles_rad(T x, T y, T z) __attribute__((__always_inline__)) {
      return Quaternion<T>(fromAxisRot_rad(Vector3<T>(1, 0, 0), x) *
                           fromAxisRot_rad(Vector3<T>(0, 1, 0), y) *
                           fromAxisRot_rad(Vector3<T>(0, 0, 1), z));
    }

    /**
     * Creates quaternion as rotation around axis.
     * @param axis Unit vector expressing axis of rotation.
     * @param angleDeg Angle of rotation around axis (in degrees).
     */
    inline static Quaternion<T> constexpr fromAxisRot(Vector3<T> axis, T angleDeg) __attribute__((__always_inline__)) {
      return fromAxisRot_rad(axis, deg2rad(angleDeg));
    }

    /**
     * Creates quaternion as rotation around axis, rad version.
     * @param axis Unit vector expressing axis of rotation.
     * @param angleDeg Angle of rotation around axis (in radians).
     */
    inline static Quaternion<T> constexpr fromAxisRot_rad(Vector3<T> axis, T angleRad) __attribute__((__always_inline__)) {
      return Quaternion<T>(std::cos(angleRad / static_cast<T>(2.0)), axis * std::sin(angleRad / static_cast<T>(2.0)));
    }

    /**
     * Converts this quaternion to an axis and angle combination
     * @param angle The angle of rotation
     * @param axis The axis around which the rotation is
     */
    inline void constexpr toAngleAxis(T &angle, Vector3<T> &axis) __attribute__((__always_inline__)) {
      float const squareLength = v.lengthSq();
      if(squareLength != 0) {
        angle = static_cast<T>(2.0) * std::acos(w);
        axis = v / std::pow(squareLength, static_cast<T>(0.5));
      } else {
        angle = static_cast<T>(0.0);
        axis.assign(static_cast<T>(1.0), static_cast<T>(0.0), static_cast<T>(0.0));
      }
    }

    /**
     * Converts quaternion into rotation matrix.
     * @return Rotation matrix expressing this quaternion.
     */
    inline Matrix3<T> constexpr rotMatrix() __attribute__((__always_inline__)) {
      return Matrix3<T>({1 - 2 * (v.y * v.y + v.z * v.z),     2 * (v.x * v.y + v.z * w),       2 * (v.x * v.z - v.y * w),
                             2 * (v.x * v.y - v.z * w),   1 - 2 * (v.x * v.x + v.z * v.z),     2 * (v.y * v.z + v.x * w),
                             2 * (v.x * v.z + v.y * w),       2 * (v.y * v.z - v.x * w),   1 - 2 * (v.x * v.x + v.y * v.y)});
    }

    /**
     * Converts quaternion into transformation matrix.
     * @note This method performs same operation as rotMatrix()
     * conversion method. But returns Matrix of 4x4 elements.
     * @return Transformation matrix expressing this quaternion.
     */
    inline Matrix4<T> constexpr transform() const __attribute__((__always_inline__)) {
      return Matrix4<T>({1 - 2 * (v.y * v.y + v.z * v.z),     2 * (v.x * v.y + v.z * w),       2 * (v.x * v.z - v.y * w),   0.0f,
                             2 * (v.x * v.y - v.z * w),   1 - 2 * (v.x * v.x + v.z * v.z),     2 * (v.y * v.z + v.x * w),   0.0f,
                             2 * (v.x * v.z + v.y * w),       2 * (v.y * v.z - v.x * w),   1 - 2 * (v.x * v.x + v.y * v.y), 0.0f,
                         0.0f,                            0.0f,                            0.0f,                            1.0f});
    }

    /**
     * Linear interpolation of two quaternions
     * @param fact Factor of interpolation. For translation from position
     * of this vector to quaternion rhs, values of factor goes from 0.0 to 1.0.
     * @param rhs Second Quaternion for interpolation
     * @note However values of fact parameter are reasonable only in interval
     * [0.0 , 1.0], you can pass also values outside of this interval and you
     * can get result (extrapolation?)
     */
    inline Quaternion<T> constexpr lerp(T fact, Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
      return Quaternion<T>((1 - fact) * w + fact * rhs.w, v.lerp(fact, rhs.v));
    }

    /**
     * Provides output to standard output stream.
     */
    inline friend std::ostream &operator <<(std::ostream &oss, Quaternion<T> const &q) __attribute__((__always_inline__)) {
      oss << "Re: " << q.w << " Im: " << q.v;
      return oss;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

    /**
     * Creates quaternion from transform matrix.
     *
     * @param m Transform matrix used to compute quaternion.
     * @return Quaternion representing rotation of matrix m.
     */
    // 2011-07-02: Davide Bacchet: changed formula to fix degenerate cases
    inline static Quaternion<T> fromMatrix(Matrix4<T> const &m) __attribute__((__always_inline__)) {
      Quaternion<T> q;

      T s;
      T const tr = m(1, 1) + m(2, 2) + m(3, 3);
      if(tr >= static_cast<T>(epsilon)) {
        s = static_cast<T>(0.5) / static_cast<T>(std::sqrt(tr + static_cast<T>(1.0)));
        q.w = static_cast<T>(0.25) / s;
        q.v.x = (m(3, 2) - m(2, 3)) * s;
        q.v.y = (m(1, 3) - m(3, 1)) * s;
        q.v.z = (m(2, 1) - m(1, 2)) * s;
      } else {
        T d0 = m(1, 1);
        T d1 = m(2, 2);
        T d2 = m(3, 3);

        char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);

        if(bigIdx == 0) {
          s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(1, 1) - m(2, 2) - m(3, 3)));
          q.w = (m(3, 2) - m(2, 3)) / s;
          q.v.x = static_cast<T>(0.25) * s;
          q.v.y = (m(1, 2) + m(2, 1)) / s;
          q.v.z = (m(1, 3) + m(3, 1)) / s;
        } else if(bigIdx == 1) {
          s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(2, 2) - m(1, 1) - m(3, 3)));
          q.w = (m(1, 3) - m(3, 1)) / s;
          q.v.x = (m(1, 2) + m(2, 1)) / s;
          q.v.y = static_cast<T>(0.25) * s;
          q.v.z = (m(2, 3) + m(3, 2)) / s;
        } else {
          s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(3, 3) - m(1, 1) - m(2, 2)));
          q.w = (m(2, 1) - m(1, 2)) / s;
          q.v.x = (m(1, 3) + m(3, 1)) / s;
          q.v.y = (m(2, 3) + m(3, 2)) / s;
          q.v.z = static_cast<T>(0.25) * s;
        }
      }

      return q;
    }

    /**
     * Creates quaternion from rotation matrix.
     *
     * @param m Rotation matrix used to compute quaternion.
     * @return Quaternion representing rotation of matrix m.
     */

    // 2011-07-02: Davide Bacchet: changed formula to fix degenerate cases
    inline static Quaternion<T> fromMatrix(Matrix3<T> const &m) __attribute__((__always_inline__)) {
      Quaternion<T> q;

      T s;
      T const tr = m(1, 1) + m(2, 2) + m(3, 3);
      if(tr >= static_cast<T>(epsilon)) {
        s = static_cast<T>(0.5) / static_cast<T>(std::sqrt(tr + static_cast<T>(1.0)));
        q.w = static_cast<T>(0.25) / s;
        q.v.x = (m(3, 2) - m(2, 3)) * s;
        q.v.y = (m(1, 3) - m(3, 1)) * s;
        q.v.z = (m(2, 1) - m(1, 2)) * s;
      } else {
        T d0 = m(1, 1);
        T d1 = m(2, 2);
        T d2 = m(3, 3);

        char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);

        if(bigIdx == 0) {
          s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(1, 1) - m(2, 2) - m(3, 3)));
          q.w = (m(3, 2) - m(2, 3)) / s;
          q.v.x = static_cast<T>(0.25) * s;
          q.v.y = (m(1, 2) + m(2, 1)) / s;
          q.v.z = (m(1, 3) + m(3, 1)) / s;
        } else if(bigIdx == 1) {
          s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(2, 2) - m(1, 1) - m(3, 3)));
          q.w = (m(1, 3) - m(3, 1)) / s;
          q.v.x = (m(1, 2) + m(2, 1)) / s;
          q.v.y = static_cast<T>(0.25) * s;
          q.v.z = (m(2, 3) + m(3, 2)) / s;
        } else {
          s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(3, 3) - m(1, 1) - m(2, 2)));
          q.w = (m(2, 1) - m(1, 2)) / s;
          q.v.x = (m(1, 3) + m(3, 1)) / s;
          q.v.y = (m(2, 3) + m(3, 2)) / s;
          q.v.z = static_cast<T>(0.25) * s;
        }
      }

      return q;
    }

    /**
     * Computes spherical interpolation between quaternions (this, q2)
     * using coefficient of interpolation r (in [0, 1]).
     *
     * @param r The ratio of interpolation form this (r = 0) to q2 (r = 1).
     * @param q2 Second quaternion for interpolation.
     * @return Result of interpolation.
     */
    inline Quaternion<T> slerp(T r, Quaternion<T> const &q2) const __attribute__((__always_inline__)) {
      Quaternion<T> ret;
      T const cosTheta = w * q2.w + v.x * q2.v.x + v.y * q2.v.y + v.z * q2.v.z;
      T const theta = static_cast<T>(std::acos(cosTheta));
      if(std::fabs(theta) < static_cast<T>(epsilon)) {
        ret = *this;
      } else {
        T sinTheta = static_cast<T>(std::sqrt(static_cast<T>(1.0) - cosTheta * cosTheta));
        if(std::fabs(sinTheta) < static_cast<T>(epsilon)) {
          ret.w = static_cast<T>(0.5) * w + static_cast<T>(0.5) * q2.w;
          ret.v = v.lerp(static_cast<T>(0.5), q2.v);
        } else {
          T rA = static_cast<T>(std::sin((static_cast<T>(1.0) - r) * theta)) / sinTheta;
          T rB = static_cast<T>(std::sin(r * theta)) / sinTheta;

          ret.w = w * rA + q2.w * rB;
          ret.v.x = v.x * rA + q2.v.x * rB;
          ret.v.y = v.y * rA + q2.v.y * rB;
          ret.v.z = v.z * rA + q2.v.z * rB;
        }
      }
      return ret;
    }

};

/// Quaternion of floats
using Quatf = Quaternion<float>;
/// Quaternion of doubles
using Quatd = Quaternion<double>;
/// Quaternion of long doubles
using Quatld = Quaternion<long double>;

#ifdef VMATH_NAMESPACE
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard C++ library extensions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Shortcut defines
#ifdef VMATH_NAMESPACE
#define VEC2 VMATH_NAMESPACE::Vector2
#define VEC3 VMATH_NAMESPACE::Vector3
#define VEC4 VMATH_NAMESPACE::Vector4
#else
#define VEC2 Vector2
#define VEC3 Vector3
#define VEC4 Vector4
#endif

namespace std {

/**
 * Gets vector containing minimal values of @a a and @a b coordinates.
 * @return Vector of minimal coordinates.
 */
template<class T>
constexpr VEC2<T> min(const VEC2<T> &a, const VEC2<T> &b) {
  return VEC2<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y));
}

/**
 * Gets vector containing minimal values of @a a and @a b coordinates.
 * @return Vector of minimal coordinates.
 */
template<class T>
constexpr VEC3<T> min(const VEC3<T> &a, const VEC3<T> &b) {
  return VEC3<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y), ::std::min(a.z, b.z));
}

/**
 * Gets vector containing minimal values of @a a and @a b coordinates.
 * @return Vector of minimal coordinates.
 */
template<class T>
constexpr VEC4<T> min(const VEC4<T> &a, const VEC4<T> &b) {
  return VEC4<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y), ::std::min(a.z, b.z), ::std::min(a.w, b.w));
}

/**
 * Gets vector containing maximal values of @a a and @a b coordinates.
 * @return Vector of maximal coordinates.
 */
template<class T>
constexpr VEC2<T> max(const VEC2<T> &a, const VEC2<T> &b) {
  return VEC2<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y));
}

/**
 * Gets vector containing maximal values of @a a and @a b coordinates.
 * @return Vector of maximal coordinates.
 */
template<class T>
constexpr VEC3<T> max(const VEC3<T> &a, const VEC3<T> &b) {
  return VEC3<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y), ::std::max(a.z, b.z));
}

/**
 * Gets vector containing maximal values of @a a and @a b coordinates.
 * @return Vector of maximal coordinates.
 */
template<class T>
constexpr VEC4<T> max(const VEC4<T> &a, const VEC4<T> &b) {
  return VEC4<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y), ::std::max(a.z, b.z), ::std::max(a.w, b.w));
}
}

// cleanup shortcut defines
#undef VEC2
#undef VEC3
#undef VEC4

#ifdef VMATH_NAMESPACE
namespace VMATH_NAMESPACE {
#endif //VMATH_NAMESPACE

/**
 * Two-dimensional axis-aligned bounding-box (aka AABB) class.
 *
 * This class provides functionality for:
 * - creating an AABB from a point, or another AABB,
 * - testing if a point of other AABB intersects with it,
 * - getting result of intersection with other AABB,
 * - transforming AABB with 4x4 matrix.
 *
 * There are also overloaded couple of operators to shorten common operations.
 * For instance you can use  @c operator<< on AABB to extend it with a passed point or other AABB.
 * @code
 * Aabb2f aabb;
 * aabb << Vector2f(1, 2) << Aabb2f(-3,-3, 2, 2);
 * @endcode
 */
template<class T>
class Aabb2 {
  public:
    /**
     * Position of Min corner of bounding box.
     */
    Vector2<T> min;

    /**
     * Position of Max corner of bounding box
     */
    Vector2<T> max;

    /**
     * Constructs invalid axes-aligned bounding-box.
     * @see valid() for explanation of invalid bounding-box usage.
     */
    inline constexpr Aabb2() __attribute__((__always_inline__))
      : min(1, 1), max(-1, -1) {
    }

    /**
     * Constructs axes-aligned bound-box containing one point @a point
     * @param point
     */
    template<class SrcT>
    inline constexpr Aabb2(const Vector2<SrcT> &point)
      : min(point), max(point) {
    }

    /**
     * Constructs axes-aligned bounding-box form two corner points (@a x0, @a y0)
     * and (@a x1, @a y1)
     * @param x0 X-coordinate of first point
     * @param y0 Y-coordinate of first point
     * @param x1 X-coordinate of second point
     * @param y1 Y-coordinate of second point
     */
    template<class SrcT>
    inline constexpr Aabb2(SrcT x0, SrcT y0, SrcT x1, SrcT y1)
      : min(std::min(x0, x1), std::min(y0, y1)),
        max(std::max(x0, x1), std::max(y0, y1)) {
    }

    /**
     * Constructs axes-aligned bounding-box from @a min and @b max
     * @param min X-coordinate of first point
     * @param max X-coordinate of second point
     */
    template<class SrcT>
    inline constexpr Aabb2(Vector2<SrcT> const &min, Vector2<SrcT> const &max)
      : min(min),
        max(max) {
    }

    /**
     * Constructs axes-aligned bounding-box containing point (@a x, @a y)
     * @param x X-coordinate of point
     * @param y Y-coordinate of point
     */
    template<class SrcT>
    inline constexpr Aabb2(SrcT x, SrcT y)
      : min(x, y), max(x, y) {
    }

    /**
     * Creates copy of axis-aligned bounding-box
     * @param src Source bounding-box
     */
    template<class SrcT>
    inline constexpr Aabb2(const Aabb2<SrcT> &src)
      : min(src.min), max(src.max) {
    }

    /**
     * Assign operator
     * @param rhs source bounding-box
     * @return refenrence to this
     */
    template<class SrcT>
    inline Aabb2<T> &operator=(const Aabb2<SrcT> &rhs) {
      min = rhs.min;
      max = rhs.max;
      return *this;
    }

    /**
     * Checks if bounding-box is valid. Valid bounding-box has non-negative size.
     * If an invalid bounding-box is extended by point or another bounding-box, the target
     * bounding box becomes valid and contains solely the source point or bounding-box respectively.
     * @return True if box is valid, otherwise false
     */
    inline bool constexpr valid() const __attribute__((__always_inline__)) {
      return min.x <= max.x && min.y <= max.y;
    }

    /**
     * Makes this bounding-box invalid. So calling valid() gets false.
     * @see valid() method for more info on usage of invalid bounding-boxes.
     */
    inline void invalidate() __attribute__((__always_inline__)) {
      min = Vector2<T>( 1,  1);
      max = Vector2<T>(-1, -1);
    }

    /**
     * Extends this bounding-box by a point @a point.
     * @param point A point to extend bounding-box by.
     */
    template<class SrcT>
    inline void extend(const Vector2<SrcT> &point) {
      if(!valid()) {
        min = max = point;
      } else {
        min = std::min(min, point);
        max = std::max(max, point);
      }
    }

    /**
     * Extends this bounding-box by a box @a box.
     * @param box A box to extend this bounding-box by.
     */
    template<class SrcT>
    inline void extend(const Aabb2<SrcT> &box) {
      if(!valid()) {
        min = box.min;
        max = box.max;
      } else {
        min = std::min(min, box.min);
        max = std::max(max, box.max);
      }
    }

    /**
     * Gets a copy of this bounding-box extend by a point @a point.
     * @param point A point to extend the box by
     * @return Copy of extended bounding-box
     */
    template<class SrcT>
    inline constexpr Aabb2<T> extended(const Vector2<SrcT> &point) const {
      Aabb2<T> ret(*this);
      ret.extend(point);
      return ret;
    }

    /**
     * Gets a copy of this bounding-box extnended by box @a box.
     * @param box A box to extend the copy be.
     * @return Copy of extended bounding-box
     */
    template<class SrcT>
    inline constexpr Aabb2<T> extended(const Aabb2<SrcT> &box) const {
      Aabb2<T> ret(*this);
      ret.extend(box);
      return *this;
    }

    /**
     * Tests if the point @a point is within this bounding-box
     * @param point A point to be tested
     * @return True if point @a point lies within bounding-box, otherwise false.
     */
    template<class SrcT>
    inline bool constexpr intersects(const Vector2<SrcT> &point) const {
      return min.x <= point.x && point.x <= max.x &&
             min.y <= point.y && point.y <= max.y;
    }

    /**
     * Tests if other bounding-box @a box intersects (even partially) with this bounding-box.
     * @param box A box to be tested for intersection.
     * @return True if there's intersection between boxes, otherwise false.
     */
    template<class SrcT>
    inline bool constexpr intersects(const Aabb2<SrcT> &box) const {
      return max.x >= box.min.x && min.x <= box.max.x &&
             max.y >= box.min.y && min.y <= box.max.y;
    }

    /**
     * Gets result of intersection of this bounding-box with @a other bounding-box.
     * In case the boxes don't intersect, the returned bounding-box is invalid.
     * @param other Box to be tested
     * @return Result of intersection.
     * @see valid() method for more information on invalid bounding-boxes.
     */
    template<class SrcT>
    inline constexpr Aabb2<T> intersection(const Aabb2<SrcT> &other) const {
      return (max.x < other.min.x || min.x > other.max.x ||
              max.y < other.min.y || min.y > other.max.y) ? Aabb2<T>() : Aabb2<T>(std::max(min, other.min), std::min(max, other.max));
    }

    /**
     * Tests if a ray @b from origin @a intersects with this bounding-box.
     * @param origin Origin of the intersecting ray
     * @param ray The intersecting ray; does not need to be normalised
     * @return True if the ray intersects the box, otherwise false.
     */
    template<class SrcT>
    inline bool constexpr ray_intersects(Vector2<SrcT> const &ray, Vector2<SrcT> const &origin = Vector2<SrcT>()) const {
      // adapted from http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
      /*
      SrcT const dist_min_x = (min.x - origin.x) / ray.x;
      SrcT const dist_max_x = (max.x - origin.x) / ray.x;
      SrcT dist_min = std::min(dist_min_x, dist_max_x);
      SrcT dist_max = std::max(dist_min_x, dist_max_x);

      SrcT const dist_min_y = (min.y - origin.y) / ray.y;
      SrcT const dist_max_y = (max.y - origin.y) / ray.y;
      dist_min = std::max(dist_min, std::min(dist_min_y, dist_max_y));
      dist_max = std::min(dist_max, std::max(dist_min_y, dist_max_y));

      return dist_max >= std::max(dist_min, 0.0f);
      */
      // constexpr-compatible reformulation:
      return std::min(std::max((min.x - origin.x) / ray.x,
                               (max.x - origin.x) / ray.x),
                      std::max((min.y - origin.y) / ray.y,
                               (max.y - origin.y) / ray.y)) >=
             std::max(std::max(std::min((min.x - origin.x) / ray.x,
                                        (max.x - origin.x) / ray.x),
                               std::min((min.y - origin.y) / ray.y,
                                        (max.y - origin.y) / ray.y)), 0.0f);
    }

    /**
     * Gets center point of bounding-box.
     * @return A center point of bounding-box.
     */
    inline constexpr Vector2<T> center() const __attribute__((__always_inline__)) {
      return (min + max) * 0.5f;
    }

    /**
     * Gets extent of bounding-box.
     * @return Extent of bounding-box.
     */
    inline constexpr Vector2<T> extent() const __attribute__((__always_inline__)) {
      return (max - min) * 0.5f;
    }

    /**
     * Gets diagonal size of bounding-box
     * @return Sizes for particular dimensions.
     */
    inline constexpr Vector2<T> size() const __attribute__((__always_inline__)) {
      return max - min;
    }

    /**
     * Gets all 4 corner-points of bounding box
     * @param i An index of bounding-box corner point. Valid values are 0 .. 3.
     * @return A position of @a i-th corner-point.
     * @note The order of points is as follows (where @c + denotes max-point and @c - min-point):
     * 1. (@c + @c + @c +)
     * 2. (@c - @c + @c +)
     * 3. (@c + @c - @c +)
     * 4. (@c - @c - @c +)
     *
     */
    inline constexpr Vector2<T> point(unsigned int i) const __attribute__((__always_inline__)) {
      return Vector2<T>(i & 1 ? min.x : max.x,
                        i & 2 ? min.y : max.y);
    }

    //-------------------------------------------------------------------------------------------------------------
    // operators
    //-------------------------------------------------------------------------------------------------------------
    /**
     * Tests if @a rhs is equal to this bounding-box
     * @param rhs Right-hand side
     * @return True if @a rhs and this bounding-boxes are equal, otherwise false
     */
    template<class RhsT>
    inline bool constexpr operator==(const Aabb2<RhsT> &rhs) const {
      return min == rhs.min && max == rhs.max;
    }

    /**
     * Tests if @a rhs is not equal to this bounding-box
     * @param rhs Right-hand side
     * @return True if @a rhs and this bounding-boxes are not equal, otherwise false
     */
    template<class RhsT>
    inline bool constexpr operator!=(const Aabb2<RhsT> &rhs) const {
      return min != rhs.min || max != rhs.max;
    }

    /**
     * Extends this bounding-box by point @a rhs.
     * @param rhs A point to extend this bounding-box by
     * @return Reference to this
     */
    template<class SrcT>
    inline Aabb2<T> &operator<<(const Vector2<SrcT> &rhs) {
      extend(rhs);
      return *this;
    }

    /**
     * Extends this bounding-box by box @a rhs.
     * @param rhs A box to extend this bounding-box by
     * @return Reference to this
     */
    template<class SrcT>
    inline Aabb2<T> &operator<<(const Aabb2<SrcT> &rhs) {
      extend(rhs);
      return *this;
    }

    /**
     * Union of this and @a rhs bounding-boxes
     * @param rhs Right-hand side of union
     * @return A resulting bounding-box representing union
     */
    template<class RhsT>
    inline constexpr Aabb2<T> operator|(const Aabb2<RhsT> &rhs) const {
      return extended(rhs);
    }

    /**
     * Intersection of this and @a rhs bounding-boxed
     * @param rhs Right-hand side
     * @return Resulting bounding-box representing the intersection.
     */
    template<class RhsT>
    inline constexpr Aabb2<T> operator&(const Aabb2<RhsT> &rhs) const {
      return intersection(rhs);
    }

    /**
     * Outputs string representation of bounding-box @a rhs to output stream @a lhs
     * @param lhs Output stream to write to
     * @param rhs Bounding-box to write to output stream.
     * @return Reference to output stream @a lhs
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, const Aabb2<T> &rhs) __attribute__((__always_inline__)) {
      lhs << rhs.min << " x " << rhs.max;
      return lhs;
    }
};

/**
 * Three-dimensional axis-aligned bounding-box (aka AABB) class.
 *
 * This class provides functionality for:
 * - creating an AABB from a point, or another AABB,
 * - testing if a point of other AABB intersects with it,
 * - getting result of intersection with other AABB,
 * - transforming AABB with 4x4 matrix.
 *
 * There are also overloaded couple of operators to shorten common operations.
 * For instance you can use  @c operator<< on AABB to extend it with a passed point or other AABB.
 * @code
 * Aabb3f aabb;
 * aabb << Vector3f(1, 1, 2) << Aabb3f(-3,-3,-3, 2, 2, 2);
 * @endcode
 */
template<class T>
class Aabb3 {
  public:
    /**
     * Position of Min corner of bounding box.
     */
    Vector3<T> min;

    /**
     * Position of Max corner of bounding box
     */
    Vector3<T> max;

    /**
     * Constructs invalid axes-aligned bounding-box.
     * @see valid() for explanation of invalid bounding-box usage.
     */
    inline constexpr Aabb3() __attribute__((__always_inline__))
      : min(1, 1, 1), max(-1, -1, -1) {
    }

    /**
     * Constructs axes-aligned bound-box containing one point @a point
     * @param point
     */
    template<class SrcT>
    inline constexpr Aabb3(const Vector3<SrcT> &point)
      : min(point), max(point) {
    }

    /**
     * Constructs axes-aligned bounding-box from two corner points (@a x0, @a y0, @a z0)
     * and (@a x1, @a y1, @a z1)
     * @param x0 X-coordinate of first point
     * @param y0 Y-coordinate of first point
     * @param z0 Z-coordinate of first point
     * @param x1 X-coordinate of second point
     * @param y1 Y-coordinate of second point
     * @param z1 Z-coordinate of second point
     */
    template<class SrcT>
    inline constexpr Aabb3(SrcT x0, SrcT y0, SrcT z0, SrcT x1, SrcT y1, SrcT z1)
      : min(std::min(x0, x1), std::min(y0, y1), std::min(z0, z1)),
        max(std::max(x0, x1), std::max(y0, y1), std::max(z0, z1)) {
    }

    /**
     * Constructs axes-aligned bounding-box from @a min and @b max
     * @param min X-coordinate of first point
     * @param max X-coordinate of second point
     */
    template<class SrcT>
    inline constexpr Aabb3(Vector3<SrcT> const &min, Vector3<SrcT> const &max)
      : min(min),
        max(max) {
    }

    /**
     * Constructs axes-aligned bounding-box containing point (@a x, @a y, @a z)
     * @param x X-coordinate of point
     * @param y Y-coordinate of point
     * @param z Z-coordinate of point
     */
    template<class SrcT>
    inline constexpr Aabb3(SrcT x, SrcT y, SrcT z)
      : min(x, y, z), max(x, y, z) {
    }

    /**
     * Creates copy of axis-aligned bounding-box
     * @param src Source bounding-box
     */
    template<class SrcT>
    inline constexpr Aabb3(const Aabb3<SrcT> &src)
      : min(src.min), max(src.max) {
    }

    /**
     * Assign operator
     * @param rhs source bounding-box
     * @return refenrence to this
     */
    template<class SrcT>
    inline Aabb3<T> &operator=(const Aabb3<SrcT> &rhs) {
      min = rhs.min;
      max = rhs.max;
      return *this;
    }

    /**
     * Checks if bounding-box is valid. Valid bounding-box has non-negative size.
     * If an invalid bounding-box is extended by point or another bounding-box, the target
     * bounding box becomes valid and contains solely the source point or bounding-box respectively.
     * @return True if box is valid, otherwise false
     */
    inline bool constexpr valid() const __attribute__((__always_inline__)) {
      return min.x <= max.x && min.y <= max.y && min.z <= max.z;
    }

    /**
     * Makes this bounding-box invalid. So calling valid() gets false.
     * @see valid() method for more info on usage of invalid bounding-boxes.
     */
    inline void invalidate() __attribute__((__always_inline__)) {
      min = Vector3<T>(1, 1, 1);
      max = Vector3<T>(-1, -1, -1);
    }

    /**
     * Extends this bounding-box by a point @a point.
     * @param point A point to extend bounding-box by.
     */
    template<class SrcT>
    inline void extend(const Vector3<SrcT> &point) {
      if(!valid()) {
        min = max = point;
      } else {
        min = std::min(min, point);
        max = std::max(max, point);
      }
    }

    /**
     * Extends this bounding-box by a box @a box.
     * @param box A box to extend this bounding-box by.
     */
    template<class SrcT>
    inline void extend(const Aabb3<SrcT> &box) {
      if(!valid()) {
        min = box.min;
        max = box.max;
      } else {
        min = std::min(min, box.min);
        max = std::max(max, box.max);
      }
    }

    /**
     * Gets a copy of this bounding-box extend by a point @a point.
     * @param point A point to extend the box by
     * @return Copy of extended bounding-box
     */
    template<class SrcT>
    inline constexpr Aabb3<T> extended(const Vector3<SrcT> &point) const {
      Aabb3<T> ret(*this);
      ret.extend(point);
      return ret;
    }

    /**
     * Gets a copy of this bounding-box extnended by box @a box.
     * @param box A box to extend the copy be.
     * @return Copy of extended bounding-box
     */
    template<class SrcT>
    inline constexpr Aabb3<T> extended(const Aabb3<SrcT> &box) const {
      Aabb3<T> ret(*this);
      ret.extend(box);
      return *this;
    }

    /**
     * Tests if the point @a point is within this bounding-box
     * @param point A point to be tested
     * @return True if point @a point lies within bounding-box, otherwise false.
     */
    template<class SrcT>
    inline bool constexpr intersects(const Vector3<SrcT> &point) const {
      return min.x <= point.x && point.x <= max.x &&
             min.y <= point.y && point.y <= max.y &&
             min.z <= point.z && point.z <= max.z;
    }

    /**
     * Tests if other bounding-box @a box intersects (even partially) with this bounding-box.
     * @param box A box to be tested for intersection.
     * @return True if there's intersection between boxes, otherwise false.
     */
    template<class SrcT>
    inline bool constexpr intersects(const Aabb3<SrcT> &box) const {
      return max.x >= box.min.x && min.x <= box.max.x &&
             max.y >= box.min.y && min.y <= box.max.y &&
             max.z >= box.min.z && min.z <= box.max.z;
    }

    /**
     * Gets result of intersection of this bounding-box with @a other bounding-box.
     * In case the boxes don't intersect, the returned bounding-box is invalid.
     * @param other Box to be tested
     * @return Result of intersection.
     * @see valid() method for more information on invalid bounding-boxes.
     */
    template<class SrcT>
    inline constexpr Aabb3<T> intersection(const Aabb3<SrcT> &other) const {
      return (max.x < other.min.x || min.x > other.max.x ||
              max.y < other.min.y || min.y > other.max.y ||
              max.z < other.min.z || min.z > other.max.z) ? Aabb3<T>() : Aabb3<T>(std::max(min, other.min), std::min(max, other.max));
    }

    /**
     * Tests if a ray @b from origin @a intersects with this bounding-box.
     * @param ray The intersecting ray; does not need to be normalised
     * @param origin Origin of the intersecting ray
     * @return True if the ray intersects the box, otherwise false.
     */
    template<class SrcT>
    inline bool constexpr ray_intersects(Vector3<SrcT> const &ray, Vector3<SrcT> const &origin = Vector3<SrcT>()) const {
      // adapted from http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
      /*
      SrcT const dist_min_x = (min.x - origin.x) / ray.x;
      SrcT const dist_max_x = (max.x - origin.x) / ray.x;
      SrcT dist_min = std::min(dist_min_x, dist_max_x);
      SrcT dist_max = std::max(dist_min_x, dist_max_x);

      SrcT const dist_min_y = (min.y - origin.y) / ray.y;
      SrcT const dist_max_y = (max.y - origin.y) / ray.y;
      dist_min = std::max(dist_min, std::min(dist_min_y, dist_max_y));
      dist_max = std::min(dist_max, std::max(dist_min_y, dist_max_y));

      SrcT const dist_min_z = (min.z - origin.z) / ray.z;
      SrcT const dist_max_z = (max.z - origin.z) / ray.z;
      dist_min = std::max(dist_min, std::min(dist_min_z, dist_max_z));
      dist_max = std::min(dist_max, std::max(dist_min_z, dist_max_z));

      return dist_max >= std::max(dist_min, 0.0f);
      */
      // constexpr-compatible reformulation:
      return std::min(std::min(std::max((min.x - origin.x) / ray.x,
                                        (max.x - origin.x) / ray.x),
                               std::max((min.y - origin.y) / ray.y,
                                        (max.y - origin.y) / ray.y)),
                      std::max((min.z - origin.z) / ray.z,
                               (max.z - origin.z) / ray.z)) >=
             std::max(std::max(std::max(std::min((min.x - origin.x) / ray.x,
                                                 (max.x - origin.x) / ray.x),
                                        std::min((min.y - origin.y) / ray.y,
                                                 (max.y - origin.y) / ray.y)),
                               std::min((min.z - origin.z) / ray.z,
                                        (max.z - origin.z) / ray.z)), 0.0f);
    }

    /**
     * Gets center point of bounding-box.
     * @return A center point of bounding-box.
     */
    inline constexpr Vector3<T> center() const __attribute__((__always_inline__)) {
      return (min + max) * 0.5f;
    }

    /**
     * Gets extent of bounding-box.
     * @return Extent of bounding-box.
     */
    inline constexpr Vector3<T> extent() const __attribute__((__always_inline__)) {
      return (max - min) * 0.5f;
    }

    /**
     * Gets diagonal size of bounding-box
     * @return Sizes for particular dimensions.
     */
    inline constexpr Vector3<T> size() const __attribute__((__always_inline__)) {
      return max - min;
    }

    /**
     * Gets all 8 corner-points of bounding box
     * @param i An index of bounding-box corner point. Valid values are 0 .. 7.
     * @return A position of @a i-th corner-point.
     * @note The order of points is as follows (where @c + denotes max-point and @c - min-point):
     * 1. (@c + @c + @c +)
     * 2. (@c - @c + @c +)
     * 3. (@c + @c - @c +)
     * 4. (@c - @c - @c +)
     * 5. (@c + @c + @c -)
     * 6. (@c - @c + @c -)
     * 7. (@c + @c - @c -)
     * 8. (@c - @c - @c -)
     *
     */
    inline constexpr Vector3<T> point(unsigned int i) const __attribute__((__always_inline__)) {
      return Vector3<T>(i & 1 ? min.x : max.x, i & 2 ? min.y : max.y, i & 4 ? min.z : max.z);
    }

    /**
     * Gets transformed bounding-box by transform @a t
     * @param t A transform matrix
     * @return Transformed bounding-box
     */
    inline Aabb3<T> transformed(const Matrix4<T> &t) const __attribute__((__always_inline__)) {
      Aabb3<T> ret;
      for(unsigned int i = 0; i != 8; ++i) {
        const Vector4<T> p(point(i), 1);
        ret.extend((t * p).xyz());
      }
      return ret;
    }

    //-------------------------------------------------------------------------------------------------------------
    // operators
    //-------------------------------------------------------------------------------------------------------------
    /**
     * Tests if @a rhs is equal to this bounding-box
     * @param rhs Right-hand side
     * @return True if @a rhs and this bounding-boxes are equal, otherwise false
     */
    template<class RhsT>
    inline bool constexpr operator==(const Aabb3<RhsT> &rhs) const {
      return min == rhs.min && max == rhs.max;
    }

    /**
     * Tests if @a rhs is not equal to this bounding-box
     * @param rhs Right-hand side
     * @return True if @a rhs and this bounding-boxes are not equal, otherwise false
     */
    template<class RhsT>
    inline bool constexpr operator!=(const Aabb3<RhsT> &rhs) const {
      return min != rhs.min || max != rhs.max;
    }

    /**
     * Gets transformed bounding-box by transform @a rhs.
     * @param rhs Matrix 4x4 representing the transform
     * @return Transformed bounding-box
     */
    inline constexpr Aabb3<T> operator*(const Matrix4<T> &rhs) const __attribute__((__always_inline__)) {
      return transformed(rhs);
    }

    /**
     * Apply transform @a rhs to this bounding-box
     * @param rhs A transform to be applied
     * @return Reference to this
     */
    inline Aabb3<T> &operator*=(const Matrix4<T> &rhs) __attribute__((__always_inline__)) {
      *this = transformed(rhs);
      return *this;
    }


    /**
     * Extends this bounding-box by point @a rhs.
     * @param rhs A point to extend this bounding-box by
     * @return Reference to this
     */
    template<class SrcT>
    inline Aabb3<T> &operator<<(const Vector3<SrcT> &rhs) {
      extend(rhs);
      return *this;
    }

    /**
     * Extends this bounding-box by box @a rhs.
     * @param rhs A box to extend this bounding-box by
     * @return Reference to this
     */
    template<class SrcT>
    inline Aabb3<T> &operator<<(const Aabb3<SrcT> &rhs) {
      extend(rhs);
      return *this;
    }

    /**
     * Union of this and @a rhs bounding-boxes
     * @param rhs Right-hand side of union
     * @return A resulting bounding-box representing union
     */
    template<class RhsT>
    inline constexpr Aabb3<T> operator|(const Aabb3<RhsT> &rhs) const {
      return extended(rhs);
    }

    /**
     * Intersection of this and @a rhs bounding-boxed
     * @param rhs Right-hand side
     * @return Resulting bounding-box representing the intersection.
     */
    template<class RhsT>
    inline constexpr Aabb3<T> operator&(const Aabb3<RhsT> &rhs) const {
      return intersection(rhs);
    }

    /**
     * Outputs string representation of bounding-box @a rhs to output stream @a lhs
     * @param lhs Output stream to write to
     * @param rhs Bounding-box to write to output stream.
     * @return Reference to output stream @a lhs
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, const Aabb3<T> &rhs) __attribute__((__always_inline__)) {
      lhs << rhs.min << " x " << rhs.max;
      return lhs;
    }
};

/// 2D axis-aligned bounding box of floats
using Aabb2f = Aabb2<float>;
/// 2D axis-aligned bounding box of doubles
using Aabb2d = Aabb2<double>;
/// 2D axis-aligned bounding box of long doubles
using Aabb2ld = Aabb2<long double>;
/// 2D axis-aligned bounding box of integers
using Aabb2i = Aabb2<int>;

/// 3D axis-aligned bounding box of floats
using Aabb3f = Aabb3<float>;
/// 3D axis-aligned bounding box of doubles
using Aabb3d = Aabb3<double>;
/// 3D axis-aligned bounding box of long doubles
using Aabb3ld = Aabb3<long double>;
/// 2D axis-aligned bounding box of integers
using Aabb3i = Aabb3<int>;

#ifdef VMATH_NAMESPACE
}
#endif //VMATH_NAMESPACE


#endif // __vmath_Header_File__

