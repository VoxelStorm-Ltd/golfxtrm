        T d2 = m(3, 3);

        char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);

        if(bigIdx == 0) {
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(1, 1) - m(2, 2) - m(3, 3)));
          q.w = (m(3, 2) - m(2, 3)) / s;
          q.v.x = static_cast<T>(0.25) * s;
          q.v.y = (m(1, 2) + m(2, 1)) / s;
          q.v.z = (m(1, 3) + m(3, 1)) / s;
        } else if(bigIdx == 1) {
          T s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(2, 2) - m(1, 1) - m(3, 3)));
          q.w = (m(1, 3) - m(3, 1)) / s;
          q.v.x = (m(1, 2) + m(2, 1)) / s;
          q.v.y = static_cast<T>(0.25) * s;
          q.v.z = (m(2, 3) + m(3, 2)) / s;
        } else {
          T s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(3, 3) - m(1, 1) - m(2, 2)));
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
    inline static Quaternion<T> constexpr fromMatrix(Matrix3<T> const &m) __attribute__((__always_inline__)) {
      Quaternion<T> q;

      T const tr = m(1, 1) + m(2, 2) + m(3, 3);
      if(tr >= static_cast<T>(epsilon)) {
        T const s = static_cast<T>(0.5) / static_cast<T>(std::sqrt(tr + static_cast<T>(1.0)));
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
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(1, 1) - m(2, 2) - m(3, 3)));
          q.w = (m(3, 2) - m(2, 3)) / s;
          q.v.x = static_cast<T>(0.25) * s;
          q.v.y = (m(1, 2) + m(2, 1)) / s;
          q.v.z = (m(1, 3) + m(3, 1)) / s;
        } else if(bigIdx == 1) {
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(2, 2) - m(1, 1) - m(3, 3)));
          q.w = (m(1, 3) - m(3, 1)) / s;
          q.v.x = (m(1, 2) + m(2, 1)) / s;
          q.v.y = static_cast<T>(0.25) * s;
          q.v.z = (m(2, 3) + m(3, 2)) / s;
        } else {
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(3, 3) - m(1, 1) - m(2, 2)));
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
    inline Quaternion<T> constexpr slerp(T r, Quaternion<T> const &q2) const __attribute__((__always_inline__)) {
      Quaternion<T> ret;
      T const cosTheta = w * q2.w + v.x * q2.v.x + v.y * q2.v.y + v.z * q2.v.z;
      T const theta = static_cast<T>(std::acos(cosTheta));
      if(std::abs(theta) < static_cast<T>(epsilon)) {
        ret = *this;
      } else {
        T sinTheta = static_cast<T>(std::sqrt(static_cast<T>(1.0) - cosTheta * cosTheta));
        if(std::abs(sinTheta) < static_cast<T>(epsilon)) {
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
template<typename T>
constexpr VEC2<T> min(VEC2<T> const &a, const VEC2<T> &b) {
  return VEC2<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y));
}

/**
 * Gets vector containing minimal values of @a a and @a b coordinates.
 * @return Vector of minimal coordinates.
 */
template<typename T>
constexpr VEC3<T> min(VEC3<T> const &a, const VEC3<T> &b) {
  return VEC3<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y), ::std::min(a.z, b.z));
}

/**
 * Gets vector containing minimal values of @a a and @a b coordinates.
 * @return Vector of minimal coordinates.
 */
template<typename T>
constexpr VEC4<T> min(VEC4<T> const &a, const VEC4<T> &b) {
  return VEC4<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y), ::std::min(a.z, b.z), ::std::min(a.w, b.w));
}
/* -*- C++ -*- */

/** @page license License
/**
 * vmath, set of classes for computer graphics mathematics.
 * Gets vector containing maximal values of @a a and @a b coordinates.
 * Copyright (c) 2005-2011, Jan Bartipan < barzto at gmail dot com >
 * @return Vector of maximal coordinates.
 * All rights reserved.
 */
 *
template<typename T>
 * Redistribution and use in source and binary forms, with or without
constexpr VEC2<T> max(VEC2<T> const &a, const VEC2<T> &b) {
 * modification, are permitted provided that the following conditions
  return VEC2<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y));
 * are met:
}
 *

/**
 * - Redistributions of source code must retain the above copyright
 * Gets vector containing maximal values of @a a and @a b coordinates.
 *   notice, this list of conditions and the following disclaimer.
 * @return Vector of maximal coordinates.
 * - Redistributions in binary form must reproduce the above copyright
 */
 *   notice, this list of conditions and the following disclaimer in
template<typename T>
 *   the documentation and/or other materials provided with the
constexpr VEC3<T> max(VEC3<T> const &a, const VEC3<T> &b) {
 *   distribution.
  return VEC3<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y), ::std::max(a.z, b.z));
 * - Neither the names of its contributors may be used to endorse or
}
 *   promote products derived from this software without specific

 *   prior written permission.
/**
 *
 * Gets vector containing maximal values of @a a and @a b coordinates.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * @return Vector of maximal coordinates.
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 */
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
template<typename T>
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
constexpr VEC4<T> max(VEC4<T> const &a, const VEC4<T> &b) {
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  return VEC4<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y), ::std::max(a.z, b.z), ::std::max(a.w, b.w));
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
}
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
}
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER

 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// cleanup shortcut defines
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
#undef VEC2
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#undef VEC3
 * POSSIBILITY OF SUCH DAMAGE.
#undef VEC4
 */


#ifdef VMATH_NAMESPACE
/**
namespace VMATH_NAMESPACE {
 * @mainpage Intro
#endif //VMATH_NAMESPACE
 * Vector mathematics for computer graphics - VoxelStorm version

 *
/**
 * @section Features
 * Two-dimensional axis-aligned bounding-box (aka AABB) class.
 * <ul>
 *
 *    <li> basic arithmetic operations - using operators </li>
 * This class provides functionality for:
 *    <li> basic linear algebra operations - such as transpose, dot product, etc. </li>
 * - creating an AABB from a point, or another AABB,
 *    <li> aliases for vertex coordinates - it means:
 * - testing if a point of other AABB intersects with it,
 *    <pre>
 * - getting result of intersection with other AABB,
 *  Vector3f v;
 * - transforming AABB with 4x4 matrix.
 *  // use vertex coordinates
 *
 *  v.x = 1; v.y = 2; v.z = -1;
 * There are also overloaded couple of operators to shorten common operations.
 *
 * For instance you can use  @c operator<< on AABB to extend it with a passed point or other AABB.
 *  // use texture coordinates
 * @code
 *  v.s = 0; v.t = 1; v.u = 0.5;
 * Aabb2f aabb;
 *  // use colour coordinates
 * aabb << Vector2f(1, 2) << Aabb2f(-3,-3, 2, 2);
 *  v.r = 1; v.g = 0.5; v.b = 0;
 * @endcode
 *    </pre>
 */
 *    </li>
template<typename T>
 *    <li> conversion constructor and assign operators - so you can assign a value of Vector3&lt;T1&gt; type
class Aabb2 {
 *    to a variable of Vector3&lt;T2&gt; type for any convertible T1, T2 type pairs. In other words, you can do this:
  public:
 *    <pre>
    /**
 *
     * Position of Min corner of bounding box.
 *  Vector3f f3; Vector3d d3 = f3;
     */
 *  ...
    Vector2<T> min;
 *  f3 = d3;

 *    </pre>
    /**
 *    </li>
     * Position of Max corner of bounding box
 * </ul>
     */
 *
    Vector2<T> max;
 * @section Predefined types

 * <ul>
    /**
 *  <li>(Vector2) Two dimensional vector
     * Constructs invalid axes-aligned bounding-box.
 *    <ul>
     * @see valid() for explanation of invalid bounding-box usage.
 *      <li>float &mdash; Vector2f</li>
     */
 *      <li>double &mdash; Vector2d</li>
    inline constexpr Aabb2() __attribute__((__always_inline__))
 *      <li>int &mdash; Vector2i</li>
      : min(1, 1), max(-1, -1) {
 *    </ul>
    }
 *  </li>

 *  <li>(Vector3) Three dimensional vector
    /**
 *    <ul>
     * Constructs axes-aligned bound-box containing one point @a point
 *      <li>float &mdash; Vector3f</li>
     * @param point
 *      <li>double &mdash; Vector3d</li>
     */
 *      <li>int &mdash; Vector3i</li>
    template<typename SrcT>
 *    </ul>
    inline constexpr Aabb2(Vector2<SrcT> const &point)
 *  </li>
      : min(point), max(point) {
 *  <li>(Vector4) Four dimensional vector
    }
 *    <ul>

 *      <li>float &mdash; Vector4f</li>
    /**
 *      <li>double &mdash; Vector4d</li>
     * Constructs axes-aligned bounding-box form two corner points (@a x0, @a y0)
 *      <li>int &mdash; Vector4i</li>
     * and (@a x1, @a y1)
 *    </ul>
     * @param x0 X-coordinate of first point
 *  </li>
     * @param y0 Y-coordinate of first point
 *  <li>(Matrix3) Matrix 3x3
     * @param x1 X-coordinate of second point
 *    <ul>
     * @param y1 Y-coordinate of second point
 *      <li>float &mdash; Matrix3f</li>
     */
 *      <li>double &mdash; Matrix3d</li>
    template<typename SrcT>
 *      <li>int &mdash; Matrix3i</li>
    inline constexpr Aabb2(SrcT x0, SrcT y0, SrcT x1, SrcT y1)
 *    </ul>
      : min(std::min(x0, x1), std::min(y0, y1)),
 *  </li>
        max(std::max(x0, x1), std::max(y0, y1)) {
 *  <li>(Matrix4) Matrix 4x4
    }
 *    <ul>

