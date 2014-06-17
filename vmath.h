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

// Modified 2013-2014, Eugene Hopkinson for VoxelStorm Ltd
//                     various expansions and conversions
//                     Version 2.0: C++11 specific optimisations, including constexpr

#ifndef __vmath_Header_File__
#define __vmath_Header_File__

#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#ifdef VMATH_NAMESPACE
namespace VMATH_NAMESPACE {
#endif

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

#define DEG2RAD(x) ((x * M_PI) / 180.0)
//#define EPSILON (4.37114e-07)

const double epsilon = 4.37114e-05;
#define EPSILON epsilon

template<class T> class Vector2;  // forward declarations
template<class T> class Vector3;
template<class T> class Vector4;
template<class T> class Matrix3;
template<class T> class Matrix4;
template<class T> class Quaternion;

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
    inline constexpr Vector2()
      : x(0), y(0) {
    }

    /**
     * Creates and sets to (x,y)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     */
    inline constexpr Vector2(T nx, T ny)
      : x(nx), y(ny) {
    }

    /**
     * Copy constructor.
     * @param src Source of data for new created instance.
     */
    inline constexpr Vector2(Vector2<T> const &src)
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
    inline void assign(T nx = 0, T ny = 0) {
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
    inline Vector2<T> &operator=(Vector2<T> const &rhs) {
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
    inline T &operator[](int n) {
      assert(n >= 0 && n <= 1);
      if(0 == n)
        return x;
      else
        return y;
    }

    /**
     * Constant array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, else reference to y
     * y coordinate.
     */
    inline const T &operator[](int n) const {
      assert(n >= 0 && n <= 1);
      if(0 == n)
        return x;
      else
        return y;
    }

    //---------------[ vector aritmetic operator ]--------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator+(Vector2<T> const &rhs) const {
      return Vector2<T>(x + rhs.x, y + rhs.y);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator-(Vector2<T> const &rhs) const {
      return Vector2<T>(x - rhs.x, y - rhs.y);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator*(Vector2<T> const &rhs) const {
      return Vector2<T>(x * rhs.x, y * rhs.y);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator/(Vector2<T> const &rhs) const {
      return Vector2<T>(x / rhs.x, y / rhs.y);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator+=(Vector2<T> const &rhs) {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }

    /**
     * Substraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator-=(Vector2<T> const &rhs) {
      x -= rhs.x;
      y -= rhs.y;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator*=(Vector2<T> const &rhs) {
      x *= rhs.x;
      y *= rhs.y;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator/=(Vector2<T> const &rhs) {
      x /= rhs.x;
      y /= rhs.y;
      return *this;
    }

    /**
     * Dot product of two vectors.
     * @param rhs Right hand side argument of binary operator.
     */
    inline T constexpr dotProduct(Vector2<T> const &rhs) const {
      return x * rhs.x + y * rhs.y;
    }

    /**
     * Cross product of two vectors
     * @param rhs Right hand side argument of binary operator.
     */
    inline T constexpr crossProduct(Vector2<T> const &rhs) const {
      return x * rhs.y - y * rhs.x;
    }

    //--------------[ scalar vector operator ]--------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator+(T rhs) const {
      return Vector2<T>(x + rhs, y + rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator-(T rhs) const {
      return Vector2<T>(x - rhs, y - rhs);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator*(T rhs) const {
      return Vector2<T>(x * rhs, y * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> constexpr operator/(T rhs) const {
      return Vector2<T>(x / rhs, y / rhs);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator+=(T rhs) {
      x += rhs;
      y += rhs;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator-=(T rhs) {
      x -= rhs;
      y -= rhs;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator*=(T rhs) {
      x *= rhs;
      y *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector2<T> &operator/=(T rhs) {
      x /= rhs;
      y /= rhs;
      return *this;
    }

    //--------------[ equality operator ]------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold EPSILON value. To be two
     * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
     * same for y-coordinate.
     */
    inline constexpr bool operator==(Vector2<T> const &rhs) const {
      return (std::abs(x - rhs.x) < EPSILON) && (std::abs(y - rhs.y) < EPSILON);
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline constexpr bool operator!=(Vector2<T> const &rhs) const {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated vector
     */
    inline Vector2<T> constexpr operator-() const {
      return Vector2<T>(-x, -y);
    }

    //-------------[ size operations ]---------------------------
    /**
     * Get length of vector.
     * @return length of vector
     */
    inline T constexpr length() const {
      return (T) std::sqrt(x * x + y * y);
    }

    /**
     * Normalize vector
     */
    inline void normalize() {
      T s = length();
      x /= s;
      y /= s;
    }
    inline void normalise() {  // proper english
      normalize();
    }
    inline Vector2<T> constexpr normalize_copy() const {
      return Vector2<T>(x / length(), y / length());
    }
    inline Vector2<T> constexpr normalise_copy() const {  // proper english
      return normalize_copy();
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two vector can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const {
      return x * x + y * y;
    }

    //-------------[ other operations ]--------------------------
    /**
     * Rotate 2D vector clockwise.
     * @param aa Angle (in degrees) to be rotated.
     */
    inline void rotate(T aa) {
      T a = cos(DEG2RAD(-aa));
      T b = sin(DEG2RAD(-aa));
      T nx = x * a - y * b;
      T ny = x * b + y * a;
      x = nx;
      y = ny;
    }
    inline void rotate_rad(T aa) {
      T a = cos(-aa);
      T b = sin(-aa);
      T nx = x * a - y * b;
      T ny = x * b + y * a;
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
    inline Vector2<T> constexpr lerp(T fact, Vector2<T> const &r) const {
      return (*this) + (r - (*this)) * fact;
    }

    //-------------[ conversion ]-----------------------------
    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector2<T>)
     * used for passing Vector2<T> values to gl*2[fd] functions.
     */
    inline constexpr operator T*() {
      return (T*)this;
    }
    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Vector2<T>)
     * used for passing Vector2<T> values to gl*2[fd] functions.
     */
    inline constexpr operator const T*() const {
      return (T const*)this;
    }

    //-------------[ output operator ]------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector2<T> const &rhs) {
      lhs << "[" << rhs.x << "," << rhs.y << "]";
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline constexpr std::string toString() const {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

  /**
   * Free function to check whether two lines intersect, and if so
   * obtain the intersection point and store it in this vector.
   * @param line1start The start coordinates of the first line
   * @param line1end The end coordinates of the first line
   * @param line2start The start coordinates of the first line
   * @param line2end The end coordinates of the first line
   */
  inline bool get_line_intersection(Vector2<T> const &line1start,
                                    Vector2<T> const &line1end,
                                    Vector2<T> const &line2start,
                                    Vector2<T> const &line2end) {
    Vector2<T> s1(line1end.x - line1start.x,
                  line1end.y - line1start.y);
    Vector2<T> s2(line2end.x - line2start.x,
                  line2end.y - line2start.y);
    T s = (-s1.y * (line1start.x - line2start.x) + s1.x * (line1start.y - line2start.y)) / (-s2.x * s1.y + s1.x * s2.y);
    T t = ( s2.x * (line1start.y - line2start.y) - s2.y * (line1start.x - line2start.x)) / (-s2.x * s1.y + s1.x * s2.y);

    if(s >= 0 && s <= 1 && t >= 0 && t <= 1) {
      // Collision detected
      x = line1start.x + (t * s1.x);
      y = line1start.y + (t * s1.y);
      return true;
    }
    return false;
  }
};

//--------------------------------------
// Typedef shortcuts for 2D vector
//-------------------------------------
/// Two dimensional Vector of floats
typedef class Vector2<float> Vector2f;
/// Two dimensional Vector of doubles
typedef class Vector2<double> Vector2d;
/// Two dimensional Vector of long doubles
typedef class Vector2<long double> Vector2ld;
/// Two dimensional Vector of ints
typedef class Vector2<int> Vector2i;

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
    inline constexpr Vector3()
      : x(0), y(0), z(0) {
    }

    /**
     * Creates and sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     */
    inline constexpr Vector3(T nx, T ny, T nz)
      : x(nx), y(ny), z(nz) {
    }

    /**
     * Copy constructor.
     * @param src Source of data for new created Vector3 instance.
     */
    inline constexpr Vector3(Vector3<T> const &src)
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

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     */
    inline void assign(T nx = 0, T ny = 0, T nz = 0) {
      x = nx;
      y = ny;
      z = nz;
    }

    //----------------[ access operators ]-------------------
    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> operator=(Vector3<T> const &rhs) {
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
    inline Vector3<T> operator=(Vector4<T> const &rhs) {
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
    inline T &operator[](int n) {
      assert(n >= 0 && n <= 2);
      if(0 == n)
        return x;
      else if(1 == n)
        return y;
      else
        return z;
    }

    /**
     * Constant array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, n = 1
     * reference to y, else reference to z
     * y coordinate.
     */
    inline const T &operator[](int n) const {
      assert(n >= 0 && n <= 2);
      if(0 == n)
        return x;
      else if(1 == n)
        return y;
      else
        return z;
    }

    //---------------[ vector arithmetic operator ]--------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator+(Vector3<T> const &rhs) const {
      return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator-(Vector3<T> const &rhs) const {
      return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const {
      return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator/(Vector3<T> const &rhs) const {
      return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator+=(Vector3<T> const &rhs) {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator-=(Vector3<T> const &rhs) {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator*=(Vector3<T> const &rhs) {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator/=(Vector3<T> const &rhs) {
      x /= rhs.x;
      y /= rhs.y;
      z /= rhs.z;
      return *this;
    }

    /**
     * Dot product of two vectors.
     * @param rhs Right hand side argument of binary operator.
     */
    inline T constexpr dotProduct(Vector3<T> const &rhs) const {
      return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    /**
     * Cross product of two vectors
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr crossProduct(Vector3<T> const &rhs) const {
      return Vector3<T>(y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
    }

    //--------------[ rotation with quaternions ]-----------------
    // see http://mollyrocket.com/forums/viewtopic.php?t=833&sid=3a84e00a70ccb046cfc87ac39881a3d0
    /**
     * Multiplication by quaternion operator (rotation by quaternion)
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Quaternion<T> const &rhs) const {
      return *this + (rhs.v.crossProduct(*this) * 2 * rhs.w) + rhs.v.crossProduct(rhs.v.crossProduct(*this) * 2);
    }

    /**
     * Multiplication by quaternion operator (rotation by quaternion)
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator*=(Quaternion<T> const &rhs) {
      Vector3<T> const temp = rhs.v.crossProduct(*this) * 2;
      *this += (temp * rhs.w) + rhs.v.crossProduct(temp);
      return *this;
    }

    //--------------[ scalar vector operator ]--------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator+(T rhs) const {
      return Vector3<T>(x + rhs, y + rhs, z + rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator-(T rhs) const {
      return Vector3<T>(x - rhs, y - rhs, z - rhs);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(T rhs) const {
      return Vector3<T>(x * rhs, y * rhs, z * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator/(T rhs) const {
      return Vector3<T>(x / rhs, y / rhs, z / rhs);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator+=(T rhs) {
      x += rhs;
      y += rhs;
      z += rhs;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator-=(T rhs) {
      x -= rhs;
      y -= rhs;
      z -= rhs;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator*=(T rhs) {
      x *= rhs;
      y *= rhs;
      z *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> &operator/=(T rhs) {
      x /= rhs;
      y /= rhs;
      z /= rhs;
      return *this;
    }

    //-------------[ equality operator ]-------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold EPSILON value. To be two
     * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
     * same for y-coordinate, and z-coordinate.
     */
    inline bool constexpr operator==(Vector3<T> const &rhs) const {
      return std::fabs(x - rhs.x) < EPSILON && std::fabs(y - rhs.y) < EPSILON && std::fabs(z - rhs.z) < EPSILON;
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Vector3<T> const &rhs) const {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated vector
     */
    inline Vector3<T> constexpr operator-() const {
      return Vector3<T>(-x, -y, -z);
    }

    //-------------[ size operations ]---------------------------
    /**
     * Get length of vector.
     * @return length of vector
     */
    inline T constexpr length() const {
      return (T) std::sqrt(x * x + y * y + z * z);
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two vector can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const {
      return x * x + y * y + z * z;
    }

    /**
     * Normalize vector
     */
    inline void normalize() {
      T const s = length();
      x /= s;
      y /= s;
      z /= s;
    }
    inline void normalise() {  // proper english
      normalize();
    }
    inline Vector3<T> constexpr normalize_copy() const {
      return Vector3<T>(x / length(), y / length(), z / length());
    }
    inline Vector3<T> constexpr normalise_copy() const {  // proper english
      return normalize_copy();
    }

    //-------------[ other operations ]--------------------------
    /**
     * Rotate vector around three axis.
     * @param ax Angle (in degrees) to be rotated around X-axis.
     * @param ay Angle (in degrees) to be rotated around Y-axis.
     * @param az Angle (in degrees) to be rotated around Z-axis.
     */
    inline void rotate(T ax, T ay, T az) {
      T a = cos(DEG2RAD(ax));
      T b = sin(DEG2RAD(ax));
      T c = cos(DEG2RAD(ay));
      T d = sin(DEG2RAD(ay));
      T e = cos(DEG2RAD(az));
      T f = sin(DEG2RAD(az));
      T nx = c * e * x - c * f * y + d * z;
      T ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
      T nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
      x = nx;
      y = ny;
      z = nz;
    }
    inline void rotate_rad(T ax, T ay, T az) {
      T a = cos(ax);
      T b = sin(ax);
      T c = cos(ay);
      T d = sin(ay);
      T e = cos(az);
      T f = sin(az);
      T nx = c * e * x - c * f * y + d * z;
      T ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
      T nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
      x = nx;
      y = ny;
      z = nz;
    }
    /**
     * Rotate vector by a quaternion.
     * @param ax Quaternion to rotate by.
     */
    inline void rotate(Quaternion<T> const &rhs) {
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
    inline Vector3<T> constexpr lerp(T fact, Vector3<T> const &r) const {
      return (*this) + (r - (*this)) * fact;
    }

    //-------------[ conversion ]--------------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector3<T>)
     * used for passing Vector3<T> values to gl*3[fd] functions.
     */
    inline constexpr operator T*() {
      return (T*)this;
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Vector3<T>)
     * used for passing Vector3<T> values to gl*3[fd] functions.
     */
    inline constexpr operator const T*() const {
      return (T const*)this;
    }

    //-------------[ output ]------------------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector3<T> const &rhs) {
      lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "]";
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

    /**
     * Gets a 2D vector equivalent using the X and Y axes
     */
    inline Vector2<T> constexpr to_2D_XY() const {
      return Vector2<T>(x, y);
    }

    /**
     * Gets a 2D vector equivalent using the X and Z axes
     */
    inline Vector2<T> constexpr to_2D_XZ() const {
      return Vector2<T>(x, z);
    }
};

/// Three dimensional Vector of floats
typedef Vector3<float> Vector3f;
/// Three dimensional Vector of doubles
typedef Vector3<double> Vector3d;
/// Three dimensional Vector of long doubles
typedef Vector3<long double> Vector3ld;
/// Three dimensional Vector of ints
typedef Vector3<int> Vector3i;

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
    inline constexpr Vector4(T nx, T ny, T nz, T nw)
      : x(nx), y(ny), z(nz), w(nw) {
    }

    /**
     * Copy constructor.
     * @param src Source of data for new created Vector4 instance.
     */
    inline constexpr Vector4(Vector4<T> const &src)
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

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     * @param nz initial w-coordinate value
     */
    inline void assign(T nx = 0, T ny = 0, T nz = 0, T nw = 0) {
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
    inline Vector4<T> operator=(Vector4<T> const &rhs) {
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
    inline Vector4<T> operator=(Vector3<T> const &rhs) {
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
    inline T &operator[](int n) {
      assert(n >= 0 && n <= 3);
      if(0 == n)
        return x;
      else if(1 == n)
        return y;
      else if(2 == n)
        return z;
      else
        return w;
    }

    /**
     * Array access operator
     * @param n Array index
     * @return For n = 0, reference to x coordinate, n = 1
     * reference to y coordinate, n = 2 reference to z,
     * else reference to w coordinate.
     */
    inline const T &operator[](int n) const {
      assert(n >= 0 && n <= 3);
      if(0 == n)
        return x;
      else if(1 == n)
        return y;
      else if(2 == n)
        return z;
      else
        return w;
    }

    //---------------[ vector aritmetic operator ]--------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator+(Vector4<T> const &rhs) const {
      return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator-(Vector4<T> const &rhs) const {
      return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(Vector4<T> const &rhs) const {
      return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator/(Vector4<T> const &rhs) const {
      return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator+=(Vector4<T> const &rhs) {
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
    inline Vector4<T> &operator-=(Vector4<T> const &rhs) {
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
    inline Vector4<T> &operator*=(Vector4<T> const &rhs) {
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
    inline Vector4<T> &operator/=(Vector4<T> const &rhs) {
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
     * @note Test of equality is based of threshold EPSILON value. To be two
     * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
     * same for y-coordinate, z-coordinate, and w-coordinate.
     */
    inline bool constexpr operator==(Vector4<T> const &rhs) const {
      return std::fabs(x - rhs.x) < EPSILON && std::fabs(y - rhs.y) < EPSILON && std::fabs(z - rhs.z) < EPSILON
             && std::fabs(w - rhs.w) < EPSILON;
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Vector4<T> const &rhs) const {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated vector
     */
    inline Vector4<T> constexpr operator-() const {
      return Vector4<T>(-x, -y, -z, -w);
    }

    //--------------[ scalar vector operator ]--------------------

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator+(T rhs) const {
      return Vector4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator-(T rhs) const {
      return Vector4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(T rhs) const {
      return Vector4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator/(T rhs) const {
      return Vector4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> &operator+=(T rhs) {
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
    inline Vector4<T> &operator-=(T rhs) {
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
    inline Vector4<T> &operator*=(T rhs) {
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
    inline Vector4<T> &operator/=(T rhs) {
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
    inline T constexpr length() const {
      return (T) std::sqrt(x * x + y * y + z * z + w * w);
    }

    /**
     * Normalize vector
     */
    inline void normalize() {
      T s = length();
      x /= s;
      y /= s;
      z /= s;
      w /= s;
    }
    inline void normalise() {  // proper english
      normalize();
    }
    inline Vector4<T> constexpr normalize_copy() const {
      return Vector4<T>(x / length(), y / length(), z / length(), w / length());
    }
    inline Vector4<T> constexpr normalise_copy() const {  // proper english
      return normalize_copy();
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two vector can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const {
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
    inline Vector4<T> constexpr lerp(T fact, Vector4<T> const &r) const {
      return (*this) + (r - (*this)) * fact;
    }

    //-------------[ conversion ]-----------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector4<T>)
     * used for passing Vector4<T> values to gl*4[fd] functions.
     */
    inline constexpr operator T*() {
      return (T*)this;
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Vector4<T>)
     * used for passing Vector4<T> values to gl*4[fd] functions.
     */
    inline constexpr operator const T*() const {
      return (T const*)this;
    }

    //-------------[ output operator ]------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector4<T> const &rhs) {
      lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << "]";
      return lhs;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

};

/// Four dimensional Vector of floats
typedef Vector4<float> Vector4f;
/// Four dimensional Vector of doubles
typedef Vector4<double> Vector4d;
/// Four dimensional Vector of long doubles
typedef Vector4<long double> Vector4ld;
/// Four dimensional Vector of ints
typedef Vector4<int> Vector4i;

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
    T data[9];

    //--------------------------[ constructors ]-------------------------------
    /**
     * Creates identity matrix
     */
    inline Matrix3() {
      for(int i = 0; i != 9; ++i) {
        data[i] = (i % 4) ? 0 : 1;
      }
    }

    /**
     * Copy matrix values from array (these data must be in column
     * major order!)
     */
    inline Matrix3(T const *dt) {
      std::memcpy(data, dt, sizeof(T) * 9);
    }

    /**
     * Copy constructor.
     * @param src Data source for new created instance of Matrix3
     */
    inline Matrix3(Matrix3<T> const &src) {
      std::memcpy(data, src.data, sizeof(T) * 9);
    }

    /**
     * Copy casting constructor.
     * @param src Data source for new created instance of Matrix3
     */
    template<class FromT>
    inline Matrix3(Matrix3<FromT> const &src) {
      for(int i = 0; i != 9; ++i) {
        data[i] = static_cast<T>(src.data[i]);
      }
    }

    /**
     * Resets matrix to be identity matrix
     */
    inline void identity() {
      for(int i = 0; i != 9; ++i) {
        data[i] = (i % 4) ? 0 : 1;
      }
    }

    /**
     * Creates rotation matrix by rotation around axis.
     * @param xDeg Angle (in degrees) of rotation around axis X.
     * @param yDeg Angle (in degrees) of rotation around axis Y.
     * @param zDeg Angle (in degrees) of rotation around axis Z.
     */
    inline static Matrix3<T> constexpr createRotationAroundAxis(T xDeg, T yDeg, T zDeg) {
      T xRads(DEG2RAD(xDeg));
      T yRads(DEG2RAD(yDeg));
      T zRads(DEG2RAD(zDeg));

      Matrix3<T> ma, mb, mc;
      T ac = cos(xRads);
      T as = sin(xRads);
      T bc = cos(yRads);
      T bs = sin(yRads);
      T cc = cos(zRads);
      T cs = sin(zRads);

      ma.at(1, 1) = ac;
      ma.at(2, 1) = as;
      ma.at(1, 2) = -as;
      ma.at(2, 2) = ac;

      mb.at(0, 0) = bc;
      mb.at(2, 0) = -bs;
      mb.at(0, 2) = bs;
      mb.at(2, 2) = bc;

      mc.at(0, 0) = cc;
      mc.at(1, 0) = cs;
      mc.at(0, 1) = -cs;
      mc.at(1, 1) = cc;

      Matrix3<T> ret = ma * mb * mc;
      return ret;
    }

    /**
     * Creates rotation matrix by aligning one vector to another.
     * Taken from http://www.iquilezles.org/www/articles/noacos/noacos.htm
     * @param from Vector to rotate from.
     * @param to Vector to rotate to.
     * @return An instance of Matrix3<T> representing rotation between the two vectors.
     */
    inline static Matrix3<T> constexpr createRotationBetweenVectors(Vector3<T> from, Vector3<T> to) {
      Vector3<T> const v = to.crossProduct(from);
      T const c = to.dotProduct(from);
      T const k = 1.0f / (1.0f + c);
      T const retData[] = {
        v.x * v.x * k + c,   v.y * v.x * k - v.z, v.z * v.x * k + v.y,
        v.x * v.y * k + v.z, v.y * v.y * k + c,   v.z * v.y * k - v.x,
        v.x * v.z * k - v.y, v.y * v.z * k + v.x, v.z * v.z * k + c
      };
      return retData;
    }

    /**
     * Creates rotation matrix from ODE Matrix.
     */
    template<class It>
    inline static Matrix3<T> constexpr fromOde(const It *mat) {
      Matrix3<T> ret;
      for(int i = 0; i != 3; ++i) {
        for(int j = 0; j != 3; ++j) {
          ret.at(i, j) = static_cast<T>(mat[j * 4 + i]);
        }
      }
      return ret;
    }

    /**
     * Creates new matrix 3x3 from array that represents such matrix 3x3
     * as array of tightly packed elements in row major order.
     * @param arr An array of elements for 3x3 matrix in row major order.
     * @return An instance of Matrix3<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix3<T> constexpr fromRowMajorArray(const FromT *arr) {
      T const retData[] = {
        static_cast<T>(arr[0]), static_cast<T>(arr[3]), static_cast<T>(arr[6]),
        static_cast<T>(arr[1]), static_cast<T>(arr[4]), static_cast<T>(arr[7]),
        static_cast<T>(arr[2]), static_cast<T>(arr[5]), static_cast<T>(arr[8])
      };

      return retData;
    }

    /**
     * Creates new matrix 3x3 from array that represents such matrix 3x3
     * as array of tightly packed elements in column major order.
     * @param arr An array of elements for 3x3 matrix in column major order.
     * @return An instance of Matrix3<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix3<T> constexpr fromColumnMajorArray(const FromT *arr) {
      T const retData[] = {
        static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]),
        static_cast<T>(arr[3]), static_cast<T>(arr[4]), static_cast<T>(arr[5]),
        static_cast<T>(arr[6]), static_cast<T>(arr[7]), static_cast<T>(arr[8])
      };

      return retData;
    }

    //---------------------[ equiality operators ]------------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold EPSILON value. To be two
     * values equal, must satisfy this condition all elements of matrix
     * | lhs[i] - rhs[i] | < EPSILON,
     * same for y-coordinate, z-coordinate, and w-coordinate.
     */
    inline bool constexpr operator==(Matrix3<T> const &rhs) const {
      for(int i = 0; i != 9; ++i) {
        if(std::fabs(data[i] - rhs.data[i]) >= EPSILON)
          return false;
      }
      return true;
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Matrix3<T> const &rhs) const {
      return !(*this == rhs);
    }

    //---------------------[ access operators ]---------------------------------
    /**
     * Get reference to element at position (x,y).
     * @param x Number of column (0..2)
     * @param y Number of row (0..2)
     */
    inline T &at(int x, int y) {
      assert(x >= 0 && x < 3);
      assert(y >= 0 && y < 3);
      return data[x * 3 + y];
    }

    /**
     * Get constant reference to element at position (x,y).
     * @param x Number of column (0..2)
     * @param y Number of row (0..2)
     */
    inline const T &at(int x, int y) const {
      assert(x >= 0 && x < 3);
      assert(y >= 0 && y < 3);
      return data[x * 3 + y];
    }

    /**
     * Get reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..3)
     * @param j Number of column (1..3)
     */
    inline T &operator()(int i, int j) {
      assert(i >= 1 && i <= 3);
      assert(j >= 1 && j <= 3);
      return data[(j - 1) * 3 + i - 1];
    }

    /**
     * Get constant reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..3)
     * @param j Number of column (1..3)
     */
    inline const T &operator()(int i, int j) const {
      assert(i >= 1 && i <= 3);
      assert(j >= 1 && j <= 3);
      return data[(j - 1) * 3 + i - 1];
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> &operator=(Matrix3<T> const &rhs) {
      std::memcpy(data, rhs.data, sizeof(T) * 9);
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
    inline Matrix3<T> &operator=(T const *rhs) {
      std::memcpy(data, rhs, sizeof(T) * 9);
      return *this;
    }

    /*inline Matrix3<T> & operator=(const T *rhs)
     {
     for(int i = 0; i != 9; ++i) {
       data[i] = (T)rhs[i];
     }
     return *this;
     }*/

    //--------------------[ matrix with matrix operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator+(Matrix3<T> const &rhs) const {
      Matrix3<T> ret;
      for(int i = 0; i != 9; ++i) {
        ret.data[i] = data[i] + rhs.data[i];
      }
      return ret;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator-(Matrix3<T> const &rhs) const {
      Matrix3<T> ret;
      for(int i = 0; i != 9; ++i) {
        ret.data[i] = data[i] - rhs.data[i];
      }
      return ret;
    }

    //--------------------[ matrix with scalar operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator+(T rhs) const {
      Matrix3<T> ret;
      for(int i = 0; i != 9; ++i) {
        ret.data[i] = data[i] + rhs;
      }
      return ret;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator-(T rhs) const {
      Matrix3<T> ret;
      for(int i = 0; i != 9; ++i) {
        ret.data[i] = data[i] - rhs;
      }
      return ret;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator*(T rhs) const {
      Matrix3<T> ret;
      for(int i = 0; i != 9; ++i) {
        ret.data[i] = data[i] * rhs;
      }
      return ret;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator/(T rhs) const {
      Matrix3<T> ret;
      for(int i = 0; i != 9; ++i) {
        ret.data[i] = data[i] / rhs;
      }
      return ret;
    }

    //--------------------[ multiply operators ]--------------------------------
    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const {
      return Vector3<T>(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
                        data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
                        data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     * Enabling Vector4 rotation as if it's a Vector3, maintaining W
     */
    inline Vector4<T> constexpr operator*(Vector4<T> const &rhs) const {
      return Vector4<T>(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
                        data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
                        data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z,
                        rhs.w);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator*(Matrix3<T> rhs) const {
      static Matrix3<T> w;
      for(int i = 0; i != 3; ++i) {
        for(int j = 0; j != 3; ++j) {
          T n = 0;
          for(int k = 0; k != 3; ++k) {
            n += rhs.at(i, k) * at(k, j);
          }
          w.at(i, j) = n;
        }
      }
      return w;

    }

    //---------------------------[ misc operations ]----------------------------
    /**
     * Transpose matrix.
     */
    inline Matrix3<T> constexpr transpose() {
      Matrix3<T> ret;
      for(int i = 0; i != 3; ++i) {
        for(int j = 0; j != 3; ++j) {
          ret.at(i, j) = at(j, i);
        }
      }
      return ret;
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
    inline Matrix3<T> constexpr lerp(T fact, Matrix3<T> const &rhs) const {
      Matrix3<T> ret = (*this) + (rhs - (*this)) * fact;
      return ret;
    }

    inline T constexpr det() {
      return + at(0, 0) * at(1, 1) * at(2, 2) + at(0, 1) * at(1, 2) * at(2, 0) + at(0, 2) * at(1, 0) * at(2, 1)
             - at(0, 0) * at(1, 2) * at(2, 1) - at(0, 1) * at(1, 0) * at(2, 2) - at(0, 2) * at(1, 1) * at(2, 0);
    }

    /**
     * Computes inverse matrix
     * @return Inverse matrix of this matrix.
     */
    inline Matrix3<T> constexpr inverse() {
      Matrix3<T> ret;
      ret.at(0, 0) = at(1, 1) * at(2, 2) - at(2, 1) * at(1, 2);
      ret.at(0, 1) = at(2, 1) * at(0, 2) - at(0, 1) * at(2, 2);
      ret.at(0, 2) = at(0, 1) * at(1, 2) - at(1, 1) * at(0, 2);
      ret.at(1, 0) = at(2, 0) * at(1, 2) - at(1, 0) * at(2, 2);
      ret.at(1, 1) = at(0, 0) * at(2, 2) - at(2, 0) * at(0, 2);
      ret.at(1, 2) = at(1, 0) * at(0, 2) - at(0, 0) * at(1, 2);
      ret.at(2, 0) = at(1, 0) * at(2, 1) - at(2, 0) * at(1, 1);
      ret.at(2, 1) = at(2, 0) * at(0, 1) - at(0, 0) * at(2, 1);
      ret.at(2, 2) = at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
      return ret * (1.0f / det());
    }

    //-------------[ conversion ]-----------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Matrix3<T>)
     * used for passing Matrix3<T> values to gl*[fd]v functions.
     */
    inline operator T*() {
      return (T*)data;
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Matrix3<T>)
     * used for passing Matrix3<T> values to gl*[fd]v functions.
     */
    inline operator const T*() const {
      return (T const*)data;
    }

    //----------[ output operator ]----------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator <<(std::ostream &lhs, Matrix3<T> const &rhs) {
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
    inline std::string constexpr toString() const {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }
};

/// Matrix 3x3 of floats
typedef Matrix3<float> Matrix3f;
/// Matrix 3x3 of doubles
typedef Matrix3<double> Matrix3d;
/// Matrix 3x3 of long doubles
typedef Matrix3<long double> Matrix3ld;
/// Matrix 3x3 of int
typedef Matrix3<int> Matrix3i;

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
    T data[16];

    //--------------------------[ constructors ]-------------------------------
    /**
     *Creates identity matrix
     */
    inline Matrix4() {
      for(int i = 0; i != 16; ++i) {
        data[i] = (i % 5) ? 0 : 1;
      }
    }

    /**
     * Copy matrix values from array (these data must be in column
     * major order!)
     */
    inline Matrix4(T const *dt) {
      std::memcpy(data, dt, sizeof(T) * 16);
    }

    /**
     * Copy constructor.
     * @param src Data source for new created instance of Matrix4.
     */
    inline Matrix4(Matrix4<T> const &src) {
      std::memcpy(data, src.data, sizeof(T) * 16);
    }

    /**
     * Copy casting constructor.
     * @param src Data source for new created instance of Matrix4.
     */
    template<class FromT>
    inline Matrix4(Matrix4<FromT> const &src) {
      for(int i = 0; i != 16; ++i) {
        data[i] = static_cast<T>(src.data[i]);
      }
    }

    /**
     * Resets matrix to be identity matrix
     */
    inline void identity() {
      for(int i = 0; i != 16; ++i) {
        data[i] = (i % 5) ? 0 : 1;
      }
    }

    /**
     * Creates rotation matrix by rotation around axis.
     * @param xDeg Angle (in degrees) of rotation around axis X.
     * @param yDeg Angle (in degrees) of rotation around axis Y.
     * @param zDeg Angle (in degrees) of rotation around axis Z.
     */
    inline static Matrix4<T> constexpr createRotationAroundAxis(T xDeg, T yDeg, T zDeg) {
      T xRads(DEG2RAD(xDeg));
      T yRads(DEG2RAD(yDeg));
      T zRads(DEG2RAD(zDeg));

      Matrix4<T> ma, mb, mc;
      T ac = cos(xRads);
      T as = sin(xRads);
      T bc = cos(yRads);
      T bs = sin(yRads);
      T cc = cos(zRads);
      T cs = sin(zRads);

      ma.at(1, 1) = ac;
      ma.at(2, 1) = as;
      ma.at(1, 2) = -as;
      ma.at(2, 2) = ac;

      mb.at(0, 0) = bc;
      mb.at(2, 0) = -bs;
      mb.at(0, 2) = bs;
      mb.at(2, 2) = bc;

      mc.at(0, 0) = cc;
      mc.at(1, 0) = cs;
      mc.at(0, 1) = -cs;
      mc.at(1, 1) = cc;

      /*std::cout << "RotVec = " << a << "," << b << "," << c << std::endl;
       std::cout << "Rx = " << std::endl << ma;
       std::cout << "Ry = " << std::endl << mb;
       std::cout << "Rz = " << std::endl << mc;*/

      Matrix4<T> ret = ma * mb * mc;
      //std::cout << "Result = " << std::endl << ma * (mb * mc);

      return ret;
    }

    /// Creates translation matrix
    /**
     * Creates translation matrix.
     * @param x X-direction translation
     * @param y Y-direction translation
     * @param z Z-direction translation
     * @param w for W-coordinate translation (implicitly set to 1)
     */
    inline static Matrix4<T> constexpr createTranslation(T x, T y, T z, T w = 1) {
      Matrix4 ret;
      ret.at(3, 0) = x;
      ret.at(3, 1) = y;
      ret.at(3, 2) = z;
      ret.at(3, 3) = w;

      return ret;
    }

    /**
     * Creates new view matrix to look from specified position @a eyePos to specified position @a centerPos
     * @param eyePos A position of camera
     * @param centerPos A position where camera looks-at
     * @param upDir Direction of up vector
     * @return Resulting view matrix that looks from and at specific position.
     */
    inline static Matrix4<T> constexpr createLookAt(Vector3<T> const &eyePos, Vector3<T> const &centerPos, Vector3<T> const &upDir) {
      Vector3<T> forward, side, up;
      Matrix4<T> m;

      forward = centerPos - eyePos;
      up = upDir;

      forward.normalize();

      // Side = forward x up
      side = forward.crossProduct(up);
      side.normalize();

      // Recompute up as: up = side x forward
      up = side.crossProduct(forward);

      m.at(0, 0) = side.x;
      m.at(1, 0) = side.y;
      m.at(2, 0) = side.z;

      m.at(0, 1) = up.x;
      m.at(1, 1) = up.y;
      m.at(2, 1) = up.z;

      m.at(0, 2) = -forward.x;
      m.at(1, 2) = -forward.y;
      m.at(2, 2) = -forward.z;

      m = m * Matrix4<T>::createTranslation(-eyePos.x, -eyePos.y, -eyePos.z);
      return m;
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
    inline static Matrix4<T> constexpr createFrustum(T left, T right, T bottom, T top, T zNear, T zFar) {
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
      Matrix4<T> ret;

      T const invWidth = 1.0 / (right - left);
      T const invHeight = 1.0 / (top - bottom);
      T const invDepth = 1.0 / (zFar - zNear);

      T const twoZNear = 2 * zNear;

      ret.at(0, 0) = twoZNear * invWidth;
      ret.at(1, 1) = twoZNear * invHeight;

      ret.at(2, 0) = (right + left) * invWidth;
      ret.at(2, 1) = (top + bottom) * invHeight;
      ret.at(2, 2) = - (zFar + zNear) * invDepth;
      ret.at(2, 3) = -1;

      ret.at(3, 2) = - twoZNear * zFar * invDepth;

      return ret;
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
    inline static Matrix4<T> constexpr createOrtho(T left, T right, T bottom, T top, T zNear, T zFar) {
      /*
             2
          ------------       0              0              tx
          right - left

                             2
              0         ------------        0              ty
                        top - bottom

                                            -2
              0              0         ------------        tz
                                        zFar-zNear

              0              0              0              1

         where

                                                      tx = - (right + left) / (right - left)

                                                      ty = - (top + bottom) / (top - bottom)

                                                      tz = - (zFar + zNear) / (zFar - zNear)

       */

      T const invWidth  = 1.0 / (right - left);
      T const invHeight = 1.0 / (top - bottom);
      T const invDepth  = 1.0 / (zFar - zNear);

      Matrix4<T> ret;

      ret.at(0, 0) =  2 * invWidth;
      ret.at(1, 1) =  2 * invHeight;
      ret.at(2, 2) = -2 * invDepth;

      ret.at(3, 0) = -(right + left) * invWidth;
      ret.at(3, 1) = -(top + bottom) * invHeight;
      ret.at(3, 2) = -(zFar + zNear) * invDepth;

      return ret;
    }

    /**
     * Creates new matrix 4x4 from array that represents such matrix 4x4
     * as array of tightly packed elements in row major order.
     * @param arr An array of elements for 4x4 matrix in row major order.
     * @return An instance of Matrix4<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix4<T> constexpr fromRowMajorArray(const FromT *arr) {
      T const retData[] = {
        static_cast<T>(arr[0]), static_cast<T>(arr[4]), static_cast<T>(arr[8]), static_cast<T>(arr[12]),
        static_cast<T>(arr[1]), static_cast<T>(arr[5]), static_cast<T>(arr[9]), static_cast<T>(arr[13]),
        static_cast<T>(arr[2]), static_cast<T>(arr[6]), static_cast<T>(arr[10]), static_cast<T>(arr[14]),
        static_cast<T>(arr[3]), static_cast<T>(arr[7]), static_cast<T>(arr[11]), static_cast<T>(arr[15])
      };

      return retData;
    }

    /**
     * Creates new matrix 4x4 from array that represents such matrix 4x4
     * as array of tightly packed elements in column major order.
     * @param arr An array of elements for 4x4 matrix in column major order.
     * @return An instance of Matrix4<T> representing @a arr
     */
    template<class FromT>
    inline static Matrix4<T> constexpr fromColumnMajorArray(const FromT *arr) {
      T const retData[] = {
        static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]), static_cast<T>(arr[3]),
        static_cast<T>(arr[4]), static_cast<T>(arr[5]), static_cast<T>(arr[6]), static_cast<T>(arr[7]),
        static_cast<T>(arr[8]), static_cast<T>(arr[9]), static_cast<T>(arr[10]), static_cast<T>(arr[11]),
        static_cast<T>(arr[12]), static_cast<T>(arr[13]), static_cast<T>(arr[14]), static_cast<T>(arr[15])
      };

      return retData;
    }

    //---------------------[ Equality operators ]------------------------------
    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold EPSILON value. To be two
     * values equal, must satisfy this condition all elements of matrix
     * | lhs[i] - rhs[i] | < EPSILON,
     * same for y-coordinate, z-coordinate, and w-coordinate.
     */
    inline bool constexpr operator==(Matrix4<T> const &rhs) const {
      for(int i = 0; i != 16; ++i) {
        if(std::fabs(data[i] - rhs.data[i]) >= EPSILON
          )
          return false;
      }
      return true;
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Matrix4<T> const &rhs) const {
      return !(*this == rhs);
    }

    //---------------------[ access operators ]---------------------------------
    /**
     * Get reference to element at postion (x,y).
     * @param x Number of column (0..3)
     * @param y Number of row (0..3)
     */
    inline T &at(int x, int y) {
      assert(x >= 0 && x < 4);
      assert(y >= 0 && y < 4);
      return data[x * 4 + y];
    }

    /**
     * Get constant reference to element at position (x,y).
     * @param x Number of column (0..3)
     * @param y Number of row (0..3)
     */
    inline const T &at(int x, int y) const {
      assert(x >= 0 && x < 4);
      assert(y >= 0 && y < 4);
      return data[x * 4 + y];
    }

    /**
     * Get reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..4)
     * @param j Number of column (1..4)
     */
    inline T &operator()(int i, int j) {
      assert(i >= 1 && i <= 4);
      assert(j >= 1 && j <= 4);
      return data[(j - 1) * 4 + i - 1];
    }

    /**
     * Get constant reference to element at position (i,j), with math matrix notation
     * @param i Number of row (1..4)
     * @param j Number of column (1..4)
     */
    inline const T &operator()(int i, int j) const {
      assert(i >= 1 && i <= 4);
      assert(j >= 1 && j <= 4);
      return data[(j - 1) * 4 + i - 1];
    }

    /**
     * Sets translation part of matrix.
     *
     * @param v Vector of translation to be set.
     */
    inline void setTranslation(Vector3<T> const &v) {
      at(3, 0) = v.x;
      at(3, 1) = v.y;
      at(3, 2) = v.z;
      at(3, 3) = 1;
    }

    inline Vector3<T> constexpr getTranslation() {
      return Vector3<T>(at(3, 0), at(3, 1), at(3, 2));
    }

    /**
     * Sets rotation part (matrix 3x3) of matrix.
     *
     * @param m Rotation part of matrix
     */
    inline void setRotation(Matrix3<T> const &m) {
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
    inline Matrix4<T> &operator=(Matrix4<T> const &rhs) {
      std::memcpy(data, rhs.data, sizeof(T) * 16);
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
    inline Matrix4<T> &operator=(T const *rhs) {
      std::memcpy(data, rhs, sizeof(T) * 16);
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
    inline Matrix4<T> constexpr operator+(Matrix4<T> const &rhs) const {
      Matrix4<T> ret;
      for(int i = 0; i != 16; ++i) {
        ret.data[i] = data[i] + rhs.data[i];
      }
      return ret;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator-(Matrix4<T> const &rhs) const {
      Matrix4<T> ret;
      for(int i = 0; i != 16; ++i) {
        ret.data[i] = data[i] - rhs.data[i];
      }
      return ret;
    }

    //--------------------[ matrix with scalar operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator+(T rhs) const {
      Matrix4<T> ret;
      for(int i = 0; i != 16; ++i) {
        ret.data[i] = data[i] + rhs;
      }
      return ret;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator-(T rhs) const {
      Matrix4<T> ret;
      for(int i = 0; i != 16; ++i) {
        ret.data[i] = data[i] - rhs;
      }
      return ret;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator*(T rhs) const {
      Matrix4<T> ret;
      for(int i = 0; i != 16; ++i) {
        ret.data[i] = data[i] * rhs;
      }
      return ret;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator/(T rhs) const {
      Matrix4<T> ret;
      for(int i = 0; i != 16; ++i) {
        ret.data[i] = data[i] / rhs;
      }
      return ret;
    }

    //--------------------[ multiply operators ]--------------------------------
    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(Vector4<T> const &rhs) const {
      return Vector4<T>(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z + data[12] * rhs.w,
                        data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z + data[13] * rhs.w,
                        data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
                        data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w);

    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const {
      return Vector3<T>(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z,
                        data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z,
                        data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr operator*(Matrix4<T> rhs) const {
      static Matrix4<T> w;
      for(int i = 0; i != 4; ++i) {
        for(int j = 0; j != 4; ++j) {
          T n = 0;
          for(int k = 0; k != 4; ++k)
            n += rhs.at(i, k) * at(k, j);
          w.at(i, j) = n;
        }
      }
      return w;

    }

    //---------------------------[ misc operations ]----------------------------

    /**
     * Computes determinant of matrix
     * @return Determinant of matrix
     * @note This function does 3 * 4 * 6 mul, 3 * 6 add.
     */
    inline T constexpr det() {

      return +at(3, 0) * at(2, 1) * at(1, 2) * at(0, 3) - at(2, 0) * at(3, 1) * at(1, 2) * at(0, 3)
             - at(3, 0) * at(1, 1) * at(2, 2) * at(0, 3) + at(1, 0) * at(3, 1) * at(2, 2) * at(0, 3)

             + at(2, 0) * at(1, 1) * at(3, 2) * at(0, 3) - at(1, 0) * at(2, 1) * at(3, 2) * at(0, 3)
             - at(3, 0) * at(2, 1) * at(0, 2) * at(1, 3) + at(2, 0) * at(3, 1) * at(0, 2) * at(1, 3)

             + at(3, 0) * at(0, 1) * at(2, 2) * at(1, 3) - at(0, 0) * at(3, 1) * at(2, 2) * at(1, 3)
             - at(2, 0) * at(0, 1) * at(3, 2) * at(1, 3) + at(0, 0) * at(2, 1) * at(3, 2) * at(1, 3)

             + at(3, 0) * at(1, 1) * at(0, 2) * at(2, 3) - at(1, 0) * at(3, 1) * at(0, 2) * at(2, 3)
             - at(3, 0) * at(0, 1) * at(1, 2) * at(2, 3) + at(0, 0) * at(3, 1) * at(1, 2) * at(2, 3)

             + at(1, 0) * at(0, 1) * at(3, 2) * at(2, 3) - at(0, 0) * at(1, 1) * at(3, 2) * at(2, 3)
             - at(2, 0) * at(1, 1) * at(0, 2) * at(3, 3) + at(1, 0) * at(2, 1) * at(0, 2) * at(3, 3)

             + at(2, 0) * at(0, 1) * at(1, 2) * at(3, 3) - at(0, 0) * at(2, 1) * at(1, 2) * at(3, 3)
             - at(1, 0) * at(0, 1) * at(2, 2) * at(3, 3) + at(0, 0) * at(1, 1) * at(2, 2) * at(3, 3);

    }

    /**
     * Computes inverse matrix
     * @return Inverse matrix of this matrix.
     * @note This is a little bit time consuming operation
     * (16 * 6 * 3 mul, 16 * 5 add + det() + mul() functions)
     */
    inline Matrix4<T> constexpr inverse() {
      Matrix4<T> ret;

      ret.at(0, 0) = +at(2, 1) * at(3, 2) * at(1, 3) - at(3, 1) * at(2, 2) * at(1, 3) + at(3, 1) * at(1, 2) * at(2, 3)
                     - at(1, 1) * at(3, 2) * at(2, 3) - at(2, 1) * at(1, 2) * at(3, 3) + at(1, 1) * at(2, 2) * at(3, 3);

      ret.at(1, 0) = +at(3, 0) * at(2, 2) * at(1, 3) - at(2, 0) * at(3, 2) * at(1, 3) - at(3, 0) * at(1, 2) * at(2, 3)
                     + at(1, 0) * at(3, 2) * at(2, 3) + at(2, 0) * at(1, 2) * at(3, 3) - at(1, 0) * at(2, 2) * at(3, 3);

      ret.at(2, 0) = +at(2, 0) * at(3, 1) * at(1, 3) - at(3, 0) * at(2, 1) * at(1, 3) + at(3, 0) * at(1, 1) * at(2, 3)
                     - at(1, 0) * at(3, 1) * at(2, 3) - at(2, 0) * at(1, 1) * at(3, 3) + at(1, 0) * at(2, 1) * at(3, 3);

      ret.at(3, 0) = +at(3, 0) * at(2, 1) * at(1, 2) - at(2, 0) * at(3, 1) * at(1, 2) - at(3, 0) * at(1, 1) * at(2, 2)
                     + at(1, 0) * at(3, 1) * at(2, 2) + at(2, 0) * at(1, 1) * at(3, 2) - at(1, 0) * at(2, 1) * at(3, 2);

      ret.at(0, 1) = +at(3, 1) * at(2, 2) * at(0, 3) - at(2, 1) * at(3, 2) * at(0, 3) - at(3, 1) * at(0, 2) * at(2, 3)
                     + at(0, 1) * at(3, 2) * at(2, 3) + at(2, 1) * at(0, 2) * at(3, 3) - at(0, 1) * at(2, 2) * at(3, 3);

      ret.at(1, 1) = +at(2, 0) * at(3, 2) * at(0, 3) - at(3, 0) * at(2, 2) * at(0, 3) + at(3, 0) * at(0, 2) * at(2, 3)
                     - at(0, 0) * at(3, 2) * at(2, 3) - at(2, 0) * at(0, 2) * at(3, 3) + at(0, 0) * at(2, 2) * at(3, 3);

      ret.at(2, 1) = +at(3, 0) * at(2, 1) * at(0, 3) - at(2, 0) * at(3, 1) * at(0, 3) - at(3, 0) * at(0, 1) * at(2, 3)
                     + at(0, 0) * at(3, 1) * at(2, 3) + at(2, 0) * at(0, 1) * at(3, 3) - at(0, 0) * at(2, 1) * at(3, 3);

      ret.at(3, 1) = +at(2, 0) * at(3, 1) * at(0, 2) - at(3, 0) * at(2, 1) * at(0, 2) + at(3, 0) * at(0, 1) * at(2, 2)
                     - at(0, 0) * at(3, 1) * at(2, 2) - at(2, 0) * at(0, 1) * at(3, 2) + at(0, 0) * at(2, 1) * at(3, 2);

      ret.at(0, 2) = +at(1, 1) * at(3, 2) * at(0, 3) - at(3, 1) * at(1, 2) * at(0, 3) + at(3, 1) * at(0, 2) * at(1, 3)
                     - at(0, 1) * at(3, 2) * at(1, 3) - at(1, 1) * at(0, 2) * at(3, 3) + at(0, 1) * at(1, 2) * at(3, 3);

      ret.at(1, 2) = +at(3, 0) * at(1, 2) * at(0, 3) - at(1, 0) * at(3, 2) * at(0, 3) - at(3, 0) * at(0, 2) * at(1, 3)
                     + at(0, 0) * at(3, 2) * at(1, 3) + at(1, 0) * at(0, 2) * at(3, 3) - at(0, 0) * at(1, 2) * at(3, 3);

      ret.at(2, 2) = +at(1, 0) * at(3, 1) * at(0, 3) - at(3, 0) * at(1, 1) * at(0, 3) + at(3, 0) * at(0, 1) * at(1, 3)
                     - at(0, 0) * at(3, 1) * at(1, 3) - at(1, 0) * at(0, 1) * at(3, 3) + at(0, 0) * at(1, 1) * at(3, 3);

      ret.at(3, 2) = +at(3, 0) * at(1, 1) * at(0, 2) - at(1, 0) * at(3, 1) * at(0, 2) - at(3, 0) * at(0, 1) * at(1, 2)
                     + at(0, 0) * at(3, 1) * at(1, 2) + at(1, 0) * at(0, 1) * at(3, 2) - at(0, 0) * at(1, 1) * at(3, 2);

      ret.at(0, 3) = +at(2, 1) * at(1, 2) * at(0, 3) - at(1, 1) * at(2, 2) * at(0, 3) - at(2, 1) * at(0, 2) * at(1, 3)
                     + at(0, 1) * at(2, 2) * at(1, 3) + at(1, 1) * at(0, 2) * at(2, 3) - at(0, 1) * at(1, 2) * at(2, 3);

      ret.at(1, 3) = +at(1, 0) * at(2, 2) * at(0, 3) - at(2, 0) * at(1, 2) * at(0, 3) + at(2, 0) * at(0, 2) * at(1, 3)
                     - at(0, 0) * at(2, 2) * at(1, 3) - at(1, 0) * at(0, 2) * at(2, 3) + at(0, 0) * at(1, 2) * at(2, 3);

      ret.at(2, 3) = +at(2, 0) * at(1, 1) * at(0, 3) - at(1, 0) * at(2, 1) * at(0, 3) - at(2, 0) * at(0, 1) * at(1, 3)
                     + at(0, 0) * at(2, 1) * at(1, 3) + at(1, 0) * at(0, 1) * at(2, 3) - at(0, 0) * at(1, 1) * at(2, 3);

      ret.at(3, 3) = +at(1, 0) * at(2, 1) * at(0, 2) - at(2, 0) * at(1, 1) * at(0, 2) + at(2, 0) * at(0, 1) * at(1, 2)
                     - at(0, 0) * at(2, 1) * at(1, 2) - at(1, 0) * at(0, 1) * at(2, 2) + at(0, 0) * at(1, 1) * at(2, 2);

      return ret / det();
    }

    /**
     * Transpose matrix.
     */
    inline Matrix4<T> constexpr transpose() {
      Matrix4<T> ret;
      for(int i = 0; i != 4; ++i) {
        for(int j = 0; j != 4; ++j) {
          ret.at(i, j) = at(j, i);
        }
      }
      return ret;
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
    inline Matrix4<T> constexpr lerp(T fact, Matrix4<T> const &rhs) const {
      Matrix4<T> ret = (*this) + (rhs - (*this)) * fact;
      return ret;
    }

    //-------------[ conversion ]-----------------------------
    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Matrix4<T>)
     * used for passing Matrix4<T> values to gl*[fd]v functions.
     */
    inline operator T*() {
      return (T*)data;
    }

    /**
     * Conversion to pointer operator
     * @return Constant Pointer to internally stored (in management of class Matrix4<T>)
     * used for passing Matrix4<T> values to gl*[fd]v functions.
     */
    inline operator const T*() const {
      return (T const*)data;
    }

    //----------[ output operator ]----------------------------
    /**
     * Output to stream operator
     * @param lhs Left hand side argument of operator (commonly ostream instance).
     * @param rhs Right hand side argument of operator.
     * @return Left hand side argument - the ostream object passed to operator.
     */
    inline friend std::ostream &operator <<(std::ostream &lhs, Matrix4<T> const &rhs) {
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
    inline std::string constexpr toString() const {
      std::ostringstream oss;
      oss << *this;
      return oss.str();
    }

};

/// Matrix 4x4 of floats
typedef Matrix4<float> Matrix4f;
/// Matrix 4x4 of doubles
typedef Matrix4<double> Matrix4d;
/// Matrix 4x4 of long doubles
typedef Matrix4<long double> Matrix4ld;
/// Matrix 4x4 of int
typedef Matrix4<int> Matrix4i;

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
    inline constexpr Quaternion()
      : w(0), v(0, 0, 0) {
    }

    /**
     * Copy constructor.
     */
    inline constexpr Quaternion(Quaternion<T> const &q)
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
    inline constexpr Quaternion(T w_, Vector3<T> const &v_)
      : w(w_), v(v_) {
    }

    /**
     * Creates quaternion object from value (w_ + xi + yj + zk).
     * @param w_ Real part of quaternion.
     * @param x Complex coefficient for i complex constant.
     * @param y Complex coefficient for j complex constant.
     * @param z Complex coefficient for k complex constant.
     */
    inline constexpr Quaternion(T w_, T x, T y, T z)
      : w(w_), v(x, y, z) {
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator=(Quaternion<T> const &rhs) {
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
    inline Quaternion<T> constexpr operator+(Quaternion<T> const &rhs) const {
      return Quaternion<T>(w + rhs.w, v + rhs.v);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator*(Quaternion<T> const &rhs) const {
      return Quaternion<T>(w * rhs.w   - v.x * rhs.v.x - v.y * rhs.v.y - v.z * rhs.v.z,
                           w * rhs.v.x + v.x * rhs.w   + v.y * rhs.v.z - v.z * rhs.v.y,
                           w * rhs.v.y - v.x * rhs.v.z + v.y * rhs.w   + v.z * rhs.v.x,
                           w * rhs.v.z + v.x * rhs.v.y - v.y * rhs.v.x + v.z * rhs.w);
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator*(T rhs) const {
      return Quaternion<T>(w * rhs, v * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator/(T rhs) const {
      return Quaternion<T>(w / rhs, v / rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> constexpr operator-(Quaternion<T> const &rhs) const {
      return Quaternion<T>(w - rhs.w, v - rhs.v);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator+=(Quaternion<T> const &rhs) {
      w += rhs.w;
      v += rhs.v;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator-=(Quaternion<T> const &rhs) {
      w -= rhs.w;
      v -= rhs.v;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator*=(Quaternion<T> const &rhs) {
      Quaternion q = (*this) * rhs;
      v = q.v;
      w = q.w;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator*=(T rhs) {
      w *= rhs;
      v *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Quaternion<T> &operator/=(T rhs) {
      w /= rhs;
      v /= rhs;
      return *this;
    }

    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold EPSILON value. To be two
     * values equal, must satisfy this condition | lhs - rhs | < EPSILON,
     * for all quaternion coordinates.
     */
    inline bool constexpr operator==(Quaternion<T> const &rhs) const {
      return (std::fabs(w - rhs.w) < EPSILON) && v == rhs.v;
    }

    /**
     * Inequality test operator
     * @param rhs Right hand side argument of binary operator.
     * @return not (lhs == rhs) :-P
     */
    inline bool constexpr operator!=(Quaternion<T> const &rhs) const {
      return !(*this == rhs);
    }

    //-------------[ unary operations ]--------------------------
    /**
     * Unary negate operator
     * @return negated quaternion
     */
    inline Quaternion<T> constexpr operator-() const {
      return Quaternion<T>(-w, -v);
    }

    /**
     * Unary conjugate operator
     * @return conjugated quaternion
     */
    inline Quaternion<T> constexpr operator~() const {
      return Quaternion<T>(w, -v);
    }

    /**
     * Get length of quaternion.
     * @return Length of quaternion.
     */
    inline T constexpr length() const {
      return (T) std::sqrt(w * w + v.lengthSq());
    }

    /**
     * Return square of length.
     * @return length ^ 2
     * @note This method is faster then length(). For comparison
     * of length of two quaternion can be used just this value, instead
     * of more expensive length() method.
     */
    inline T constexpr lengthSq() const {
      return w * w + v.lengthSq();
    }

    /**
     * Normalize quaternion
     */
    inline void normalize() {
      T len = length();
      w /= len;
      v /= len;
    }
    inline void normalise() {  // proper english
      normalize();
    }
    inline Quaternion<T> constexpr normalize_copy() const {
      return Quaternion<T>(w / length(), v / length());
    }
    inline Quaternion<T> constexpr normalise_copy() const {  // proper english
      return normalize_copy();
    }

    inline void conjugate() {
      v = -v;
    }

    inline Quaternion<T> constexpr conjugate_copy() const {
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
    inline void invert() {
      T l = length();
      conjugate();
      (*this) /= l;
    }

    inline Quaternion<T> constexpr invert_copy() const {
      return conjugate_copy() / length();
    }

    /**
     * Creates quaternion for Euler angles.
     * @param x Rotation around x axis (in degrees).
     * @param y Rotation around y axis (in degrees).
     * @param z Rotation around z axis (in degrees).
     * @return Quaternion object representing transformation.
     */
    inline static Quaternion<T> constexpr fromEulerAngles(T x, T y, T z) {
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
    inline static Quaternion<T> constexpr fromEulerAngles_rad(T x, T y, T z) {
      return Quaternion<T>(fromAxisRot_rad(Vector3<T>(1, 0, 0), x) *
                           fromAxisRot_rad(Vector3<T>(0, 1, 0), y) *
                           fromAxisRot_rad(Vector3<T>(0, 0, 1), z));
    }

    /**
     * Creates quaternion as rotation around axis.
     * @param axis Unit vector expressing axis of rotation.
     * @param angleDeg Angle of rotation around axis (in degrees).
     */
    inline static Quaternion<T> constexpr fromAxisRot(Vector3<T> axis, T angleDeg) {
      return Quaternion<T>(std::cos(DEG2RAD(angleDeg) / 2.0), axis * std::sin(DEG2RAD(angleDeg) / 2.0));
    }

    /**
     * Creates quaternion as rotation around axis, rad version.
     * @param axis Unit vector expressing axis of rotation.
     * @param angleDeg Angle of rotation around axis (in radians).
     */
    inline static Quaternion<T> constexpr fromAxisRot_rad(Vector3<T> axis, T angleRad) {
      return Quaternion<T>(std::cos(angleRad / 2.0), axis * std::sin(angleRad / 2.0));
    }

    /**
     * Converts quaternion into rotation matrix.
     * @return Rotation matrix expressing this quaternion.
     */
    inline Matrix3<T> constexpr rotMatrix() {
      Matrix3<T> ret;

      /*ret.at(0,0) = 1 - 2*v.y*v.y - 2*v.z*v.z;
       ret.at(1,0) = 2*v.x*v.y - 2*w*v.z;
       ret.at(2,0) = 2*v.x*v.z - 2*w*v.y;

       ret.at(0,1) = 2*v.x*v.y + 2*w*v.z;
       ret.at(1,1) = 1 - 2*v.x*v.x - 2*v.z*v.z;
       ret.at(2,1) = 2*v.y*v.z - 2*w*v.x;

       ret.at(0,2) = 2*v.x*v.z - 2*w*v.y;
       ret.at(1,2) = 2*v.y*v.z + 2*w*v.x;
       ret.at(2,2) = 1 - 2*v.x*v.x - 2*v.y*v.y;*/

      T xx = v.x * v.x;
      T xy = v.x * v.y;
      T xz = v.x * v.z;
      T xw = v.x * w;

      T yy = v.y * v.y;
      T yz = v.y * v.z;
      T yw = v.y * w;

      T zz = v.z * v.z;
      T zw = v.z * w;

      ret.at(0, 0) = 1 - 2 * (yy + zz);
      ret.at(1, 0) = 2 * (xy - zw);
      ret.at(2, 0) = 2 * (xz + yw);

      ret.at(0, 1) = 2 * (xy + zw);
      ret.at(1, 1) = 1 - 2 * (xx + zz);
      ret.at(2, 1) = 2 * (yz - xw);

      ret.at(0, 2) = 2 * (xz - yw);
      ret.at(1, 2) = 2 * (yz + xw);
      ret.at(2, 2) = 1 - 2 * (xx + yy);

      return ret;
    }

    /**
     * Converts quaternion into transformation matrix.
     * @note This method performs same operation as rotMatrix()
     * conversion method. But returns Matrix of 4x4 elements.
     * @return Transformation matrix expressing this quaternion.
     */
    inline Matrix4<T> constexpr transform() const {
      Matrix4<T> ret;

      T xx = v.x * v.x;
      T xy = v.x * v.y;
      T xz = v.x * v.z;
      T xw = v.x * w;

      T yy = v.y * v.y;
      T yz = v.y * v.z;
      T yw = v.y * w;

      T zz = v.z * v.z;
      T zw = v.z * w;

      ret.at(0, 0) = 1 - 2 * (yy + zz);
      ret.at(1, 0) = 2 * (xy - zw);
      ret.at(2, 0) = 2 * (xz + yw);
      ret.at(3, 0) = 0;

      ret.at(0, 1) = 2 * (xy + zw);
      ret.at(1, 1) = 1 - 2 * (xx + zz);
      ret.at(2, 1) = 2 * (yz - xw);
      ret.at(3, 1) = 0;

      ret.at(0, 2) = 2 * (xz - yw);
      ret.at(1, 2) = 2 * (yz + xw);
      ret.at(2, 2) = 1 - 2 * (xx + yy);
      ret.at(3, 2) = 0;

      ret.at(0, 3) = 0;
      ret.at(1, 3) = 0;
      ret.at(2, 3) = 0;
      ret.at(3, 3) = 1;

      return ret;

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
    inline Quaternion<T> constexpr lerp(T fact, Quaternion<T> const &rhs) const {
      return Quaternion<T>((1 - fact) * w + fact * rhs.w, v.lerp(fact, rhs.v));
    }

    /**
     * Provides output to standard output stream.
     */
    inline friend std::ostream &operator <<(std::ostream &oss, Quaternion<T> const &q) {
      oss << "Re: " << q.w << " Im: " << q.v;
      return oss;
    }

    /**
     * Gets string representation.
     */
    inline std::string constexpr toString() const {
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
    inline static Quaternion<T> fromMatrix(Matrix4<T> const &m) {
      Quaternion<T> q;

      T s;
      T const tr = m(1, 1) + m(2, 2) + m(3, 3);
      if(tr >= epsilon) {
        s = 0.5 / (T)sqrt(tr + 1.0);
        q.w = 0.25 / s;
        q.v.x = (m(3, 2) - m(2, 3)) * s;
        q.v.y = (m(1, 3) - m(3, 1)) * s;
        q.v.z = (m(2, 1) - m(1, 2)) * s;
      } else {
        T d0 = m(1, 1);
        T d1 = m(2, 2);
        T d2 = m(3, 3);

        char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);

        if(bigIdx == 0) {
          s = 2.0 * (T) sqrt(1.0 + m(1, 1) - m(2, 2) - m(3, 3));
          q.w = (m(3, 2) - m(2, 3)) / s;
          q.v.x = 0.25 * s;
          q.v.y = (m(1, 2) + m(2, 1)) / s;
          q.v.z = (m(1, 3) + m(3, 1)) / s;
        } else if(bigIdx == 1) {
          s = 2.0 * (T) sqrt(1.0 + m(2, 2) - m(1, 1) - m(3, 3));
          q.w = (m(1, 3) - m(3, 1)) / s;
          q.v.x = (m(1, 2) + m(2, 1)) / s;
          q.v.y = 0.25 * s;
          q.v.z = (m(2, 3) + m(3, 2)) / s;
        } else {
          s = 2.0 * (T) sqrt(1.0 + m(3, 3) - m(1, 1) - m(2, 2));
          q.w = (m(2, 1) - m(1, 2)) / s;
          q.v.x = (m(1, 3) + m(3, 1)) / s;
          q.v.y = (m(2, 3) + m(3, 2)) / s;
          q.v.z = 0.25 * s;
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
    inline static Quaternion<T> fromMatrix(Matrix3<T> const &m) {
      Quaternion<T> q;

      T s;
      T const tr = m(1, 1) + m(2, 2) + m(3, 3);
      if(tr >= epsilon) {
        s = 0.5 / (T)sqrt(tr + 1.0);
        q.w = 0.25 / s;
        q.v.x = (m(3, 2) - m(2, 3)) * s;
        q.v.y = (m(1, 3) - m(3, 1)) * s;
        q.v.z = (m(2, 1) - m(1, 2)) * s;
      } else {
        T d0 = m(1, 1);
        T d1 = m(2, 2);
        T d2 = m(3, 3);

        char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);

        if(bigIdx == 0) {
          s = 2.0 * (T) sqrt(1.0 + m(1, 1) - m(2, 2) - m(3, 3));
          q.w = (m(3, 2) - m(2, 3)) / s;
          q.v.x = 0.25 * s;
          q.v.y = (m(1, 2) + m(2, 1)) / s;
          q.v.z = (m(1, 3) + m(3, 1)) / s;
        } else if(bigIdx == 1) {
          s = 2.0 * (T) sqrt(1.0 + m(2, 2) - m(1, 1) - m(3, 3));
          q.w = (m(1, 3) - m(3, 1)) / s;
          q.v.x = (m(1, 2) + m(2, 1)) / s;
          q.v.y = 0.25 * s;
          q.v.z = (m(2, 3) + m(3, 2)) / s;
        } else {
          s = 2.0 * (T) sqrt(1.0 + m(3, 3) - m(1, 1) - m(2, 2));
          q.w = (m(2, 1) - m(1, 2)) / s;
          q.v.x = (m(1, 3) + m(3, 1)) / s;
          q.v.y = (m(2, 3) + m(3, 2)) / s;
          q.v.z = 0.25 * s;
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
    inline Quaternion<T> slerp(T r, Quaternion<T> const &q2) const {
      Quaternion<T> ret;
      T const cosTheta = w * q2.w + v.x * q2.v.x + v.y * q2.v.y + v.z * q2.v.z;
      T const theta = (T)acos(cosTheta);
      if(fabs(theta) < epsilon) {
        ret = *this;
      } else {
        T sinTheta = (T)sqrt(1.0 - cosTheta * cosTheta);
        if(fabs(sinTheta) < epsilon) {
          ret.w = 0.5 * w + 0.5 * q2.w;
          ret.v = v.lerp(0.5, q2.v);
        } else {
          T rA = (T) sin((1.0 - r) * theta) / sinTheta;
          T rB = (T) sin(r * theta) / sinTheta;

          ret.w = w * rA + q2.w * rB;
          ret.v.x = v.x * rA + q2.v.x * rB;
          ret.v.y = v.y * rA + q2.v.y * rB;
          ret.v.z = v.z * rA + q2.v.z * rB;
        }
      }
      return ret;
    }

};

typedef Quaternion<float> Quatf;
typedef Quaternion<double> Quatd;
typedef Quaternion<long double> Quatld;

#ifdef VMATH_NAMESPACE
}
#endif

#endif // __vmath_Header_File__

