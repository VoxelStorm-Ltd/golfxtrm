#pragma once

/// Cut-down header alternative for vmath.h including only forward declarations
/// Include this in other headers instead of vmath.h wherever possible

#define _USE_MATH_DEFINES

#ifdef VMATH_NAMESPACE
namespace VMATH_NAMESPACE {
#endif

#define EPSILON epsilon
#define DEG2RAD deg2rad
#define RAD2DEG rad2deg

template<typename T> inline static T constexpr const deg2rad(T const angle_deg);
template<typename T> inline static T constexpr const rad2deg(T const angle_rad);

template<typename T> class Vector2;                                             // forward declarations
template<typename T> class Vector3;
template<typename T> class Vector4;
template<typename T> class Matrix3;
template<typename T> class Matrix4;
template<typename T> class Quaternion;
template<typename T> class Aabb2;
template<typename T> class Aabb3;

template<typename T> class Vector2;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2ld = Vector2<long double>;
using Vector2i = Vector2<int>;

template<typename T> class Vector3;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3ld = Vector3<long double>;
using Vector3i = Vector3<int>;

template<typename T> class Vector4;
using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;
using Vector4ld = Vector4<long double>;
using Vector4i = Vector4<int>;

template<typename T> class Matrix3;
using Matrix3f = Matrix3<float>;
using Matrix3d = Matrix3<double>;
using Matrix3ld = Matrix3<long double>;
using Matrix3i = Matrix3<int>;

template<typename T> class Matrix4;
using Matrix4f = Matrix4<float>;
using Matrix4d = Matrix4<double>;
using Matrix4ld = Matrix4<long double>;
using Matrix4i = Matrix4<int>;

template<typename T> class Quaternion;
using Quatf = Quaternion<float>;
using Quatd = Quaternion<double>;
using Quatld = Quaternion<long double>;

#ifdef VMATH_NAMESPACE
}
#endif

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

template<typename T> constexpr VEC2<T> min(VEC2<T> const &a, const VEC2<T> &b);
template<typename T> constexpr VEC3<T> min(VEC3<T> const &a, const VEC3<T> &b);
template<typename T> constexpr VEC4<T> min(VEC4<T> const &a, const VEC4<T> &b);
template<typename T> constexpr VEC2<T> max(VEC2<T> const &a, const VEC2<T> &b);
template<typename T> constexpr VEC3<T> max(VEC3<T> const &a, const VEC3<T> &b);
template<typename T> constexpr VEC4<T> max(VEC4<T> const &a, const VEC4<T> &b);

}

// cleanup shortcut defines
#undef VEC2
#undef VEC3
#undef VEC4

#ifdef VMATH_NAMESPACE
namespace VMATH_NAMESPACE {
#endif //VMATH_NAMESPACE

template<typename T> class Aabb2;
template<typename T> class Aabb3;

using Aabb2f = Aabb2<float>;
using Aabb2d = Aabb2<double>;
using Aabb2ld = Aabb2<long double>;
using Aabb2i = Aabb2<int>;
using Aabb3f = Aabb3<float>;
using Aabb3d = Aabb3<double>;
using Aabb3ld = Aabb3<long double>;
using Aabb3i = Aabb3<int>;

#ifdef VMATH_NAMESPACE
}
#endif //VMATH_NAMESPACE