 *      <li>float &mdash; Matrix4f</li>
    /**
 *      <li>double &mdash; Matrix4d</li>
     * Constructs axes-aligned bounding-box from @a min and @b max
 *      <li>int &mdash; Matrix4i</li>
 *    </ul>
     * @param min X-coordinate of first point
 *  </li>
     * @param max X-coordinate of second point
  *   <li> Quaternion
     */
 *    <ul>
    template<typename SrcT>
 *      <li>float &mdash; Quatf</li>
    inline constexpr Aabb2(Vector2<SrcT> const &new_min, Vector2<SrcT> const &new_max)
 *      <li>double &mdash; Quatd</li>
      : min(new_min),
 *    </ul>
        max(new_max) {
 *    </li>
    }
 *  </li>

 * </ul>
    /**
 */
     * Constructs axes-aligned bounding-box containing point (@a x, @a y)

     * @param x X-coordinate of point
// Modified 2011-06-12, Davide Bacchet (davide.bacchet at gmail dot com)
     * @param y Y-coordinate of point
//                      added () operators with standard matrix notation (opposite wrt at() ).
     */

    template<typename SrcT>
// Modified 2013-2016, Eugene Hopkinson for VoxelStorm Ltd
    inline constexpr Aabb2(SrcT x, SrcT y)
//                     various expansions and conversions
      : min(x, y), max(x, y) {
//                     Version 2.0: C++11 specific optimisations, including constexpr
    }
//                     Version 2.1: C++14 optimisations, intersection algorithms

//                     Version 2.2: Many additional functions
    /**

     * Creates copy of axis-aligned bounding-box
#ifndef __vmath_Header_File__
     * @param src Source bounding-box
#define __vmath_Header_File__
     */

    template<typename SrcT>
#define _USE_MATH_DEFINES
    inline constexpr Aabb2(Aabb2<SrcT> const &src)
#include <cmath>
      : min(src.min), max(src.max) {
#include <cstring>
    }
#include <iostream>

#include <sstream>
    /**
#include <string>
     * Assign operator
#include <array>
     * @param rhs source bounding-box

     * @return refenrence to this
#ifdef VMATH_NAMESPACE
     */
namespace VMATH_NAMESPACE {
    template<typename SrcT>
#endif
    inline Aabb2<T> constexpr &operator=(Aabb2<SrcT> const &rhs) {

      min = rhs.min;
// use boost's constants if available
      max = rhs.max;
#ifndef VMATH_NO_BOOST
      return *this;
#include <boost/math/constants/constants.hpp>
    }
#else  // VMATH_NO_BOOST

#ifndef M_PI
    /**
#define M_PI           3.14159265358979323846  /* pi */
#endif // M_PI
     * Checks if bounding-box is valid. Valid bounding-box has non-negative size.
#endif // VMATH_NO_BOOST

     * If an invalid bounding-box is extended by point or another bounding-box, the target
// note: use VMATH_SOFT_COMPARE to enable fuzzy matching for different types,
     * bounding box becomes valid and contains solely the source point or bounding-box respectively.
// with epsilon used to match within a range of rounding error - but doing this
     * @return True if box is valid, otherwise false
// can be very expensive in some cases.
     */
double constexpr epsilon = 4.37114e-05;
    inline bool constexpr valid() const __attribute__((__always_inline__)) {
#define EPSILON epsilon
      return min.x <= max.x && min.y <= max.y;
#define DEG2RAD deg2rad
    }
#define RAD2DEG rad2deg


    /**
template<typename T>
     * Makes this bounding-box invalid. So calling valid() gets false.
inline static T constexpr const deg2rad(T const angle_deg) __attribute__((__always_inline__));
     * @see valid() method for more info on usage of invalid bounding-boxes.
template<typename T>
     */
inline static T constexpr const deg2rad(T const angle_deg) {
    inline void constexpr invalidate() __attribute__((__always_inline__)) {
  #ifndef VMATH_NO_BOOST
      min = Vector2<T>( 1,  1);
    //return (angle_deg * boost::math::constants::pi<T>()) / 180.0;
      max = Vector2<T>(-1, -1);
    return angle_deg * boost::math::constants::degree<T>();
    }
  #else  // VMATH_NO_BOOST

    return (angle_deg * M_PI) / 180.0;
    /**
  #endif // VMATH_NO_BOOST
     * Extends this bounding-box by a point @a point.
}
     * @param point A point to extend bounding-box by.

     */
template<typename T>
    template<typename SrcT>
inline static T constexpr const rad2deg(T const angle_rad) __attribute__((__always_inline__));
    inline void constexpr extend(Vector2<SrcT> const &point) {
template<typename T>
      if(!valid()) {
inline static T constexpr const rad2deg(T const angle_rad) {
        min = max = point;
  #ifndef VMATH_NO_BOOST
      } else {
    return angle_rad * boost::math::constants::radian<T>();
        min = std::min(min, point);
  #else  // VMATH_NO_BOOST
        max = std::max(max, point);
    return (angle_rad * 180.0) / M_PI;
      }
  #endif // VMATH_NO_BOOST
    }
}


    /**
template<typename T>
     * Extends this bounding-box by a box @a box.
inline static void constexpr sincos_any(T const angle_rad, T &out_sin, T &out_cos) __attribute__((__always_inline__));
     * @param box A box to extend this bounding-box by.
template<typename T>
     */
inline static void constexpr sincos_any(T const angle_rad, T &out_sin, T &out_cos) {
    template<typename SrcT>
  __builtin_sincos(angle_rad, &out_sin, &out_cos);
    inline void constexpr extend(Aabb2<SrcT> const &box) {
}
      if(!valid()) {
inline static void constexpr sincos_any(int const angle_rad, int &out_sin, int &out_cos) __attribute__((__always_inline__));
        min = box.min;
inline static void constexpr sincos_any(int const angle_rad, int &out_sin, int &out_cos) {
        max = box.max;
  // if we're only working with integers, assume single precision is enough
      } else {
  float out_sin_temp = 0.0f;
        min = std::min(min, box.min);
  float out_cos_temp = 0.0f;
        max = std::max(max, box.max);
  __builtin_sincosf(static_cast<float>(angle_rad), &out_sin_temp, &out_cos_temp);
      }
  out_sin = static_cast<int>(out_sin_temp);
    }
  out_cos = static_cast<int>(out_cos_temp);

}
    /**
inline static void constexpr sincos_any(float const angle_rad, float &out_sin, float &out_cos) __attribute__((__always_inline__));
     * Gets a copy of this bounding-box extend by a point @a point.
inline static void constexpr sincos_any(float const angle_rad, float &out_sin, float &out_cos) {
     * @param point A point to extend the box by
  __builtin_sincosf(angle_rad, &out_sin, &out_cos);
     * @return Copy of extended bounding-box
}
     */
inline static void constexpr sincos_any(long double const angle_rad, long double &out_sin, long double &out_cos) __attribute__((__always_inline__));
    template<typename SrcT>
inline static void constexpr sincos_any(long double const angle_rad, long double &out_sin, long double &out_cos) {
    inline constexpr Aabb2<T> extended(Vector2<SrcT> const &point) const {
  __builtin_sincosl(angle_rad, &out_sin, &out_cos);
      Aabb2<T> ret(*this);
}
      ret.extend(point);

      return ret;
inline static float constexpr sqrt_inv_fast(float number) __attribute__((__always_inline__));
    }
inline static float constexpr sqrt_inv_fast(float number) {

  /// Adapted from Quake III's fast inverse square root approximation
    /**
  float constexpr const threehalfs = 1.5f;
     * Gets a copy of this bounding-box extnended by box @a box.

     * @param box A box to extend the copy be.
  float x = number * 0.5f;
  float y = number;
  uint32_t i  = *(uint32_t*)&y;                                                 // evil floating point bit level hacking
     * @return Copy of extended bounding-box
  //i = 0x5f3759df - (i >> 1);                                                    // what the fuck?
     */
  i = 0x5f375a84 - (i >> 1);                                                    // improved magic number from http://jheriko-rtw.blogspot.co.uk/2009/04/understanding-and-improving-fast.html
    template<typename SrcT>
  y = *(float*)&i;
    inline constexpr Aabb2<T> extended(Aabb2<SrcT> const &box) const {
  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
      Aabb2<T> ret(*this);
  y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
      ret.extend(box);
  return y;
      return *this;
}
    }
inline static double constexpr sqrt_inv_fast(double number) __attribute__((__always_inline__));

inline static double constexpr sqrt_inv_fast(double number) {
    /**
  /// Similar to the Quake III fast inverse square root but for doubles
     * Tests if the point @a point is within this bounding-box
  double constexpr const threehalfs = 1.5;
     * @param point A point to be tested

     * @return True if point @a point lies within bounding-box, otherwise false.
  double x = number * 0.5;
     */
  double y = number;
    template<typename SrcT>
  uint64_t i  = *(uint64_t*)&y;                                                 // evil floating point bit level hacking
    inline bool constexpr intersects(Vector2<SrcT> const &point) const {
  //i = 0x5fe6eb50c7b537a9ll - (i >> 1);                                          // even more magic than "what the fuck" number
      return min.x <= point.x && point.x <= max.x &&
  uint64_t constexpr const magic = (uint64_t(0x5fe6eb50) << (8 * 4)) + uint64_t(0xc7b537a9); // hack to produce 0x5fe6eb50c7b537a9ll without triggering -Wlong-long warning
             min.y <= point.y && point.y <= max.y;
  i = magic - (i >> 1);
    }
  y = *(double*)&i;

  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
    /**
  y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
     * Tests if other bounding-box @a box intersects (even partially) with this bounding-box.
  return y;
     * @param box A box to be tested for intersection.
}
     * @return True if there's intersection between boxes, otherwise false.
template<typename T>
     */
inline static T constexpr sqrt_fast(T number) __attribute__((__always_inline__));
    template<typename SrcT>
template<typename T>
    inline bool constexpr intersects(Aabb2<SrcT> const &box) const {
inline static T constexpr sqrt_fast(T number) {
      return max.x >= box.min.x && min.x <= box.max.x &&
  return sqrt_inv_fast(number) * number;
             max.y >= box.min.y && min.y <= box.max.y;
}
    }
inline static long double constexpr sqrt_fast(long double number) __attribute__((__always_inline__));

inline static long double constexpr sqrt_fast(long double number) {
    /**
  // we don't have a way to handle long doubles with the fast approximation, so just cast to double
     * Gets result of intersection of this bounding-box with @a other bounding-box.
  return static_cast<long double>(sqrt_inv_fast(static_cast<double>(number))) * number;
     * In case the boxes don't intersect, the returned bounding-box is invalid.
}
     * @param other Box to be tested
inline static int constexpr sqrt_fast(int number) __attribute__((__always_inline__));
     * @return Result of intersection.
inline static int constexpr sqrt_fast(int number) {
     * @see valid() method for more information on invalid bounding-boxes.
  // convert ints to floats and back
     */
  return static_cast<int>(sqrt_inv_fast(static_cast<float>(number)) * static_cast<float>(number));
    template<typename SrcT>
}
    inline constexpr Aabb2<T> intersection(Aabb2<SrcT> const &other) const {

      return (max.x < other.min.x || min.x > other.max.x ||
inline static float constexpr sqrt_inv_faster(float number) __attribute__((__always_inline__));
              max.y < other.min.y || min.y > other.max.y) ? Aabb2<T>() : Aabb2<T>(std::max(min, other.min), std::min(max, other.max));
inline static float constexpr sqrt_inv_faster(float number) {
    }
  /// Adapted from Quake III's fast inverse square root approximation - one iteration version

  float constexpr const threehalfs = 1.5f;
    /**

     * Tests if a ray @b from origin @a intersects with this bounding-box.
  float x = number * 0.5f;
     * @param origin Origin of the intersecting ray
  float y = number;
     * @param ray The intersecting ray; does not need to be normalised
  uint32_t i  = *(uint32_t*)&y;                                                 // evil floating point bit level hacking
     * @return True if the ray intersects the box, otherwise false.
  //i = 0x5f3759df - (i >> 1);                                                    // what the fuck?
     */
    template<typename SrcT>
  i = 0x5f375a84 - (i >> 1);                                                    // improved magic number from http://jheriko-rtw.blogspot.co.uk/2009/04/understanding-and-improving-fast.html
    inline bool constexpr ray_intersects(Vector2<SrcT> const &ray, Vector2<SrcT> const &origin = Vector2<SrcT>()) const {
  y = *(float*)&i;
      // adapted from http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
      /*
  //y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
      SrcT const dist_min_x = (min.x - origin.x) / ray.x;
  return y;
      SrcT const dist_max_x = (max.x - origin.x) / ray.x;
}
      SrcT dist_min = std::min(dist_min_x, dist_max_x);
inline static double constexpr sqrt_inv_faster(double number) __attribute__((__always_inline__));
      SrcT dist_max = std::max(dist_min_x, dist_max_x);
inline static double constexpr sqrt_inv_faster(double number) {

  /// Similar to the Quake III fast inverse square root but for doubles
      SrcT const dist_min_y = (min.y - origin.y) / ray.y;
  double constexpr const threehalfs = 1.5;
      SrcT const dist_max_y = (max.y - origin.y) / ray.y;

      dist_min = std::max(dist_min, std::min(dist_min_y, dist_max_y));
  double x = number * 0.5;
      dist_max = std::min(dist_max, std::max(dist_min_y, dist_max_y));
  double y = number;

  uint64_t i  = *(uint64_t*)&y;                                                 // evil floating point bit level hacking
      return dist_max >= std::max(dist_min, 0.0f);
  //i = 0x5fe6eb50c7b537a9ll - (i >> 1);                                          // even more magic than "what the fuck" number
      */
      // constexpr-compatible reformulation:
  uint64_t constexpr const magic = (uint64_t(0x5fe6eb50) << (8 * 4)) + uint64_t(0xc7b537a9); // hack to produce 0x5fe6eb50c7b537a9ll without triggering -Wlong-long warning
      return std::min(std::max((min.x - origin.x) / ray.x,
  i = magic - (i >> 1);
                               (max.x - origin.x) / ray.x),
  y = *(double*)&i;
                      std::max((min.y - origin.y) / ray.y,
  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
                               (max.y - origin.y) / ray.y)) >=
  //y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
             std::max(std::max(std::min((min.x - origin.x) / ray.x,
  return y;
                                        (max.x - origin.x) / ray.x),
}
                               std::min((min.y - origin.y) / ray.y,
template<typename T>
                                        (max.y - origin.y) / ray.y)), 0.0f);
inline static T constexpr sqrt_faster(T number) __attribute__((__always_inline__));
    }
template<typename T>

inline static T constexpr sqrt_faster(T number) {
    /**
  return sqrt_inv_faster(number) * number;
     * Gets center point of bounding-box.
}
     * @return A center point of bounding-box.
inline static long double constexpr sqrt_faster(long double number) __attribute__((__always_inline__));
     */
inline static long double constexpr sqrt_faster(long double number) {
    inline constexpr Vector2<T> center() const __attribute__((__always_inline__)) {
  // we don't have a way to handle long doubles with the fast approximation, so just cast to double
      return (min + max) * static_cast<T>(0.5);
  return static_cast<long double>(sqrt_inv_faster(static_cast<double>(number))) * number;
    }
}

inline static int constexpr sqrt_faster(int number) __attribute__((__always_inline__));
    /**
inline static int constexpr sqrt_faster(int number) {
     * Gets extent of bounding-box.
  // convert ints to floats and back
     * @return Extent of bounding-box.
  return static_cast<int>(sqrt_inv_faster(static_cast<float>(number)) * static_cast<float>(number));
     */
}
    inline constexpr Vector2<T> extent() const __attribute__((__always_inline__)) {

      return (max - min) * static_cast<T>(0.5);
inline static float sqrt_inv_sse(float number) __attribute__((__always_inline__));
    }
inline static float sqrt_inv_sse(float number) {

  /// Scalar SSE inverse square root approximation
    /**
  float result;
     * Gets diagonal size of bounding-box
  _mm_store_ss(&result, _mm_rsqrt_ss(_mm_load_ss(&number)));
     * @return Sizes for particular dimensions.
  return result;
     */
}
    inline constexpr Vector2<T> size() const __attribute__((__always_inline__)) {
inline static float sqrt_sse(float number) __attribute__((__always_inline__));
      return max - min;
inline static float sqrt_sse(float number) {
    }
  /// Scalar SSE square root approximation

  return sqrt_inv_sse(number) * number;
    /**
}
     * Gets all 4 corner-points of bounding box

     * @param i An index of bounding-box corner point. Valid values are 0 .. 3.
template<typename T> class Vector2;                                             // forward declarations
     * @return A position of @a i-th corner-point.
template<typename T> class Vector3;
     * @note The order of points is as follows (where @c + denotes max-point and @c - min-point):
template<typename T> class Vector4;
     * 1. (@c + @c + @c +)
template<typename T> class Matrix3;
     * 2. (@c - @c + @c +)
template<typename T> class Matrix4;
     * 3. (@c + @c - @c +)
template<typename T> class Quaternion;
     * 4. (@c - @c - @c +)
template<typename T> class Aabb2;
     *
template<typename T> class Aabb3;
     */

/**
    inline constexpr Vector2<T> point(unsigned int i) const __attribute__((__always_inline__)) {
 * Class for two dimensional vector.
      return Vector2<T>(i & 1 ? min.x : max.x,
 * There are three ways of accessing vector components.
                        i & 2 ? min.y : max.y);
 * Let's have <code>Vector2f v</code>, you can either:
    }
 * <ul>

 *  <li>access as position(x,y) &mdash; <code>v.x = v.y = 3;</code></li>
    //-------------------------------------------------------------------------------------------------------------
 *  <li>access as texture coordinate (s,t) &mdash; <code>v.s = v.t = 3;</code></li>
    // operators
 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = 3;</code></li>
    //-------------------------------------------------------------------------------------------------------------
 * </ul>
    /**
 */
     * Tests if @a rhs is equal to this bounding-box
template<typename T>
     * @param rhs Right-hand side
class Vector2 {
     * @return True if @a rhs and this bounding-boxes are equal, otherwise false
  public:
     */
    union {
    template<typename RhsT>
      /**
    inline bool constexpr operator==(Aabb2<RhsT> const &rhs) const {
       * First element of vector, alias for X-coordinate.
      return min == rhs.min && max == rhs.max;
       */
    }
      T x;


    /**
      /**
     * Tests if @a rhs is not equal to this bounding-box
       * First element of vector, alias for S-coordinate.
     * @param rhs Right-hand side
       * For textures notation.
     * @return True if @a rhs and this bounding-boxes are not equal, otherwise false
       */
     */
      T s;
    template<typename RhsT>
    };
    inline bool constexpr operator!=(Aabb2<RhsT> const &rhs) const {

      return min != rhs.min || max != rhs.max;
    union {
    }
      /**

       * Second element of vector, alias for Y-coordinate.
    /**
       */
     * Extends this bounding-box by point @a rhs.
      T y;

     * @param rhs A point to extend this bounding-box by
      /**
     * @return Reference to this
       * Second element of vector, alias for Z-coordinate (for x-z 2D grids).
     */
       */
    template<typename SrcT>
      T z;
    inline Aabb2<T> constexpr &operator<<(Vector2<SrcT> const &rhs) {

      extend(rhs);
      /**
      return *this;
       * Second element of vector, alias for T-coordinate.
    }
       * For textures notation.

       */
    /**
      T t;
     * Extends this bounding-box by box @a rhs.
    };
     * @param rhs A box to extend this bounding-box by

     * @return Reference to this
    //----------------[ constructors ]--------------------------
     */
    /**
    template<typename SrcT>
     * Creates and sets to (0,0)
    inline Aabb2<T> constexpr &operator<<(Aabb2<SrcT> const &rhs) {
     */
      extend(rhs);
    inline constexpr Vector2() __attribute__((__always_inline__))
      return *this;
      : x(0), y(0) {
    }
    }


    /**
    /**
     * Union of this and @a rhs bounding-boxes
     * Creates and sets to (x,y)
     * @param rhs Right-hand side of union
     * @param nx initial x-coordinate value
     * @return A resulting bounding-box representing union
     * @param ny initial y-coordinate value
     */
     */
    template<typename RhsT>
    inline constexpr Vector2(T nx, T ny) __attribute__((__always_inline__))
    inline Aabb2<T> constexpr operator|(Aabb2<RhsT> const &rhs) const {
      : x(nx), y(ny) {
      return extended(rhs);
    }
    }


    /**
    /**
     * Copy constructor.
     * Intersection of this and @a rhs bounding-boxed
     * @param src Source of data for new created instance.
     * @param rhs Right-hand side
     */
     * @return Resulting bounding-box representing the intersection.
    inline constexpr Vector2(Vector2<T> const &src) __attribute__((__always_inline__))
     */
      : x(src.x), y(src.y) {
    template<typename RhsT>
    }

    inline Aabb2<T> constexpr operator&(Aabb2<RhsT> const &rhs) const {
    /**
      return intersection(rhs);
     * Copy casting constructor.
    }
     * @param src Source of data for new created instance.

     */
    /**
    template<typename FromT>
     * Outputs string representation of bounding-box @a rhs to output stream @a lhs
    inline constexpr Vector2(Vector2<FromT> const &src)
     * @param lhs Output stream to write to
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)) {
     * @param rhs Bounding-box to write to output stream.
    }
     * @return Reference to output stream @a lhs

     */
    //----------------[ assignment ]-------------------------
    inline friend std::ostream &operator<<(std::ostream &lhs, const Aabb2<T> &rhs) __attribute__((__always_inline__)) {
    /**
      lhs << rhs.min << " x " << rhs.max;
     * Sets to (x,y)
      return lhs;
     * @param nx initial x-coordinate value
    }
     * @param ny initial y-coordinate value
};
     */

    inline void constexpr assign(T nx = 0, T ny = 0) __attribute__((__always_inline__)) {
/**
      x = nx;
 * Three-dimensional axis-aligned bounding-box (aka AABB) class.
      y = ny;
 *
    }
 * This class provides functionality for:

 * - creating an AABB from a point, or another AABB,
    //----------------[ access operators ]-------------------
 * - testing if a point of other AABB intersects with it,
    /**
 * - getting result of intersection with other AABB,
     * Copy casting operator
 * - transforming AABB with 4x4 matrix.
     * @param rhs Right hand side argument of binary operator.
 *
     */
 * There are also overloaded couple of operators to shorten common operations.
    template<typename FromT>
 * For instance you can use  @c operator<< on AABB to extend it with a passed point or other AABB.
    inline Vector2<T> constexpr &operator=(Vector2<FromT> const &rhs) {
 * @code
      x = static_cast<T>(rhs.x);
 * Aabb3f aabb;
      y = static_cast<T>(rhs.y);
 * aabb << Vector3f(1, 1, 2) << Aabb3f(-3,-3,-3, 2, 2, 2);
      return *this;
 * @endcode
    }
 */

template<typename T>
    /**
class Aabb3 {
     * Copy operator
  public:
     * @param rhs Right hand side argument of binary operator.
    /**
     */
     * Position of Min corner of bounding box.
    inline Vector2<T> constexpr &operator=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
     */
      x = rhs.x;
    Vector3<T> min;
      y = rhs.y;

      return *this;
    /**
    }

     * Position of Max corner of bounding box
    /**
     */
     * Array access operator
    Vector3<T> max;
     * @param n Array index

     * @return For n = 0, reference to x coordinate, else reference to y
    /**
     * y coordinate.
     * Constructs invalid axes-aligned bounding-box.
     */
     * @see valid() for explanation of invalid bounding-box usage.
    inline T constexpr &operator[](int n) __attribute__((__always_inline__)) {
     */
      return n == 0 ? x : y;
    inline constexpr Aabb3() __attribute__((__always_inline__))
    }
      : min(1, 1, 1), max(-1, -1, -1) {

    }
    /**

     * Constant array access operator
    /**
     * @param n Array index
     * Constructs axes-aligned bound-box containing one point @a point
     * @return For n = 0, reference to x coordinate, else reference to y
     * @param point
     * y coordinate.
     */
     */
    template<typename SrcT>
    inline T constexpr const &operator[](int n) const __attribute__((__always_inline__)) {
    inline constexpr Aabb3(Vector3<SrcT> const &point)
      return n == 0 ? x : y;
      : min(point), max(point) {
    }
    }


    //---------------[ vector aritmetic operator ]--------------
    /**
    /**
     * Constructs axes-aligned bounding-box from two corner points (@a x0, @a y0, @a z0)
     * Addition operator
     * and (@a x1, @a y1, @a z1)
     * @param rhs Right hand side argument of binary operator.
     * @param x0 X-coordinate of first point
     */
     * @param y0 Y-coordinate of first point
    inline Vector2<T> constexpr operator+(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
     * @param z0 Z-coordinate of first point
      return Vector2<T>(x + rhs.x, y + rhs.y);
     * @param x1 X-coordinate of second point
    }
     * @param y1 Y-coordinate of second point

     * @param z1 Z-coordinate of second point
    /**
     */
     * Subtraction operator
    template<typename SrcT>
     * @param rhs Right hand side argument of binary operator.
    inline constexpr Aabb3(SrcT x0, SrcT y0, SrcT z0, SrcT x1, SrcT y1, SrcT z1)
     */
      : min(std::min(x0, x1), std::min(y0, y1), std::min(z0, z1)),
    inline Vector2<T> constexpr operator-(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
        max(std::max(x0, x1), std::max(y0, y1), std::max(z0, z1)) {
      return Vector2<T>(x - rhs.x, y - rhs.y);
    }
    }


    /**
    /**
     * Constructs axes-aligned bounding-box from @a min and @b max
     * Multiplication operator
     * @param min X-coordinate of first point
     * @param rhs Right hand side argument of binary operator.
     * @param max X-coordinate of second point
     */
     */
    inline Vector2<T> constexpr operator*(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
    template<typename SrcT>
      return Vector2<T>(x * rhs.x, y * rhs.y);
    inline constexpr Aabb3(Vector3<SrcT> const &new_min, Vector3<SrcT> const &new_max)
    }
      : min(new_min),

        max(new_max) {
    /**
    }
     * Division operator

     * @param rhs Right hand side argument of binary operator.
    /**
     */
     * Constructs axes-aligned bounding-box containing point (@a x, @a y, @a z)
    inline Vector2<T> constexpr operator/(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
     * @param x X-coordinate of point
      return Vector2<T>(x / rhs.x, y / rhs.y);
     * @param y Y-coordinate of point
    }

     * @param z Z-coordinate of point
    /**
     */
     * Addition operator
    template<typename SrcT>
     * @param rhs Right hand side argument of binary operator.
    inline constexpr Aabb3(SrcT x, SrcT y, SrcT z)
     */
      : min(x, y, z), max(x, y, z) {
    inline Vector2<T> constexpr &operator+=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
    }
      x += rhs.x;

      y += rhs.y;
    /**
      return *this;
     * Creates copy of axis-aligned bounding-box
    }
     * @param src Source bounding-box

     */
    /**
    template<typename SrcT>
     * Substraction operator
    inline constexpr Aabb3(Aabb3<SrcT> const &src)
     * @param rhs Right hand side argument of binary operator.
      : min(src.min), max(src.max) {
     */
    }
    inline Vector2<T> constexpr &operator-=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {

      x -= rhs.x;
    /**
      y -= rhs.y;
     * Assign operator
      return *this;
     * @param rhs source bounding-box
    }
     * @return refenrence to this

     */
    /**
    template<typename SrcT>
     * Multiplication operator
    inline Aabb3<T> constexpr &operator=(Aabb3<SrcT> const &rhs) {
     * @param rhs Right hand side argument of binary operator.
      min = rhs.min;
     */
      max = rhs.max;
    inline Vector2<T> constexpr &operator*=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      return *this;
      x *= rhs.x;
    }
      y *= rhs.y;

      return *this;
    /**
    }
     * Checks if bounding-box is valid. Valid bounding-box has non-negative size.

     * If an invalid bounding-box is extended by point or another bounding-box, the target
    /**
     * bounding box becomes valid and contains solely the source point or bounding-box respectively.
     * Division operator
     * @return True if box is valid, otherwise false
     * @param rhs Right hand side argument of binary operator.
     */
     */
    inline bool constexpr valid() const __attribute__((__always_inline__)) {
    inline Vector2<T> constexpr &operator/=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      return min.x <= max.x && min.y <= max.y && min.z <= max.z;
      x /= rhs.x;
    }
      y /= rhs.y;

      return *this;
    /**
    }

     * Makes this bounding-box invalid. So calling valid() gets false.
    /**
     * @see valid() method for more info on usage of invalid bounding-boxes.
     * Dot product of two vectors.
     */
     * @param rhs Right hand side argument of binary operator.
    inline void constexpr invalidate() __attribute__((__always_inline__)) {
     */
      min = Vector3<T>(1, 1, 1);
    inline T constexpr dotProduct(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      max = Vector3<T>(-1, -1, -1);
      return x * rhs.x + y * rhs.y;
    }
    }


    /**
    /**
     * Extends this bounding-box by a point @a point.
     * Cross product of two vectors
     * @param point A point to extend bounding-box by.
     * @param rhs Right hand side argument of binary operator.
     */
     */
    template<typename SrcT>
    inline T constexpr crossProduct(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
    inline void constexpr extend(Vector3<SrcT> const &point) {
      return x * rhs.y - y * rhs.x;
      if(!valid()) {
    }
        min = max = point;

      } else {
    //--------------[ scalar vector operator ]--------------------
        min = std::min(min, point);
    /**
        max = std::max(max, point);
     * Addition operator
      }
     * @param rhs Right hand side argument of binary operator.
    }
     */

    inline Vector2<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
    /**
      return Vector2<T>(x + rhs, y + rhs);
     * Extends this bounding-box by a box @a box.
    }
     * @param box A box to extend this bounding-box by.

     */
    /**
    template<typename SrcT>
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline void constexpr extend(Aabb3<SrcT> const &box) {
    inline Vector2<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return Vector2<T>(x - rhs, y - rhs);
      if(!valid()) {
    }
        min = box.min;

        max = box.max;
    /**
      } else {
     * Multiplication operator
        min = std::min(min, box.min);
     * @param rhs Right hand side argument of binary operator.
        max = std::max(max, box.max);
     */
      }
    inline Vector2<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
    }
      return Vector2<T>(x * rhs, y * rhs);

    }
    /**

     * Gets a copy of this bounding-box extend by a point @a point.
    /**
     * @param point A point to extend the box by
     * Division operator
     * @return Copy of extended bounding-box
     * @param rhs Right hand side argument of binary operator.
     */
     */
    template<typename SrcT>
    inline Vector2<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
    inline constexpr Aabb3<T> extended(Vector3<SrcT> const &point) const {
      return Vector2<T>(x / rhs, y / rhs);
      Aabb3<T> ret(*this);
    }
      ret.extend(point);

      return ret;
    /**
    }
     * Addition operator

     * @param rhs Right hand side argument of binary operator.
    /**
     */
     * Gets a copy of this bounding-box extnended by box @a box.
    inline Vector2<T> constexpr &operator+=(T rhs) __attribute__((__always_inline__)) {
     * @param box A box to extend the copy be.
      x += rhs;
     * @return Copy of extended bounding-box
      y += rhs;
     */
      return *this;
    template<typename SrcT>
    }
    inline constexpr Aabb3<T> extended(Aabb3<SrcT> const &box) const {

      Aabb3<T> ret(*this);
    /**
      ret.extend(box);
     * Subtraction operator
      return *this;
     * @param rhs Right hand side argument of binary operator.
    }
     */

    inline Vector2<T> constexpr &operator-=(T rhs) __attribute__((__always_inline__)) {
    /**
      x -= rhs;
     * Tests if the point @a point is within this bounding-box
      y -= rhs;
     * @param point A point to be tested
      return *this;
     * @return True if point @a point lies within bounding-box, otherwise false.
    }
     */

    template<typename SrcT>
    /**
    inline bool constexpr intersects(Vector3<SrcT> const &point) const {
     * Multiplication operator
      return min.x <= point.x && point.x <= max.x &&
     * @param rhs Right hand side argument of binary operator.
             min.y <= point.y && point.y <= max.y &&
     */
             min.z <= point.z && point.z <= max.z;
    inline Vector2<T> constexpr &operator*=(T rhs) __attribute__((__always_inline__)) {
    }
      x *= rhs;

      y *= rhs;
    /**
      return *this;
    }
     * Tests if other bounding-box @a box intersects (even partially) with this bounding-box.

     * @param box A box to be tested for intersection.
    /**
     * @return True if there's intersection between boxes, otherwise false.
     * Division operator
     */
     * @param rhs Right hand side argument of binary operator.
    template<typename SrcT>
     */
    inline bool constexpr intersects(Aabb3<SrcT> const &box) const {
    inline Vector2<T> constexpr &operator/=(T rhs) __attribute__((__always_inline__)) {
      return max.x >= box.min.x && min.x <= box.max.x &&
      x /= rhs;
             max.y >= box.min.y && min.y <= box.max.y &&
      y /= rhs;
             max.z >= box.min.z && min.z <= box.max.z;
      return *this;
    }
    }


    /**
    //--------------[ equality operator ]------------------------
     * Gets result of intersection of this bounding-box with @a other bounding-box.
    /**
     * In case the boxes don't intersect, the returned bounding-box is invalid.
     * Equality test operator
     * @param other Box to be tested
     * @param rhs Right hand side argument of binary operator.
     * @return Result of intersection.
     * @note Test of equality is based of threshold epsilon value. To be two
     * @see valid() method for more information on invalid bounding-boxes.
     * values equal, must satisfy this condition | lhs.x - rhs.y | < epsilon,
     */
     * same for y-coordinate.
    template<typename SrcT>
     */
    inline constexpr Aabb3<T> intersection(Aabb3<SrcT> const &other) const {
    inline bool constexpr operator==(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return (max.x < other.min.x || min.x > other.max.x ||
      #ifdef VMATH_SOFT_COMPARE
              max.y < other.min.y || min.y > other.max.y ||
        return (std::abs(x - rhs.x) < static_cast<T>(epsilon)) &&
              max.z < other.min.z || min.z > other.max.z) ? Aabb3<T>() : Aabb3<T>(std::max(min, other.min), std::min(max, other.max));
               (std::abs(y - rhs.y) < static_cast<T>(epsilon));
    }
      #else

        return x == rhs.x &&
    /**
               y == rhs.y;
     * Tests if a ray @b from origin @a intersects with this bounding-box.
      #endif
    }
     * @param ray The intersecting ray; does not need to be normalised

     * @param origin Origin of the intersecting ray
    /**
     * @return True if the ray intersects the box, otherwise false.
     * Inequality test operator
     */
     * @param rhs Right hand side argument of binary operator.
    template<typename SrcT>
     * @return not (lhs == rhs) :-P
    inline bool constexpr ray_intersects(Vector3<SrcT> const &ray, Vector3<SrcT> const &origin = Vector3<SrcT>()) const {
     */
      // adapted from http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
    inline bool constexpr operator!=(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      /*
      return !(*this == rhs);
      SrcT const dist_min_x = (min.x - origin.x) / ray.x;
    }
      SrcT const dist_max_x = (max.x - origin.x) / ray.x;

      SrcT dist_min = std::min(dist_min_x, dist_max_x);
    //-------------[ unary operations ]--------------------------
      SrcT dist_max = std::max(dist_min_x, dist_max_x);
    /**

     * Unary negate operator
      SrcT const dist_min_y = (min.y - origin.y) / ray.y;
     * @return negated vector
      SrcT const dist_max_y = (max.y - origin.y) / ray.y;
     */
      dist_min = std::max(dist_min, std::min(dist_min_y, dist_max_y));
    inline Vector2<T> constexpr operator-() const __attribute__((__always_inline__)) {
      dist_max = std::min(dist_max, std::max(dist_min_y, dist_max_y));
      return Vector2<T>(-x, -y);

    }
      SrcT const dist_min_z = (min.z - origin.z) / ray.z;

      SrcT const dist_max_z = (max.z - origin.z) / ray.z;
    //-------------[ size operations ]---------------------------
      dist_min = std::max(dist_min, std::min(dist_min_z, dist_max_z));
    /**
      dist_max = std::min(dist_max, std::max(dist_min_z, dist_max_z));
     * Return square of length.

     * @return length ^ 2
      return dist_max >= std::max(dist_min, 0.0f);
     * @note This method is faster then length(). For comparison
      */
     * of length of two vector can be used just this value, instead
      // constexpr-compatible reformulation:
     * of more expensive length() method.
      return std::min(std::min(std::max((min.x - origin.x) / ray.x,
     */
                                        (max.x - origin.x) / ray.x),
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
                               std::max((min.y - origin.y) / ray.y,
      return x * x + y * y;
                                        (max.y - origin.y) / ray.y)),
    }
                      std::max((min.z - origin.z) / ray.z,

                               (max.z - origin.z) / ray.z)) >=
    /**
             std::max(std::max(std::max(std::min((min.x - origin.x) / ray.x,
     * Get length of vector.
                                                 (max.x - origin.x) / ray.x),
     * @return length of vector
     */
                                        std::min((min.y - origin.y) / ray.y,
    inline T constexpr length() const __attribute__((__always_inline__)) {
                                                 (max.y - origin.y) / ray.y)),
      return static_cast<T>(std::sqrt(lengthSq()));
                               std::min((min.z - origin.z) / ray.z,
    }
                                        (max.z - origin.z) / ray.z)), 0.0f);
    /**
    }
     * Get length of vector, fast approximation.

     * @return length of vector
    /**
     */
     * Gets center point of bounding-box.
    inline T constexpr length_fast() const __attribute__((__always_inline__)) {
     * @return A center point of bounding-box.
      return static_cast<T>(sqrt_fast(lengthSq()));
     */
    }
    inline Vector3<T> constexpr center() const __attribute__((__always_inline__)) {
    /**
      return (min + max) * static_cast<T>(0.5);
     * Get length of vector, rougher fast approximation.
    }
     * @return length of vector

     */
    /**
    inline T constexpr length_faster() const __attribute__((__always_inline__)) {
     * Gets extent of bounding-box.
      return static_cast<T>(sqrt_faster(lengthSq()));
     * @return Extent of bounding-box.
    }
     */

    inline Vector3<T> constexpr extent() const __attribute__((__always_inline__)) {
    /**
      return (max - min) * static_cast<T>(0.5);
     * normalise vector
    }
     */

    inline void constexpr normalise() __attribute__((__always_inline__)) {
    /**
      T const temp = length();
     * Gets diagonal size of bounding-box
      x /= temp;
     * @return Sizes for particular dimensions.
      y /= temp;
     */
    }
    inline Vector3<T> constexpr size() const __attribute__((__always_inline__)) {
    inline void constexpr normalise_fast() __attribute__((__always_inline__)) {
      return max - min;
      T const temp = length_fast();
    }
      x /= temp;

      y /= temp;
    /**
    }
     * Gets all 8 corner-points of bounding box
    inline void constexpr normalise_faster() __attribute__((__always_inline__)) {
     * @param i An index of bounding-box corner point. Valid values are 0 .. 7.
      T const temp = length_faster();
     * @return A position of @a i-th corner-point.
      x /= temp;
     * @note The order of points is as follows (where @c + denotes max-point and @c - min-point):
      y /= temp;
     * 1. (@c + @c + @c +)
    }
     * 2. (@c - @c + @c +)
    inline void constexpr normalize() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
     * 3. (@c + @c - @c +)
      normalise();
     * 4. (@c - @c - @c +)
    }
     * 5. (@c + @c + @c -)
    inline void constexpr normalize_fast() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
     * 6. (@c - @c + @c -)
      normalise_fast();
     * 7. (@c + @c - @c -)
    }
     * 8. (@c - @c - @c -)
    inline void constexpr normalize_faster() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
     *
      normalise_faster();
     */
    }
    inline Vector3<T> constexpr point(unsigned int i) const __attribute__((__always_inline__)) {
    inline Vector2<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {
      return Vector3<T>(i & 1 ? min.x : max.x, i & 2 ? min.y : max.y, i & 4 ? min.z : max.z);
      T const temp(length());
    }
      return Vector2<T>(x / temp, y / temp);

    }
    /**
    inline Vector2<T> constexpr normalise_copy_fast() const __attribute__((__always_inline__)) {
     * Gets transformed bounding-box by transform @a t
      T const temp(length_fast());
     * @param t A transform matrix
      return Vector2<T>(x / temp, y / temp);
     * @return Transformed bounding-box
    }
     */
    inline Vector2<T> constexpr normalise_copy_faster() const __attribute__((__always_inline__)) {
    inline Aabb3<T> constexpr transformed(Matrix4<T> const &t) const __attribute__((__always_inline__)) {
      T const temp(length_faster());
      Aabb3<T> ret;
      return Vector2<T>(x / temp, y / temp);
      for(unsigned int i = 0; i != 8; ++i) {
    }
        const Vector4<T> p(point(i), 1);
    inline Vector2<T> constexpr normalize_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
        ret.extend((t * p).xyz());
      return normalise_copy();
      }
    }
      return ret;
    inline Vector2<T> constexpr normalize_copy_fast() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
    }
      return normalise_copy_fast();

    }
    //-------------------------------------------------------------------------------------------------------------
    inline Vector2<T> constexpr normalize_copy_faster() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
    // operators
      return normalise_copy_faster();
    //-------------------------------------------------------------------------------------------------------------
    }
    /**
    /**
     * Tests if @a rhs is equal to this bounding-box
     * normalise vector. with added zero safety check
     * @param rhs Right-hand side
     */
     * @return True if @a rhs and this bounding-boxes are equal, otherwise false
    inline void constexpr normalise_safe() __attribute__((__always_inline__)) {
     */
      T const temp = length();
    template<typename RhsT>
      if(temp == static_cast<T>(0)) {
    inline bool constexpr operator==(Aabb3<RhsT> const &rhs) const {
        x = static_cast<T>(0);
      return min == rhs.min && max == rhs.max;
        y = static_cast<T>(0);
    }
      } else {

        x /= temp;
    /**
        y /= temp;
     * Tests if @a rhs is not equal to this bounding-box
      }
     * @param rhs Right-hand side
    }
     * @return True if @a rhs and this bounding-boxes are not equal, otherwise false
    inline void constexpr normalize_safe() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
     */
      normalise_safe();
    template<typename RhsT>
    }
    inline bool constexpr operator!=(Aabb3<RhsT> const &rhs) const {
    inline Vector2<T> constexpr normalise_safe_copy() const __attribute__((__always_inline__)) {
      return min != rhs.min || max != rhs.max;
      T const temp(length());
    }
      if(temp == static_cast<T>(0)) {

        return Vector2<T>();
    /**
      } else {
     * Gets transformed bounding-box by transform @a rhs.
        return Vector2<T>(x / temp, y / temp);
     * @param rhs Matrix 4x4 representing the transform
      }
     * @return Transformed bounding-box
    }
     */
    inline Vector2<T> constexpr normalize_safe_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
    inline Aabb3<T> constexpr operator*(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
      return normalise_safe_copy();
      return transformed(rhs);
    }
    }


    /**
    /**
     * Absolute vector (make all values positive)
     * Apply transform @a rhs to this bounding-box
     */
     * @param rhs A transform to be applied
    inline void constexpr abs() __attribute__((__always_inline__)) {
     * @return Reference to this
      x = std::abs(x);
     */
      y = std::abs(y);
    }
    inline Aabb3<T> constexpr &operator*=(Matrix4<T> const &rhs) __attribute__((__always_inline__)) {
    inline Vector2<T> constexpr abs_copy() const __attribute__((__always_inline__)) {
      *this = transformed(rhs);
      return Vector2<T>(std::abs(x), std::abs(y));
      return *this;
    }
    }


    //-------------[ other operations ]--------------------------
    /**
    /**
     * Extends this bounding-box by point @a rhs.
     * Rotate 2D vector clockwise.
     * @param rhs A point to extend this bounding-box by
     * @param aa Angle (in degrees) to be rotated.
     * @return Reference to this
     */
     */
    inline void constexpr rotate(T aa) __attribute__((__always_inline__)) {
    template<typename SrcT>
      rotate_rad(deg2rad(aa));
    inline Aabb3<T> constexpr &operator<<(Vector3<SrcT> const &rhs) {
    }
      extend(rhs);

      return *this;
    /**
    }
     * Rotate 2D vector clockwise, radian version.

     * @param aa Angle (in radians) to be rotated.
    /**
     */
     * Extends this bounding-box by box @a rhs.
    inline void constexpr rotate_rad(T aa) __attribute__((__always_inline__)) {
     * @param rhs A box to extend this bounding-box by
      T temp_sin = static_cast<T>(0);
     * @return Reference to this
      T temp_cos = static_cast<T>(0);
     */
      sincos_any(-aa, temp_sin, temp_cos);
    template<typename SrcT>
      T const nx = static_cast<T>((temp_cos * x) - (temp_sin * y));
    inline Aabb3<T> constexpr &operator<<(Aabb3<SrcT> const &rhs) {
      T const ny = static_cast<T>((temp_cos * y) + (temp_sin * x));
      extend(rhs);
      x = nx;
      return *this;
      y = ny;
    }
    }


    /**
    /**
     * Union of this and @a rhs bounding-boxes
     * Linear interpolation of two vectors
     * @param rhs Right-hand side of union
     * @param fact Factor of interpolation. For translation from position
     * @return A resulting bounding-box representing union
     * of this vector to vector r, values of factor goes from 0.0 to 1.0.
     */
     * @param r Second Vector for interpolation
    template<typename RhsT>
     * @note However values of fact parameter are reasonable only in interval
    inline Aabb3<T> constexpr operator|(Aabb3<RhsT> const &rhs) const {
     * [0.0 , 1.0], you can pass also values outside of this interval and you
      return extended(rhs);
     * can get result (extrapolation?)
    }
     */

    /**
    inline Vector2<T> constexpr lerp(T fact, Vector2<T> const &r) const __attribute__((__always_inline__)) {
     * Intersection of this and @a rhs bounding-boxed
      return (*this) + (r - (*this)) * fact;
     * @param rhs Right-hand side
    }

     * @return Resulting bounding-box representing the intersection.
    //-------------[ conversion ]-----------------------------
     */
    /**
    template<typename RhsT>
     * Conversion to pointer operator
    inline Aabb3<T> constexpr operator&(Aabb3<RhsT> const &rhs) const {
     * @return Pointer to internally stored (in management of class Vector2<T>)
      return intersection(rhs);
     * used for passing Vector2<T> values to gl*2[fd] functions.
    }
     */

    inline constexpr operator T*() __attribute__((__always_inline__)) {
    /**
      return reinterpret_cast<T*>(this);
     * Outputs string representation of bounding-box @a rhs to output stream @a lhs
    }
     * @param lhs Output stream to write to
    /**
     * @param rhs Bounding-box to write to output stream.
     * Conversion to pointer operator
     * @return Reference to output stream @a lhs
     * @return Constant Pointer to internally stored (in management of class Vector2<T>)
     */
     * used for passing Vector2<T> values to gl*2[fd] functions.
    inline friend std::ostream &operator<<(std::ostream &lhs, const Aabb3<T> &rhs) __attribute__((__always_inline__)) {
     */
      lhs << rhs.min << " x " << rhs.max;
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
      return lhs;
      return reinterpret_cast<T const*>(this);
    }
    }
};


    //-------------[ output operator ]------------------------
/// 2D axis-aligned bounding box of floats
    /**
using Aabb2f = Aabb2<float>;
     * Output to stream operator
/// 2D axis-aligned bounding box of doubles
     * @param lhs Left hand side argument of operator (commonly ostream instance).
using Aabb2d = Aabb2<double>;
     * @param rhs Right hand side argument of operator.
/// 2D axis-aligned bounding box of long doubles
     * @return Left hand side argument - the ostream object passed to operator.
using Aabb2ld = Aabb2<long double>;
     */
/// 2D axis-aligned bounding box of integers
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector2<T> const &rhs) __attribute__((__always_inline__)) {
using Aabb2i = Aabb2<int>;
      lhs << "[" << rhs.x << "," << rhs.y << "]";

      return lhs;
/// 3D axis-aligned bounding box of floats
    }
using Aabb3f = Aabb3<float>;

/// 3D axis-aligned bounding box of doubles
    /**
using Aabb3d = Aabb3<double>;
     * Gets string representation.
/// 3D axis-aligned bounding box of long doubles
     */
using Aabb3ld = Aabb3<long double>;
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
/// 2D axis-aligned bounding box of integers
      std::ostringstream oss;
using Aabb3i = Aabb3<int>;
      oss << *this;

      return oss.str();
#ifdef VMATH_NAMESPACE
    }
}

#endif //VMATH_NAMESPACE
    /**

     * Check whether two lines intersect, and if so obtain the intersection point
#endif // __vmath_Header_File__
     * and store it in this vector.

     * @param line1start The start coordinates of the first line
     * @param line1end The end coordinates of the first line
     * @param line2start The start coordinates of the first line
     * @param line2end The end coordinates of the first line
     */
    template<typename FromT>
    inline bool constexpr get_line_intersection(Vector2<FromT> const &line1start,
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
    template<typename FromT>
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

    /**
     * Gets a 3D vector equivalent populating the X and Y axes
     */
    inline Vector3<T> constexpr to_3D_XY() const __attribute__((__always_inline__)) {
      return Vector3<T>(x, y, 0);
    }
    inline Vector3<T> constexpr to_3d_xy() const __attribute__((__always_inline__)) {
      return Vector3<T>(x, y, 0);
    }

    /**
     * Gets a 3D vector equivalent populating the X and Z axes
     */
    inline Vector3<T> constexpr to_3D_XZ() const __attribute__((__always_inline__)) {
      return Vector3<T>(x, 0, z);
    }
    inline Vector3<T> constexpr to_3d_xz() const __attribute__((__always_inline__)) {
      return Vector3<T>(x, 0, z);
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
 *  <li>access as colour (r,g,b) &mdash; <code>v.r = v.g = v.b = 1;</code></li>
 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = 1;</code></li>
 * </ul>
 */
template<typename T>
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
       * For colour notation.
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
       * For colour notation.
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
       * For colour notation.
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
    template<typename FromT>
    inline constexpr Vector3(Vector3<FromT> const &src)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)) {
    }

    inline constexpr Vector3(Vector2<T> const &src, T new_z)
      : x(src.x), y(src.y), z(new_z) {
    }

    template<typename FromT>
    inline constexpr Vector3(Vector2<FromT> const &src, FromT new_z)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(new_z)) {
    }

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     */
    inline void constexpr assign(T nx = 0, T ny = 0, T nz = 0) __attribute__((__always_inline__)) {
      x = nx;
      y = ny;
      z = nz;
    }

    //----------------[ access operators ]-------------------
    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
      return *this;
    }

    /**
     * Copy casting operator.
     * @param rhs Right hand side argument of binary operator.
     */
    template<typename FromT>
    inline Vector3<T> constexpr operator=(Vector3<FromT> const &rhs) {
      x = static_cast<T>(rhs.x);
      y = static_cast<T>(rhs.y);
      z = static_cast<T>(rhs.z);
      return *this;
    }

    /**
     * Copy operator to Vector3
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
      return *this;
    }

    /**
     * Copy casting operator to Vector3
     * @param rhs Right hand side argument of binary operator.
     */
    template<typename FromT>
    inline Vector3<T> constexpr operator=(Vector4<FromT> const &rhs) {
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
    inline T constexpr &operator[](int n) __attribute__((__always_inline__)) {
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
    inline Vector3<T> constexpr &operator+=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator-=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator*=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator/=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
      z /= rhs.z;
      return *this;
    }

    /**
     * Addition operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator+(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x + rhs.x, y + rhs.y, z);
    }

    /**
     * Subtraction operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator-(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x - rhs.x, y - rhs.y, z);
    }

    /**
     * Multiplication operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator*(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x * rhs.x, y * rhs.y, z);
    }

    /**
     * Division operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr operator/(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector3<T>(x / rhs.x, y / rhs.y, z);
    }

    /**
     * Addition operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator+=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }

    /**
     * Subtraction operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator-=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x -= rhs.x;
      y -= rhs.y;
      return *this;
    }

    /**
     * Multiplication operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator*=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x *= rhs.x;
      y *= rhs.y;
      return *this;
    }

    /**
     * Division operator with a 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator/=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
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
      return *this + (rhs.v.crossProduct(*this) * static_cast<T>(2) * rhs.w) + rhs.v.crossProduct(rhs.v.crossProduct(*this) * static_cast<T>(2));
    }

    /**
     * Multiplication by quaternion operator (rotation by quaternion)
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator*=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
      Vector3<T> const temp = rhs.v.crossProduct(*this) * static_cast<T>(2);
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
    inline Vector3<T> constexpr &operator+=(T rhs) __attribute__((__always_inline__)) {
      x += rhs;
      y += rhs;
      z += rhs;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator-=(T rhs) __attribute__((__always_inline__)) {
      x -= rhs;
      y -= rhs;
      z -= rhs;
      return *this;
    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator*=(T rhs) __attribute__((__always_inline__)) {
      x *= rhs;
      y *= rhs;
      z *= rhs;
      return *this;
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator/=(T rhs) __attribute__((__always_inline__)) {
      x /= rhs;
      y /= rhs;
      z /= rhs;
      return *this;
    }

    //-------------[ matrix operations ]-------------------------

    /**
     * Multiplication by 3x3 matrix operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator*=(Matrix3<T> rhs) __attribute__((__always_inline__)) {
      *this = rhs * *this;
      return *this;
    }

    /**
     * Multiplication by 4x4 matrix operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector3<T> constexpr &operator*=(Matrix4<T> rhs) __attribute__((__always_inline__)) {
      *this = rhs * *this;
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
        return std::abs(x - rhs.x) < static_cast<T>(epsilon) &&
               std::abs(y - rhs.y) < static_cast<T>(epsilon) &&
               std::abs(z - rhs.z) < static_cast<T>(epsilon);
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
     * Get length of vector.
     * @return length of vector
     */
    inline T constexpr length() const __attribute__((__always_inline__)) {
      return static_cast<T>(std::sqrt(lengthSq()));
    }
    /**
     * Get length of vector, fast approximation.
     * @return length of vector
     */
    inline T constexpr length_fast() const __attribute__((__always_inline__)) {
      return static_cast<T>(sqrt_fast(lengthSq()));
    }
    /**
     * Get length of vector, rougher fast approximation.
     * @return length of vector
     */
    inline T constexpr length_faster() const __attribute__((__always_inline__)) {
      return static_cast<T>(sqrt_faster(lengthSq()));
    }

    /**
     * normalise vector
     */
    inline void constexpr normalise() __attribute__((__always_inline__)) {
      T const temp = length();
      x /= temp;
      y /= temp;
      z /= temp;
    }
    inline void constexpr normalise_fast() __attribute__((__always_inline__)) {
      T const temp = length_fast();
      x /= temp;
      y /= temp;
      z /= temp;
    }
    inline void constexpr normalise_faster() __attribute__((__always_inline__)) {
      T const temp = length_faster();
      x /= temp;
      y /= temp;
      z /= temp;
    }
    inline void constexpr normalize() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise();
    }
    inline void constexpr normalize_fast() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise_fast();
    }
    inline void constexpr normalize_faster() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise_faster();
    }
    inline Vector3<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {
      T const temp(length());
      return Vector3<T>(x / temp, y / temp, z / temp);
    }
    inline Vector3<T> constexpr normalise_copy_fast() const __attribute__((__always_inline__)) {
      T const temp(length_fast());
      return Vector3<T>(x / temp, y / temp, z / temp);
    }
    inline Vector3<T> constexpr normalise_copy_faster() const __attribute__((__always_inline__)) {
      T const temp(length_faster());
      return Vector3<T>(x / temp, y / temp, z / temp);
    }
    inline Vector3<T> constexpr normalize_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      return normalise_copy();
    }
    inline Vector3<T> constexpr normalize_copy_fast() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      return normalise_copy_fast();
    }
    inline Vector3<T> constexpr normalize_copy_faster() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      return normalise_copy_faster();
    }
    /**
     * normalise vector. with added zero safety check
     */
    inline void constexpr normalise_safe() __attribute__((__always_inline__)) {
      T const temp = length();
      if(temp == static_cast<T>(0)) {
        x = static_cast<T>(0);
        y = static_cast<T>(0);
        z = static_cast<T>(0);
      } else {
        x /= temp;
        y /= temp;
        z /= temp;
      }
    }
    inline void constexpr normalize_safe() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise_safe();
    }
    inline Vector3<T> constexpr normalise_safe_copy() const __attribute__((__always_inline__)) {
      T const temp(length());
      if(temp == static_cast<T>(0)) {
        return Vector3<T>();
      } else {
        return Vector3<T>(x / temp, y / temp, z / temp);
      }
    }
    inline Vector3<T> constexpr normalize_safe_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      return normalise_safe_copy();
    }

    /**
     * Absolute vector (make all values positive)
     */
    inline void constexpr abs() __attribute__((__always_inline__)) {
      x = std::abs(x);
      y = std::abs(y);
      z = std::abs(z);
    }
    inline Vector3<T> constexpr abs_copy() const __attribute__((__always_inline__)) {
      return Vector3<T>(std::abs(x), std::abs(y), std::abs(z));
    }

    //-------------[ other operations ]--------------------------
    /**
     * Rotate vector around three axis.
     * @param ax Angle (in degrees) to be rotated around X-axis.
     * @param ay Angle (in degrees) to be rotated around Y-axis.
     * @param az Angle (in degrees) to be rotated around Z-axis.
     */
    inline void constexpr rotate(T ax, T ay, T az) __attribute__((__always_inline__)) {
      rotate_rad(deg2rad(ax), deg2rad(ay), deg2rad(az));
    }
    /**
     * Rotate vector around three axis, radian version.
     * @param ax Angle (in radians) to be rotated around X-axis.
     * @param ay Angle (in radians) to be rotated around Y-axis.
     * @param az Angle (in radians) to be rotated around Z-axis.
     */
    inline void constexpr rotate_rad(T ax, T ay, T az) __attribute__((__always_inline__)) {
      T cos_x = static_cast<T>(0);
      T sin_x = static_cast<T>(0);
      T cos_y = static_cast<T>(0);
      T sin_y = static_cast<T>(0);
      T cos_z = static_cast<T>(0);
      T sin_z = static_cast<T>(0);
      sincos_any(ax, sin_x, cos_x);
      sincos_any(ay, sin_y, cos_y);
      sincos_any(az, sin_z, cos_z);
      T const nx = static_cast<T>(((cos_y *                         cos_z) * x) - ((cos_y *                         sin_z) * y) + (sin_y *         z));
      T const ny = static_cast<T>(((cos_x * sin_z + sin_x * sin_y * cos_z) * x) + ((cos_x * cos_z - sin_x * sin_y * sin_z) * y) - (sin_x * cos_y * z));
      T const nz = static_cast<T>(((sin_x * sin_z - cos_x * sin_y * cos_z) * x) + ((cos_x * sin_y * sin_z + sin_x * cos_z) * y) + (cos_x * cos_y * z));
      x = nx;
      y = ny;
      z = nz;
    }
    /**
     * Rotate vector around the X axis.
     * @param ax Angle (in degrees) to be rotated around X-axis.
     */
    inline void constexpr rotate_x(T ax) __attribute__((__always_inline__)) {
      rotate_rad_x(deg2rad(ax));
    }
    /**
     * Rotate vector around the Y axis.
     * @param ay Angle (in degrees) to be rotated around Y-axis.
     */
    inline void constexpr rotate_y(T ay) __attribute__((__always_inline__)) {
      rotate_rad_y(deg2rad(ay));
    }
    /**
     * Rotate vector around the Z axis.
     * @param az Angle (in degrees) to be rotated around Z-axis.
     */
    inline void constexpr rotate_z(T az) __attribute__((__always_inline__)) {
      rotate_rad_z(deg2rad(az));
    }
    /**
     * Rotate vector around the X axis, radian version.
     * @param ax Angle (in radians) to be rotated around X-axis.
     */
    inline void constexpr rotate_rad_x(T ax) __attribute__((__always_inline__)) {
      T cos_x = static_cast<T>(0);
      T sin_x = static_cast<T>(0);
      sincos_any(ax, sin_x, cos_x);
      T const ny = static_cast<T>((cos_x * y) - (sin_x * z));
      T const nz = static_cast<T>((cos_x * z) + (sin_x * y));
      y = ny;
      z = nz;
    }
    /**
     * Rotate vector around the Y axis, radian version.
     * @param ay Angle (in radians) to be rotated around Y-axis.
     */
    inline void constexpr rotate_rad_y(T ay) __attribute__((__always_inline__)) {
      T cos_y = static_cast<T>(0);
      T sin_y = static_cast<T>(0);
      sincos_any(ay, sin_y, cos_y);
      T const nx = static_cast<T>((cos_y * x) + (sin_y * z));
      T const nz = static_cast<T>((cos_y * z) - (sin_y * x));
      x = nx;
      z = nz;
    }
    /**
     * Rotate vector around the Z axis, radian version.
     * @param az Angle (in radians) to be rotated around Z-axis.
     */
    inline void constexpr rotate_rad_z(T az) __attribute__((__always_inline__)) {
      T cos_z = static_cast<T>(0);
      T sin_z = static_cast<T>(0);
      sincos_any(az, sin_z, cos_z);
      T const nx = static_cast<T>((cos_z * x) - (sin_z * y));
      T const ny = static_cast<T>((cos_z * y) + (sin_z * x));
      x = nx;
      y = ny;
    }
    /**
     * Rotate vector by a quaternion.
     * @param ax Quaternion to rotate by.
     */
    inline void constexpr rotate(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
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
    inline Vector3<T> constexpr lerp(T fact, Vector3<T> const &new_r) const __attribute__((__always_inline__)) {
      return (*this) + (new_r - (*this)) * fact;
    }

    //-------------[ conversion ]--------------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector3<T>)
     * used for passing Vector3<T> values to gl*3[fd] functions.
     */
    inline constexpr operator T*() __attribute__((__always_inline__)) {
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
    inline Vector2<T> constexpr to_2d_xy() const __attribute__((__always_inline__)) {
      return Vector2<T>(x, y);
    }

    /**
     * Gets a 2D vector equivalent using the X and Z axes
     */
    inline Vector2<T> constexpr to_2D_XZ() const __attribute__((__always_inline__)) {
      return Vector2<T>(x, z);
    }
    inline Vector2<T> constexpr to_2d_xz() const __attribute__((__always_inline__)) {
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
 *  <li>access as colour (r,g,b,a) &mdash; <code>v.r = v.g = v.b = v.a = 1;</code></li>
 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = v[3] = 1;</code></li>
 * </ul>
 */
template<typename T>
class Vector4 {
  public:

    union {
      /**
       * First element of vector, alias for R-coordinate.
       * For colour notation.
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
       * For colour notation.
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
       * For colour notation.
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
       * For colour notation. This represnt aplha channel
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
    template<typename FromT>
    inline constexpr Vector4(Vector4<FromT> const &src)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(src.w)) {
    }

    inline constexpr Vector4(Vector3<T> const &src, T new_w) __attribute__((__always_inline__))
      : x(src.x), y(src.y), z(src.z), w(new_w) {
    }

    template<typename FromT>
    inline constexpr Vector4(Vector3<FromT> const &src, FromT new_w)
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(new_w)) {
    }

    //----------------[ assignment ]-------------------------
    /**
     * Sets to (x,y,z)
     * @param nx initial x-coordinate value
     * @param ny initial y-coordinate value
     * @param nz initial z-coordinate value
     * @param nz initial w-coordinate value
     */
    inline void constexpr assign(T nx = 0, T ny = 0, T nz = 0, T nw = 0) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr operator=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
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
    template<typename FromT>
    inline Vector4<T> constexpr operator=(Vector4<FromT> const &rhs) {
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
    inline Vector4<T> constexpr operator=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
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
    template<typename FromT>
    inline Vector4<T> constexpr operator=(Vector3<FromT> const &rhs) {
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
    inline T constexpr &operator[](int n) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr &operator+=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr &operator-=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr &operator*=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr &operator/=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
      z /= rhs.z;
      w /= rhs.w;
      return *this;
    }

    /**
     * Addition operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator+(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w);
    }

    /**
     * Subtraction operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator-(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w);
    }

    /**
     * Multiplication operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w);
    }

    /**
     * Division operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator/(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w);
    }

    /**
     * Addition operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator+=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
      return *this;
    }

    /**
     * Subtraction operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator-=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;
      return *this;
    }

    /**
     * Multiplication operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator*=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
      return *this;
    }

    /**
     * Division operator with 3-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator/=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
      z /= rhs.z;
      return *this;
    }

    /**
     * Addition operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator+(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x + rhs.x, y + rhs.y, z, w);
    }

    /**
     * Subtraction operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator-(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x - rhs.x, y - rhs.y, z, w);
    }

    /**
     * Multiplication operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator*(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x * rhs.x, y * rhs.y, z, w);
    }

    /**
     * Division operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr operator/(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      return Vector4<T>(x / rhs.x, y / rhs.y, z, w);
    }

    /**
     * Addition operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator+=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }

    /**
     * Subtraction operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator-=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x -= rhs.x;
      y -= rhs.y;
      return *this;
    }

    /**
     * Multiplication operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator*=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x *= rhs.x;
      y *= rhs.y;
      return *this;
    }

    /**
     * Division operator with 2-vector
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator/=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      x /= rhs.x;
      y /= rhs.y;
      return *this;
    }

    //-------------[ matrix operations ]-------------------------

    /**
     * Multiplication by 3x3 matrix operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator*=(Matrix3<T> rhs) __attribute__((__always_inline__)) {
      *this = rhs * *this;
      return *this;
    }

    /**
     * Multiplication by 4x4 matrix operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Vector4<T> constexpr &operator*=(Matrix4<T> rhs) __attribute__((__always_inline__)) {
      *this = rhs * *this;
      return *this;
    }

    //--------------[ equality operator ]------------------------

    /**
     * Equality test operator
     * @param rhs Right hand side argument of binary operator.
     * @note Test of equality is based of threshold epsilon value. To be two
     * values equal, must satisfy this condition | lhs.x - rhs.y | < epsilon,
     * same for y-coordinate, z-coordinate, and w-coordinate.
     */
    inline bool constexpr operator==(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
      #ifdef VMATH_SOFT_COMPARE
        return std::abs(x - rhs.x) < static_cast<T>(epsilon) &&
               std::abs(y - rhs.y) < static_cast<T>(epsilon) &&
               std::abs(z - rhs.z) < static_cast<T>(epsilon) &&
               std::abs(w - rhs.w) < static_cast<T>(epsilon);
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
    inline Vector4<T> constexpr &operator+=(T rhs) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr &operator-=(T rhs) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr &operator*=(T rhs) __attribute__((__always_inline__)) {
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
    inline Vector4<T> constexpr &operator/=(T rhs) __attribute__((__always_inline__)) {
      x /= rhs;
      y /= rhs;
      z /= rhs;
      w /= rhs;
      return *this;
    }

    //-------------[ size operations ]---------------------------
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

    /**
     * Get length of vector.
     * @return length of vector
     */
    inline T constexpr length() const __attribute__((__always_inline__)) {
      return static_cast<T>(std::sqrt(lengthSq()));
    }
    /**
     * Get length of vector, fast approximation.
     * @return length of vector
     */
    inline T constexpr length_fast() const __attribute__((__always_inline__)) {
      return static_cast<T>(sqrt_fast(lengthSq()));
    }
    /**
     * Get length of vector, rougher fast approximation.
     * @return length of vector
     */
    inline T constexpr length_faster() const __attribute__((__always_inline__)) {
      return static_cast<T>(sqrt_faster(lengthSq()));
    }

    /**
     * normalise vector
     */
    inline void constexpr normalise() __attribute__((__always_inline__)) {
      T const temp = length();
      x /= temp;
      y /= temp;
      z /= temp;
      w /= temp;
    }
    inline void constexpr normalise_fast() __attribute__((__always_inline__)) {
      T const temp = length_fast();
      x /= temp;
      y /= temp;
      z /= temp;
      w /= temp;
    }
    inline void constexpr normalise_faster() __attribute__((__always_inline__)) {
      T const temp = length_faster();
      x /= temp;
      y /= temp;
      z /= temp;
      w /= temp;
    }
    inline void constexpr normalize() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise();
    }
    inline void constexpr normalize_fast() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise_fast();
    }
    inline void constexpr normalize_faster() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise_faster();
