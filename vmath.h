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
    inline T constexpr &at(int x, int y) __attribute__((__always_inline__)) {
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
    inline T constexpr &operator()(int i, int j) __attribute__((__always_inline__)) {
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
    inline void constexpr setTranslation(Vector3<T> const &v) __attribute__((__always_inline__)) {
      data[12] = v.x;
      data[13] = v.y;
      data[14] = v.z;
      data[15] = 1;
    }

    /**
     * Returns translation part of matrix.
     */
    inline Vector3<T> constexpr getTranslation() const __attribute__((__always_inline__)) {
      return Vector3<T>(data[12], data[13], data[14]);
    }

    /**
     * Sets rotation part (matrix 3x3) of matrix.
     *
     * @param m Rotation part of matrix
     */
    inline void constexpr setRotation(Matrix3<T> const &m) __attribute__((__always_inline__)) {
      data[ 0] = m.data[0];
      data[ 1] = m.data[1];
      data[ 2] = m.data[2];
      data[ 4] = m.data[3];
      data[ 5] = m.data[4];
      data[ 6] = m.data[5];
      data[ 8] = m.data[6];
      data[ 9] = m.data[7];
      data[10] = m.data[8];
    }

    /**
     * Returns rotation (matrix 3x3) part of matrix.
     */
    inline Matrix3<T> constexpr getRotation() const __attribute__((__always_inline__)) {
      return Matrix3<T>(data[ 0], data[ 1], data[ 2],
                        data[ 4], data[ 5], data[ 6],
                        data[ 8], data[ 9], data[10]);
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr &operator=(Matrix4<T> const &rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs.data.data(), sizeof(T) * 16);
      return *this;
    }

    /**
     * Copy casting operator
     * @param rhs Right hand side argument of binary operator.
     */
    template<typename FromT>
    inline Matrix4<T> constexpr &operator=(Matrix4<FromT> const &rhs) {
      data[ 0] = static_cast<T>(rhs.data[ 0]);
      data[ 1] = static_cast<T>(rhs.data[ 1]);
      data[ 2] = static_cast<T>(rhs.data[ 2]);
      data[ 3] = static_cast<T>(rhs.data[ 3]);
      data[ 4] = static_cast<T>(rhs.data[ 4]);
      data[ 5] = static_cast<T>(rhs.data[ 5]);
      data[ 6] = static_cast<T>(rhs.data[ 6]);
      data[ 7] = static_cast<T>(rhs.data[ 7]);
      data[ 8] = static_cast<T>(rhs.data[ 8]);
      data[ 9] = static_cast<T>(rhs.data[ 9]);
      data[10] = static_cast<T>(rhs.data[10]);
      data[11] = static_cast<T>(rhs.data[11]);
      data[12] = static_cast<T>(rhs.data[12]);
      data[13] = static_cast<T>(rhs.data[13]);
      data[14] = static_cast<T>(rhs.data[14]);
      data[15] = static_cast<T>(rhs.data[15]);
      return *this;
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix4<T> constexpr &operator=(T const *rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs, sizeof(T) * 16);
      return *this;
      }
    }
    }


    /*
    /**
    inline Matrix4<T> constexpr &operator=(T const *rhs) __attribute__((__always_inline__)) {
     * Extends this bounding-box by a box @a box.
      for(int i = 0; i != 16; ++i) {
        data[i] = (T)rhs[i];
     * @param box A box to extend this bounding-box by.
      }
     */
      return *this;
    template<typename SrcT>
    }
    inline void constexpr extend(Aabb3<SrcT> const &box) {
    */
      if(!valid()) {

        min = box.min;
    //--------------------[ matrix with matrix operations ]---------------------
        max = box.max;
    /**
      } else {
     * Addition operator
        min = std::min(min, box.min);
     * @param rhs Right hand side argument of binary operator.
        max = std::max(max, box.max);
     */
      }
    inline Matrix4<T> constexpr operator+(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
    }
      return Matrix4<T>(data[ 0] + rhs.data[ 0], data[ 1] + rhs.data[ 1], data[ 2] + rhs.data[ 2], data[ 3] + rhs.data[ 3],

                        data[ 4] + rhs.data[ 4], data[ 5] + rhs.data[ 5], data[ 6] + rhs.data[ 6], data[ 7] + rhs.data[ 7],
    /**
                        data[ 8] + rhs.data[ 8], data[ 9] + rhs.data[ 9], data[10] + rhs.data[10], data[11] + rhs.data[11],
     * Gets a copy of this bounding-box extend by a point @a point.
                        data[12] + rhs.data[12], data[13] + rhs.data[13], data[14] + rhs.data[14], data[15] + rhs.data[15]);
     * @param point A point to extend the box by
    }

     * @return Copy of extended bounding-box
    /**
     */
     * Subtraction operator
    template<typename SrcT>
     * @param rhs Right hand side argument of binary operator.
    inline constexpr Aabb3<T> extended(Vector3<SrcT> const &point) const {
     */
      Aabb3<T> ret(*this);
    inline Matrix4<T> constexpr operator-(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
      ret.extend(point);
      return Matrix4<T>(data[ 0] - rhs.data[ 0], data[ 1] - rhs.data[ 1], data[ 2] - rhs.data[ 2], data[ 3] - rhs.data[ 3],
      return ret;
                        data[ 4] - rhs.data[ 4], data[ 5] - rhs.data[ 5], data[ 6] - rhs.data[ 6], data[ 7] - rhs.data[ 7],
    }
                        data[ 8] - rhs.data[ 8], data[ 9] - rhs.data[ 9], data[10] - rhs.data[10], data[11] - rhs.data[11],

                        data[12] - rhs.data[12], data[13] - rhs.data[13], data[14] - rhs.data[14], data[15] - rhs.data[15]);
    /**
    }
     * Gets a copy of this bounding-box extnended by box @a box.

     * @param box A box to extend the copy be.
    //--------------------[ matrix with scalar operations ]---------------------
     * @return Copy of extended bounding-box
    /**
     */
     * Addition operator
    template<typename SrcT>
     * @param rhs Right hand side argument of binary operator.
    inline constexpr Aabb3<T> extended(Aabb3<SrcT> const &box) const {
     */
      Aabb3<T> ret(*this);
    inline Matrix4<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
      ret.extend(box);
      return Matrix4<T>(data[ 0] + rhs, data[ 1] + rhs, data[ 2] + rhs, data[ 3] + rhs,
      return *this;
                        data[ 4] + rhs, data[ 5] + rhs, data[ 6] + rhs, data[ 7] + rhs,
    }
                        data[ 8] + rhs, data[ 9] + rhs, data[10] + rhs, data[11] + rhs,

                        data[12] + rhs, data[13] + rhs, data[14] + rhs, data[15] + rhs);
    /**
    }
     * Tests if the point @a point is within this bounding-box

    /**
     * @param point A point to be tested
     * Subtraction operator
     * @return True if point @a point lies within bounding-box, otherwise false.
     * @param rhs Right hand side argument of binary operator.
     */
     */
    template<typename SrcT>
    inline Matrix4<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
    inline bool constexpr intersects(Vector3<SrcT> const &point) const {
      return Matrix4<T>(data[ 0] - rhs, data[ 1] - rhs, data[ 2] - rhs, data[ 3] - rhs,
      return min.x <= point.x && point.x <= max.x &&
                        data[ 4] - rhs, data[ 5] - rhs, data[ 6] - rhs, data[ 7] - rhs,
             min.y <= point.y && point.y <= max.y &&
                        data[ 8] - rhs, data[ 9] - rhs, data[10] - rhs, data[11] - rhs,
             min.z <= point.z && point.z <= max.z;
                        data[12] - rhs, data[13] - rhs, data[14] - rhs, data[15] - rhs);
    }
    }


    /**
    /**
     * Tests if other bounding-box @a box intersects (even partially) with this bounding-box.
     * Multiplication operator
     * @param box A box to be tested for intersection.
     * @param rhs Right hand side argument of binary operator.
     */
     * @return True if there's intersection between boxes, otherwise false.
    inline Matrix4<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
     */
      return Matrix4<T>(data[ 0] * rhs, data[ 1] * rhs, data[ 2] * rhs, data[ 3] * rhs,
    template<typename SrcT>
                        data[ 4] * rhs, data[ 5] * rhs, data[ 6] * rhs, data[ 7] * rhs,
    inline bool constexpr intersects(Aabb3<SrcT> const &box) const {
                        data[ 8] * rhs, data[ 9] * rhs, data[10] * rhs, data[11] * rhs,
      return max.x >= box.min.x && min.x <= box.max.x &&
                        data[12] * rhs, data[13] * rhs, data[14] * rhs, data[15] * rhs);
             max.y >= box.min.y && min.y <= box.max.y &&
    }
             max.z >= box.min.z && min.z <= box.max.z;

    }
    /**

     * Division operator
    /**
     * @param rhs Right hand side argument of binary operator.
     * Gets result of intersection of this bounding-box with @a other bounding-box.
     */
     * In case the boxes don't intersect, the returned bounding-box is invalid.
    inline Matrix4<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
     * @param other Box to be tested
      return Matrix4<T>(data[ 0] / rhs, data[ 1] / rhs, data[ 2] / rhs, data[ 3] / rhs,
     * @return Result of intersection.
                        data[ 4] / rhs, data[ 5] / rhs, data[ 6] / rhs, data[ 7] / rhs,
                        data[ 8] / rhs, data[ 9] / rhs, data[10] / rhs, data[11] / rhs,
     * @see valid() method for more information on invalid bounding-boxes.
                        data[12] / rhs, data[13] / rhs, data[14] / rhs, data[15] / rhs);
     */
    }
    template<typename SrcT>

    inline constexpr Aabb3<T> intersection(Aabb3<SrcT> const &other) const {
    /**
      return (max.x < other.min.x || min.x > other.max.x ||
     * Addition operator
              max.y < other.min.y || min.y > other.max.y ||
     * @param rhs Right hand side argument of binary operator.
              max.z < other.min.z || min.z > other.max.z) ? Aabb3<T>() : Aabb3<T>(std::max(min, other.min), std::min(max, other.max));
     */
    }
    inline Matrix4<T> constexpr &operator+=(T rhs) __attribute__((__always_inline__)) {

      *this = *this + rhs;
    /**
      return *this;
    }
     * Tests if a ray @b from origin @a intersects with this bounding-box.

     * @param ray The intersecting ray; does not need to be normalised
    /**
     * @param origin Origin of the intersecting ray
     * Subtraction operator
     * @return True if the ray intersects the box, otherwise false.
     * @param rhs Right hand side argument of binary operator.
     */
     */
    template<typename SrcT>
    inline Matrix4<T> constexpr &operator-=(T rhs) __attribute__((__always_inline__)) {
      *this = *this - rhs;
    inline bool constexpr ray_intersects(Vector3<SrcT> const &ray, Vector3<SrcT> const &origin = Vector3<SrcT>()) const {
      return *this;
      // adapted from http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
    }
      /*

      SrcT const dist_min_x = (min.x - origin.x) / ray.x;
    /**
      SrcT const dist_max_x = (max.x - origin.x) / ray.x;
     * Multiplication operator
      SrcT dist_min = std::min(dist_min_x, dist_max_x);
     * @param rhs Right hand side argument of binary operator.
      SrcT dist_max = std::max(dist_min_x, dist_max_x);
     */

    inline Matrix4<T> constexpr &operator*=(T rhs) __attribute__((__always_inline__)) {
      SrcT const dist_min_y = (min.y - origin.y) / ray.y;
      *this = *this * rhs;
      return *this;
      SrcT const dist_max_y = (max.y - origin.y) / ray.y;
    }
      dist_min = std::max(dist_min, std::min(dist_min_y, dist_max_y));

      dist_max = std::min(dist_max, std::max(dist_min_y, dist_max_y));
    /**

     * Division operator
      SrcT const dist_min_z = (min.z - origin.z) / ray.z;
     * @param rhs Right hand side argument of binary operator.
      SrcT const dist_max_z = (max.z - origin.z) / ray.z;
     */
      dist_min = std::max(dist_min, std::min(dist_min_z, dist_max_z));
    inline Matrix4<T> constexpr &operator/=(T rhs) __attribute__((__always_inline__)) {
      dist_max = std::min(dist_max, std::max(dist_min_z, dist_max_z));
      *this = *this / rhs;

      return *this;
      return dist_max >= std::max(dist_min, 0.0f);
    }

      */
    //--------------------[ multiply operators ]--------------------------------
      // constexpr-compatible reformulation:
    /**
      return std::min(std::min(std::max((min.x - origin.x) / ray.x,
     * Multiplication operator
                                        (max.x - origin.x) / ray.x),
     * @param rhs Right hand side argument of binary operator.
                               std::max((min.y - origin.y) / ray.y,
     */
                                        (max.y - origin.y) / ray.y)),
    inline Vector4<T> constexpr operator*(Vector4<T> const &rhs) const __attribute__((__always_inline__)) {
                      std::max((min.z - origin.z) / ray.z,
      return Vector4<T>(data[0] * rhs.x + data[4] * rhs.y + data[ 8] * rhs.z + data[12] * rhs.w,
                               (max.z - origin.z) / ray.z)) >=
                        data[1] * rhs.x + data[5] * rhs.y + data[ 9] * rhs.z + data[13] * rhs.w,
             std::max(std::max(std::max(std::min((min.x - origin.x) / ray.x,
                        data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
                                                 (max.x - origin.x) / ray.x),
                        data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w);
                                        std::min((min.y - origin.y) / ray.y,
    }

                                                 (max.y - origin.y) / ray.y)),
    /**
                               std::min((min.z - origin.z) / ray.z,
     * Multiplication operator
                                        (max.z - origin.z) / ray.z)), 0.0f);
     * @param rhs Right hand side argument of binary operator.
    }
     */

    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
    /**
      return Vector3<T>(data[0] * rhs.x + data[4] * rhs.y + data[8]  * rhs.z,
     * Gets center point of bounding-box.
                        data[1] * rhs.x + data[5] * rhs.y + data[9]  * rhs.z,
     * @return A center point of bounding-box.
                        data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z);
     */
    }
    inline Vector3<T> constexpr center() const __attribute__((__always_inline__)) {

      return (min + max) * static_cast<T>(0.5);
    /**
    }
     * Multiplication operator

     * @param rhs Right hand side argument of binary operator.
    /**
     */
     * Gets extent of bounding-box.
    inline Matrix4<T> constexpr operator*(Matrix4<T> rhs) const __attribute__((__always_inline__)) {
     * @return Extent of bounding-box.
      return Matrix4<T>(rhs.data[ 0] * data[ 0] + rhs.data[ 1] * data[ 4] + rhs.data[ 2] * data[ 8] + rhs.data[ 3] * data[12],
     */
                        rhs.data[ 0] * data[ 1] + rhs.data[ 1] * data[ 5] + rhs.data[ 2] * data[ 9] + rhs.data[ 3] * data[13],
    inline Vector3<T> constexpr extent() const __attribute__((__always_inline__)) {
                        rhs.data[ 0] * data[ 2] + rhs.data[ 1] * data[ 6] + rhs.data[ 2] * data[10] + rhs.data[ 3] * data[14],
      return (max - min) * static_cast<T>(0.5);
                        rhs.data[ 0] * data[ 3] + rhs.data[ 1] * data[ 7] + rhs.data[ 2] * data[11] + rhs.data[ 3] * data[15],
    }


                        rhs.data[ 4] * data[ 0] + rhs.data[ 5] * data[ 4] + rhs.data[ 6] * data[ 8] + rhs.data[ 7] * data[12],
    /**
                        rhs.data[ 4] * data[ 1] + rhs.data[ 5] * data[ 5] + rhs.data[ 6] * data[ 9] + rhs.data[ 7] * data[13],
     * Gets diagonal size of bounding-box
                        rhs.data[ 4] * data[ 2] + rhs.data[ 5] * data[ 6] + rhs.data[ 6] * data[10] + rhs.data[ 7] * data[14],
     * @return Sizes for particular dimensions.
                        rhs.data[ 4] * data[ 3] + rhs.data[ 5] * data[ 7] + rhs.data[ 6] * data[11] + rhs.data[ 7] * data[15],
     */

    inline Vector3<T> constexpr size() const __attribute__((__always_inline__)) {
                        rhs.data[ 8] * data[ 0] + rhs.data[ 9] * data[ 4] + rhs.data[10] * data[ 8] + rhs.data[11] * data[12],
      return max - min;
                        rhs.data[ 8] * data[ 1] + rhs.data[ 9] * data[ 5] + rhs.data[10] * data[ 9] + rhs.data[11] * data[13],
    }
                        rhs.data[ 8] * data[ 2] + rhs.data[ 9] * data[ 6] + rhs.data[10] * data[10] + rhs.data[11] * data[14],

                        rhs.data[ 8] * data[ 3] + rhs.data[ 9] * data[ 7] + rhs.data[10] * data[11] + rhs.data[11] * data[15],
    /**

     * Gets all 8 corner-points of bounding box
                        rhs.data[12] * data[ 0] + rhs.data[13] * data[ 4] + rhs.data[14] * data[ 8] + rhs.data[15] * data[12],
     * @param i An index of bounding-box corner point. Valid values are 0 .. 7.
                        rhs.data[12] * data[ 1] + rhs.data[13] * data[ 5] + rhs.data[14] * data[ 9] + rhs.data[15] * data[13],
     * @return A position of @a i-th corner-point.
                        rhs.data[12] * data[ 2] + rhs.data[13] * data[ 6] + rhs.data[14] * data[10] + rhs.data[15] * data[14],
     * @note The order of points is as follows (where @c + denotes max-point and @c - min-point):
                        rhs.data[12] * data[ 3] + rhs.data[13] * data[ 7] + rhs.data[14] * data[11] + rhs.data[15] * data[15]);
    }
     * 1. (@c + @c + @c +)

     * 2. (@c - @c + @c +)
    /**
     * 3. (@c + @c - @c +)
     * Multiplication operator
     * 4. (@c - @c - @c +)
     * @param rhs Right hand side argument of binary operator.
     * 5. (@c + @c + @c -)
     */
     * 6. (@c - @c + @c -)
    inline Matrix4<T> constexpr &operator*=(Matrix4<T> rhs) __attribute__((__always_inline__)) {
     * 7. (@c + @c - @c -)
      *this = *this * rhs;
     * 8. (@c - @c - @c -)
      return *this;
     *
    }
     */

    inline Vector3<T> constexpr point(unsigned int i) const __attribute__((__always_inline__)) {
    //---------------------------[ misc operations ]----------------------------
      return Vector3<T>(i & 1 ? min.x : max.x, i & 2 ? min.y : max.y, i & 4 ? min.z : max.z);

    }
    /**

     * Computes determinant of matrix
    /**
     * @return Determinant of matrix
     * Gets transformed bounding-box by transform @a t
     * @note This function does 3 * 4 * 6 mul, 3 * 6 add.
     */
     * @param t A transform matrix
    inline T constexpr det() const __attribute__((__always_inline__)) {
     * @return Transformed bounding-box
      return   data[12] * data[9] * data[6]  * data[3]  - data[8] * data[13] * data[6]  * data[3]
     */
             - data[12] * data[5] * data[10] * data[3]  + data[4] * data[13] * data[10] * data[3]
    inline Aabb3<T> constexpr transformed(Matrix4<T> const &t) const __attribute__((__always_inline__)) {

      Aabb3<T> ret;
             + data[8]  * data[5] * data[14] * data[3]  - data[4] * data[9]  * data[14] * data[3]
      for(unsigned int i = 0; i != 8; ++i) {
             - data[12] * data[9] * data[2]  * data[7]  + data[8] * data[13] * data[2]  * data[7]
        const Vector4<T> p(point(i), 1);

        ret.extend((t * p).xyz());
             + data[12] * data[1] * data[10] * data[7]  - data[0] * data[13] * data[10] * data[7]
      }
             - data[8]  * data[1] * data[14] * data[7]  + data[0] * data[9]  * data[14] * data[7]
      return ret;

    }
             + data[12] * data[5] * data[2]  * data[11] - data[4] * data[13] * data[2]  * data[11]

             - data[12] * data[1] * data[6]  * data[11] + data[0] * data[13] * data[6]  * data[11]
    //-------------------------------------------------------------------------------------------------------------

    // operators
             + data[4]  * data[1] * data[14] * data[11] - data[0] * data[5]  * data[14] * data[11]
    //-------------------------------------------------------------------------------------------------------------
             - data[8]  * data[5] * data[2]  * data[15] + data[4] * data[9]  * data[2]  * data[15]
    /**

     * Tests if @a rhs is equal to this bounding-box
             + data[8]  * data[1] * data[6]  * data[15] - data[0] * data[9]  * data[6]  * data[15]
     * @param rhs Right-hand side
             - data[4]  * data[1] * data[10] * data[15] + data[0] * data[5]  * data[10] * data[15];
    }
     * @return True if @a rhs and this bounding-boxes are equal, otherwise false

     */
    /**
    template<typename RhsT>
     * Computes inverse matrix
    inline bool constexpr operator==(Aabb3<RhsT> const &rhs) const {
     * @return Inverse matrix of this matrix.
      return min == rhs.min && max == rhs.max;
     * @note This is a little bit time consuming operation
    }
     * (16 * 6 * 3 mul, 16 * 5 add + det() + mul() functions)

     */
    /**
    inline Matrix4<T> constexpr inverse() const __attribute__((__always_inline__)) {
     * Tests if @a rhs is not equal to this bounding-box
      return Matrix4<T>(data[9]  * data[14] * data[7]  - data[13] * data[10] * data[7]  + data[13] * data[6]  * data[11] -
     * @param rhs Right-hand side
                        data[5]  * data[14] * data[11] - data[9]  * data[6]  * data[15] + data[5]  * data[10] * data[15],
     * @return True if @a rhs and this bounding-boxes are not equal, otherwise false
                        data[13] * data[10] * data[3]  - data[9]  * data[14] * data[3]  - data[13] * data[2]  * data[11] +
     */
                        data[1]  * data[14] * data[11] + data[9]  * data[2]  * data[15] - data[1]  * data[10] * data[15],
    template<typename RhsT>
                        data[5]  * data[14] * data[3]  - data[13] * data[6]  * data[3]  + data[13] * data[2]  * data[7] -
    inline bool constexpr operator!=(Aabb3<RhsT> const &rhs) const {
      return min != rhs.min || max != rhs.max;
    }
                        data[1]  * data[14] * data[7]  - data[5]  * data[2]  * data[15] + data[1]  * data[6]  * data[15],

                        data[9]  * data[6]  * data[3]  - data[5]  * data[10] * data[3]  - data[9]  * data[2]  * data[7] +
    /**
                        data[1]  * data[10] * data[7]  + data[5]  * data[2]  * data[11] - data[1]  * data[6]  * data[11],
     * Gets transformed bounding-box by transform @a rhs.
                        data[12] * data[10] * data[7]  - data[8]  * data[14] * data[7]  - data[12] * data[6]  * data[11] +
     * @param rhs Matrix 4x4 representing the transform
                        data[4]  * data[14] * data[11] + data[8]  * data[6]  * data[15] - data[4]  * data[10] * data[15],
     * @return Transformed bounding-box
                        data[8]  * data[14] * data[3]  - data[12] * data[10] * data[3]  + data[12] * data[2]  * data[11] -
     */
                        data[0]  * data[14] * data[11] - data[8]  * data[2]  * data[15] + data[0]  * data[10] * data[15],
    inline Aabb3<T> constexpr operator*(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
                        data[12] * data[6]  * data[3]  - data[4]  * data[14] * data[3]  - data[12] * data[2]  * data[7] +
      return transformed(rhs);
                        data[0]  * data[14] * data[7]  + data[4]  * data[2]  * data[15] - data[0]  * data[6]  * data[15],
    }
                        data[4]  * data[10] * data[3]  - data[8]  * data[6]  * data[3]  + data[8]  * data[2]  * data[7] -

                        data[0]  * data[10] * data[7]  - data[4]  * data[2]  * data[11] + data[0]  * data[6]  * data[11],
    /**
                        data[8]  * data[13] * data[7]  - data[12] * data[9]  * data[7]  + data[12] * data[5]  * data[11] -
     * Apply transform @a rhs to this bounding-box
                        data[4]  * data[13] * data[11] - data[8]  * data[5]  * data[15] + data[4]  * data[9]  * data[15],
     * @param rhs A transform to be applied
                        data[12] * data[9]  * data[3]  - data[8]  * data[13] * data[3]  - data[12] * data[1]  * data[11] +
     * @return Reference to this
                        data[0]  * data[13] * data[11] + data[8]  * data[1]  * data[15] - data[0]  * data[9]  * data[15],
     */
                        data[4]  * data[13] * data[3]  - data[12] * data[5]  * data[3]  + data[12] * data[1]  * data[7] -
    inline Aabb3<T> constexpr &operator*=(Matrix4<T> const &rhs) __attribute__((__always_inline__)) {
                        data[0]  * data[13] * data[7]  - data[4]  * data[1]  * data[15] + data[0]  * data[5]  * data[15],
      *this = transformed(rhs);
                        data[8]  * data[5]  * data[3]  - data[4]  * data[9]  * data[3]  - data[8]  * data[1]  * data[7] +
      return *this;
                        data[0]  * data[9]  * data[7]  + data[4]  * data[1]  * data[11] - data[0]  * data[5]  * data[11],
    }
                        data[12] * data[9]  * data[6]  - data[8]  * data[13] * data[6]  - data[12] * data[5]  * data[10] +

                        data[4]  * data[13] * data[10] + data[8]  * data[5]  * data[14] - data[4]  * data[9]  * data[14],
    /**
                        data[8]  * data[13] * data[2]  - data[12] * data[9]  * data[2]  + data[12] * data[1]  * data[10] -
     * Extends this bounding-box by point @a rhs.
                        data[0]  * data[13] * data[10] - data[8]  * data[1]  * data[14] + data[0]  * data[9]  * data[14],
     * @param rhs A point to extend this bounding-box by
                        data[12] * data[5]  * data[2]  - data[4]  * data[13] * data[2]  - data[12] * data[1]  * data[6] +
     * @return Reference to this
                        data[0]  * data[13] * data[6]  + data[4]  * data[1]  * data[14] - data[0]  * data[5]  * data[14],
     */
                        data[4]  * data[9]  * data[2]  - data[8]  * data[5]  * data[2]  + data[8]  * data[1]  * data[6] -
    template<typename SrcT>
                        data[0]  * data[9]  * data[6]  - data[4]  * data[1]  * data[10] + data[0]  * data[5]  * data[10]) / det();
    inline Aabb3<T> constexpr &operator<<(Vector3<SrcT> const &rhs) {
    }
      extend(rhs);

      return *this;
    /**
    }
     * Transpose matrix.

     */
    /**
    inline Matrix4<T> constexpr transpose() const __attribute__((__always_inline__)) {
     * Extends this bounding-box by box @a rhs.
      return Matrix4<T>(data[0], data[4], data[8],  data[12],
     * @param rhs A box to extend this bounding-box by
                        data[1], data[5], data[9],  data[13],
     * @return Reference to this
                        data[2], data[6], data[10], data[14],
     */
                        data[3], data[7], data[11], data[15]);
    template<typename SrcT>
    }

    inline Aabb3<T> constexpr &operator<<(Aabb3<SrcT> const &rhs) {
    /**
      extend(rhs);
     * Linear interpolation of two matrices
      return *this;
     * @param fact Factor of interpolation. For translation from positon
    }
     * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.

     * @param rhs Second Matrix for interpolation
    /**
     * @note However values of fact parameter are reasonable only in interval
     * Union of this and @a rhs bounding-boxes
     * [0.0 , 1.0], you can pass also values outside of this interval and you
     * @param rhs Right-hand side of union
     * can get result (extrapolation?)
     * @return A resulting bounding-box representing union
     */
     */
    inline Matrix4<T> constexpr lerp(T fact, Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
    template<typename RhsT>
      return (*this) + (rhs - (*this)) * fact;
    inline Aabb3<T> constexpr operator|(Aabb3<RhsT> const &rhs) const {
    }
      return extended(rhs);

    }
    //-------------[ conversion ]-----------------------------

    /**
    /**
     * Conversion to pointer operator
     * Intersection of this and @a rhs bounding-boxed
     * @return Pointer to internally stored (in management of class Matrix4<T>)
     * @param rhs Right-hand side
     * used for passing Matrix4<T> values to gl*[fd]v functions.
     */
     * @return Resulting bounding-box representing the intersection.
    inline constexpr operator T*() __attribute__((__always_inline__)) {
     */
      return reinterpret_cast<T*>(data.data());
    template<typename RhsT>
    }
    inline Aabb3<T> constexpr operator&(Aabb3<RhsT> const &rhs) const {

      return intersection(rhs);
    /**
    }
     * Conversion to pointer operator

     * @return Constant Pointer to internally stored (in management of class Matrix4<T>)
    /**
     * used for passing Matrix4<T> values to gl*[fd]v functions.
     * Outputs string representation of bounding-box @a rhs to output stream @a lhs
     */
     * @param lhs Output stream to write to
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
     * @param rhs Bounding-box to write to output stream.
      return reinterpret_cast<T const*>(data.data());
     * @return Reference to output stream @a lhs
    }
     */

    //----------[ output operator ]----------------------------
    inline friend std::ostream &operator<<(std::ostream &lhs, const Aabb3<T> &rhs) __attribute__((__always_inline__)) {
    /**
      lhs << rhs.min << " x " << rhs.max;
     * Output to stream operator
      return lhs;
     * @param lhs Left hand side argument of operator (commonly ostream instance).
    }
     * @param rhs Right hand side argument of operator.
};
     * @return Left hand side argument - the ostream object passed to operator.

     */
/// 2D axis-aligned bounding box of floats
    inline friend std::ostream &operator <<(std::ostream &lhs, Matrix4<T> const &rhs) __attribute__((__always_inline__)) {
using Aabb2f = Aabb2<float>;
      for(int i = 0; i != 4; ++i) {
/// 2D axis-aligned bounding box of doubles
        lhs << "|\t";
using Aabb2d = Aabb2<double>;
        for(int j = 0; j != 4; ++j) {
/// 2D axis-aligned bounding box of long doubles
          lhs << rhs.at(j, i) << "\t";
using Aabb2ld = Aabb2<long double>;
        }
/// 2D axis-aligned bounding box of integers
        lhs << "|" << std::endl;
using Aabb2i = Aabb2<int>;
      }

      return lhs;
    }
/// 3D axis-aligned bounding box of floats

using Aabb3f = Aabb3<float>;
    /**
/// 3D axis-aligned bounding box of doubles
     * Gets string representation.
using Aabb3d = Aabb3<double>;
     */
/// 3D axis-aligned bounding box of long doubles
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
using Aabb3ld = Aabb3<long double>;
      std::ostringstream oss;
/// 2D axis-aligned bounding box of integers
      oss << *this;
using Aabb3i = Aabb3<int>;
      return oss.str();

    }
};
#ifdef VMATH_NAMESPACE

}
/// Matrix 4x4 of floats
#endif //VMATH_NAMESPACE
using Matrix4f = Matrix4<float>;

/// Matrix 4x4 of doubles
#endif // __vmath_Header_File__
using Matrix4d = Matrix4<double>;

/// Matrix 4x4 of long doubles
/* -*- C++ -*- */
using Matrix4ld = Matrix4<long double>;
/** @page license License
/// Matrix 4x4 of int
 * vmath, set of classes for computer graphics mathematics.
using Matrix4i = Matrix4<int>;
 * Copyright (c) 2005-2011, Jan Bartipan < barzto at gmail dot com >

 * All rights reserved.
/**
 *
 * Quaternion class implementing some quaternion algebra operations.
 * Redistribution and use in source and binary forms, with or without
 * Quaternion is kind of complex number it consists of its real part (w)
 * modification, are permitted provided that the following conditions
 * and its complex part v. This complex part has three elements, so we
 * are met:
 * can express it as xi + yj + zk . Note that coordinates of (x,y,z) are
 *
 * hold inside v field.
 */
 * - Redistributions of source code must retain the above copyright
template<typename T>
 *   notice, this list of conditions and the following disclaimer.
class Quaternion {
 * - Redistributions in binary form must reproduce the above copyright
  public:
 *   notice, this list of conditions and the following disclaimer in

 *   the documentation and/or other materials provided with the
    union {
 *   distribution.
      /**
 * - Neither the names of its contributors may be used to endorse or
       * Real part of quaternion.
       */
 *   promote products derived from this software without specific
      T w;
 *   prior written permission.
      T real;
 *
      T scalar;
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    };
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT

 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    union {
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
      /**
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
       * Imaginary part of quaternion.
       */
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
      Vector3<T> v;
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
      Vector3<T> imaginary;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
      Vector3<T> vector;
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    };
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY

 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    /**
 * POSSIBILITY OF SUCH DAMAGE.
     * Quaternion constructor, sets quaternion to (0 + 0i + 0j + 0k).
 */
     */

    inline constexpr Quaternion() __attribute__((__always_inline__))
/**
      : w(0), v(0, 0, 0) {
 * @mainpage Intro
    }

 * Vector mathematics for computer graphics - VoxelStorm version
    /**
 *
     * Copy constructor.
 * @section Features
     */
 * <ul>
    inline constexpr Quaternion(Quaternion<T> const &q) __attribute__((__always_inline__))
 *    <li> basic arithmetic operations - using operators </li>
      : w(q.w), v(q.v) {
 *    <li> basic linear algebra operations - such as transpose, dot product, etc. </li>
    }
 *    <li> aliases for vertex coordinates - it means:

 *    <pre>
    /**
 *  Vector3f v;
     * Copy casting constructor.
 *  // use vertex coordinates
     */
 *  v.x = 1; v.y = 2; v.z = -1;
    template<typename FromT>
 *
    inline constexpr Quaternion(Quaternion<FromT> const &q)
 *                                                                              // use texture coordinates
      : w(static_cast<T>(q.w)), v(q.v) {
    }
 *  v.s = 0; v.t = 1; v.u = 0.5;

 *                                                                              // use colour coordinates
    /**
 *  v.r = 1; v.g = 0.5; v.b = 0;
     * Creates quaternion object from real part w_ and complex part v_.
 *    </pre>
     * @param w_ Real part of quaternion.
 *    </li>
     * @param v_ Complex part of quaternion (xi + yj + zk).
 *    <li> conversion constructor and assign operators - so you can assign a value of Vector3&lt;T1&gt; type
     */
 *    to a variable of Vector3&lt;T2&gt; type for any convertible T1, T2 type pairs. In other words, you can do this:
    inline constexpr Quaternion(T w_, Vector3<T> const &v_) __attribute__((__always_inline__))
 *    <pre>
      : w(w_), v(v_) {
 *
    }

 *  Vector3f f3; Vector3d d3 = f3;
    /**
 *  ...
     * Creates quaternion object from value (w_ + xi + yj + zk).
 *  f3 = d3;
     * @param w_ Real part of quaternion.
 *    </pre>
     * @param x Complex coefficient for i complex constant.
 *    </li>
     * @param y Complex coefficient for j complex constant.
 * </ul>
     * @param z Complex coefficient for k complex constant.
 *
     */
 * @section Predefined types
    inline constexpr Quaternion(T w_, T x, T y, T z) __attribute__((__always_inline__))
 * <ul>
      : w(w_), v(x, y, z) {
 *  <li>(Vector2) Two dimensional vector
    }
 *    <ul>

 *      <li>float &mdash; Vector2f</li>
    /**
 *      <li>double &mdash; Vector2d</li>
     * Construct quaternion from rotation matrix.
 *      <li>int &mdash; Vector2i</li>
     * @return Rotation matrix expressing this quaternion.
 *    </ul>
     */
 *  </li>
    inline constexpr Quaternion(Matrix3<T> const &matrix) __attribute__((__always_inline__)) {
 *  <li>(Vector3) Three dimensional vector
      // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
 *    <ul>
      // article "Quaternion Calculus and Fast Animation".
 *      <li>float &mdash; Vector3f</li>
      T const trace = matrix.at(0, 0) + matrix.at(1, 1) + matrix.at(2, 2);
 *      <li>double &mdash; Vector3d</li>
      if(trace > 0) {
 *      <li>int &mdash; Vector3i</li>
        // |w| > 1/2, may as well choose w > 1/2
 *    </ul>
        T root = std::sqrt(trace + static_cast<T>(1.0));                        // 2w
 *  </li>
        w = static_cast<T>(0.5) * root;
 *  <li>(Vector4) Four dimensional vector
        root = static_cast<T>(0.5) / root;                                      // 1/(4w)
 *    <ul>
        v.x = (matrix.at(2, 1) - matrix.at(1, 2)) * root;
 *      <li>float &mdash; Vector4f</li>
        v.y = (matrix.at(0, 2) - matrix.at(2, 0)) * root;
 *      <li>double &mdash; Vector4d</li>
        v.z = (matrix.at(1, 0) - matrix.at(0, 1)) * root;
 *      <li>int &mdash; Vector4i</li>
      } else {
 *    </ul>
        // |w| <= 1/2
 *  </li>
        unsigned int constexpr next[3] = {1, 2, 0};

 *  <li>(Matrix3) Matrix 3x3
        unsigned int i = 0;
 *    <ul>
        if(matrix.at(1, 1) > matrix.at(0, 0)) {
 *      <li>float &mdash; Matrix3f</li>
          i = 1;
 *      <li>double &mdash; Matrix3d</li>
        }
 *      <li>int &mdash; Matrix3i</li>
        if(matrix.at(2, 2) > matrix.at(i, i)) {
 *    </ul>
          i = 2;
 *  </li>
        }
 *  <li>(Matrix4) Matrix 4x4
        unsigned int j = next[i];
 *    <ul>
        unsigned int k = next[j];
 *      <li>float &mdash; Matrix4f</li>

 *      <li>double &mdash; Matrix4d</li>
        T root = std::sqrt(matrix.at(i, i) - matrix.at(j, j) - matrix.at(k, k) + static_cast<T>(1.0));
 *      <li>int &mdash; Matrix4i</li>
        T *quaternion[3] = {&v.x, &v.y, &v.z};
 *    </ul>
        *quaternion[i] = static_cast<T>(0.5) * root;
 *  </li>
        root = static_cast<T>(0.5) / root;
  *   <li> Quaternion
        w = (matrix.at(k, j) - matrix.at(j, k)) * root;
 *    <ul>
        *quaternion[j] = (matrix.at(j, i) + matrix.at(i, j)) * root;
 *      <li>float &mdash; Quatf</li>
        *quaternion[k] = (matrix.at(k, i) + matrix.at(i, k)) * root;
 *      <li>double &mdash; Quatd</li>
      }
 *    </ul>
    }
 *    </li>
    inline constexpr Quaternion(Matrix4<T> const &matrix) __attribute__((__always_inline__)) {
 *  </li>
      // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
 * </ul>
      // article "Quaternion Calculus and Fast Animation".
 */
      T const trace = matrix.at(0, 0) + matrix.at(1, 1) + matrix.at(2, 2);

      if(trace > 0) {
        // |w| > 1/2, may as well choose w > 1/2
// Modified 2011-06-12, Davide Bacchet (davide.bacchet at gmail dot com)
        T root = std::sqrt(trace + static_cast<T>(1.0));                        // 2w
//                      added () operators with standard matrix notation (opposite wrt at() ).
        w = static_cast<T>(0.5) * root;

        root = static_cast<T>(0.5) / root;                                      // 1/(4w)
// Modified 2013-2016, Eugene Hopkinson for VoxelStorm Ltd
        v.x = (matrix.at(2, 1) - matrix.at(1, 2)) * root;
//                     various expansions and conversions
        v.y = (matrix.at(0, 2) - matrix.at(2, 0)) * root;
//                     Version 2.0: C++11 specific optimisations, including constexpr
        v.z = (matrix.at(1, 0) - matrix.at(0, 1)) * root;
//                     Version 2.1: C++14 optimisations, intersection algorithms
      } else {
//                     Version 2.2: Many additional functions
        // |w| <= 1/2

        unsigned int constexpr next[3] = {1, 2, 0};

#ifndef __vmath_Header_File__
        unsigned int i = 0;
#define __vmath_Header_File__
        if(matrix.at(1, 1) > matrix.at(0, 0)) {

          i = 1;
#define _USE_MATH_DEFINES
        }
#include <cmath>
        if(matrix.at(2, 2) > matrix.at(i, i)) {
#include <cstring>
          i = 2;
#include <iostream>
        }
#include <sstream>
        unsigned int j = next[i];
#include <string>
        unsigned int k = next[j];
#include <array>

#include <mmintrin.h>
        T root = std::sqrt(matrix.at(i, i) - matrix.at(j, j) - matrix.at(k, k) + static_cast<T>(1.0));

        T *quaternion[3] = {&v.x, &v.y, &v.z};
#ifdef VMATH_NAMESPACE
        *quaternion[i] = static_cast<T>(0.5) * root;
namespace VMATH_NAMESPACE {
        root = static_cast<T>(0.5) / root;
#endif
        w = (matrix.at(k, j) - matrix.at(j, k)) * root;

        *quaternion[j] = (matrix.at(j, i) + matrix.at(i, j)) * root;
// use boost's constants if available
        *quaternion[k] = (matrix.at(k, i) + matrix.at(i, k)) * root;
#ifndef VMATH_NO_BOOST
      }
    }
#include <boost/math/constants/constants.hpp>

#else  // VMATH_NO_BOOST
    //----------------[ assignment ]-------------------------
#ifndef M_PI
    /**
#define M_PI           3.14159265358979323846  /* pi */
     * Sets to (w_ + xi + yj + zk).
#endif // M_PI
     * @param w_ Real part of quaternion.
#endif // VMATH_NO_BOOST
     * @param x Complex coefficient for i complex constant.

     * @param y Complex coefficient for j complex constant.
// note: use VMATH_SOFT_COMPARE to enable fuzzy matching for different types,
     * @param z Complex coefficient for k complex constant.
// with epsilon used to match within a range of rounding error - but doing this
     */
// can be very expensive in some cases.
    inline void constexpr assign(T w_ = 0, T x = 0, T y = 0, T z = 0) __attribute__((__always_inline__)) {
double constexpr epsilon = 4.37114e-05;
      w = w_;
#define EPSILON epsilon
      v.assign(x, y, z);
#define DEG2RAD deg2rad
    }
#define RAD2DEG rad2deg


    /**
template<typename T>
     * Copy operator
inline static T constexpr const deg2rad(T const angle_deg) __attribute__((__always_inline__));
     * @param rhs Right hand side argument of binary operator.
template<typename T>
     */
inline static T constexpr const deg2rad(T const angle_deg) {
    inline Quaternion<T> constexpr &operator=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
  #ifndef VMATH_NO_BOOST
      v = rhs.v;
      w = rhs.w;
    //return (angle_deg * boost::math::constants::pi<T>()) / 180.0;
      return *this;
    return angle_deg * boost::math::constants::degree<T>();
    }
  #else  // VMATH_NO_BOOST

    return (angle_deg * M_PI) / 180.0;
    /**
  #endif // VMATH_NO_BOOST
     * Copy convert operator
}
     * @param rhs Right hand side argument of binary operator.

     */
template<typename T>
    template<typename FromT>
inline static T constexpr const rad2deg(T const angle_rad) __attribute__((__always_inline__));
    inline Quaternion<T> constexpr &operator=(Quaternion<FromT> const &rhs) {
template<typename T>
      v = rhs.v;
inline static T constexpr const rad2deg(T const angle_rad) {
      w = static_cast<T>(rhs.w);
      return *this;
  #ifndef VMATH_NO_BOOST
    }
    return angle_rad * boost::math::constants::radian<T>();

  #else  // VMATH_NO_BOOST
    /**
    return (angle_rad * 180.0) / M_PI;
     * Addition operator
  #endif // VMATH_NO_BOOST
     * @param rhs Right hand side argument of binary operator.
}
     */

    inline Quaternion<T> constexpr operator+(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
template<typename T>
      return Quaternion<T>(w + rhs.w, v + rhs.v);
inline static void constexpr sincos_any(T const angle_rad, T &out_sin, T &out_cos) __attribute__((__always_inline__));
    }
template<typename T>

    /**
inline static void constexpr sincos_any(T const angle_rad, T &out_sin, T &out_cos) {
     * Multiplication operator
  __builtin_sincos(angle_rad, &out_sin, &out_cos);
     * @param rhs Right hand side argument of binary operator.
}
     */
inline static void constexpr sincos_any(int const angle_rad, int &out_sin, int &out_cos) __attribute__((__always_inline__));
    inline Quaternion<T> constexpr operator*(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
inline static void constexpr sincos_any(int const angle_rad, int &out_sin, int &out_cos) {
      return Quaternion<T>(w * rhs.w   - v.x * rhs.v.x - v.y * rhs.v.y - v.z * rhs.v.z,
  // if we're only working with integers, assume single precision is enough
                           w * rhs.v.x + v.x * rhs.w   + v.y * rhs.v.z - v.z * rhs.v.y,
  float out_sin_temp = 0.0f;
                           w * rhs.v.y - v.x * rhs.v.z + v.y * rhs.w   + v.z * rhs.v.x,
  float out_cos_temp = 0.0f;
                           w * rhs.v.z + v.x * rhs.v.y - v.y * rhs.v.x + v.z * rhs.w);
  __builtin_sincosf(static_cast<float>(angle_rad), &out_sin_temp, &out_cos_temp);
    }
  out_sin = static_cast<int>(out_sin_temp);

  out_cos = static_cast<int>(out_cos_temp);
    /**
}
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
inline static void constexpr sincos_any(float const angle_rad, float &out_sin, float &out_cos) __attribute__((__always_inline__));
     */
inline static void constexpr sincos_any(float const angle_rad, float &out_sin, float &out_cos) {
    inline Quaternion<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
  __builtin_sincosf(angle_rad, &out_sin, &out_cos);
      return Quaternion<T>(w * rhs, v * rhs);
}
    }
inline static void constexpr sincos_any(long double const angle_rad, long double &out_sin, long double &out_cos) __attribute__((__always_inline__));

inline static void constexpr sincos_any(long double const angle_rad, long double &out_sin, long double &out_cos) {
    /**
  __builtin_sincosl(angle_rad, &out_sin, &out_cos);
     * Division operator
}
     * @param rhs Right hand side argument of binary operator.

     */
inline static float constexpr sqrt_inv_fast(float number) __attribute__((__always_inline__));
    inline Quaternion<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Quaternion<T>(w / rhs, v / rhs);
inline static float constexpr sqrt_inv_fast(float number) {
    }
  /// Adapted from Quake III's fast inverse square root approximation

  float constexpr const threehalfs = 1.5f;
    /**

     * Subtraction operator
  float x = number * 0.5f;
     * @param rhs Right hand side argument of binary operator.
  float y = number;
     */
  uint32_t i  = *(uint32_t*)&y;                                                 // evil floating point bit level hacking
    inline Quaternion<T> constexpr operator-(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
  //i = 0x5f3759df - (i >> 1);                                                    // what the fuck?
      return Quaternion<T>(w - rhs.w, v - rhs.v);
  i = 0x5f375a84 - (i >> 1);                                                    // improved magic number from http://jheriko-rtw.blogspot.co.uk/2009/04/understanding-and-improving-fast.html
    }

  y = *(float*)&i;
    /**
  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
     * Addition operator
  y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
     * @param rhs Right hand side argument of binary operator.
  return y;
     */
}
    inline Quaternion<T> constexpr &operator+=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
inline static double constexpr sqrt_inv_fast(double number) __attribute__((__always_inline__));
      w += rhs.w;
inline static double constexpr sqrt_inv_fast(double number) {
      v += rhs.v;
  /// Similar to the Quake III fast inverse square root but for doubles
      return *this;
  double constexpr const threehalfs = 1.5;
    }


    /**
  double x = number * 0.5;
     * Subtraction operator
  double y = number;
     * @param rhs Right hand side argument of binary operator.
  uint64_t i  = *(uint64_t*)&y;                                                 // evil floating point bit level hacking
     */
  //i = 0x5fe6eb50c7b537a9ll - (i >> 1);                                          // even more magic than "what the fuck" number
    inline Quaternion<T> constexpr &operator-=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
  uint64_t constexpr const magic = (uint64_t(0x5fe6eb50) << (8 * 4)) + uint64_t(0xc7b537a9); // hack to produce 0x5fe6eb50c7b537a9ll without triggering -Wlong-long warning
      w -= rhs.w;
  i = magic - (i >> 1);
      v -= rhs.v;
  y = *(double*)&i;
      return *this;
  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
    }

  y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
    /**
  return y;
     * Multiplication operator
}
     * @param rhs Right hand side argument of binary operator.
template<typename T>
     */
inline static T constexpr sqrt_fast(T number) __attribute__((__always_inline__));
    inline Quaternion<T> constexpr &operator*=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
template<typename T>
      Quaternion q = (*this) * rhs;
inline static T constexpr sqrt_fast(T number) {
      v = q.v;
  return sqrt_inv_fast(number) * number;
      w = q.w;
}
      return *this;
inline static long double constexpr sqrt_fast(long double number) __attribute__((__always_inline__));
    }
inline static long double constexpr sqrt_fast(long double number) {

    /**
  // we don't have a way to handle long doubles with the fast approximation, so just cast to double
     * Multiplication operator
  return static_cast<long double>(sqrt_inv_fast(static_cast<double>(number))) * number;
     * @param rhs Right hand side argument of binary operator.
}
     */
inline static int constexpr sqrt_fast(int number) __attribute__((__always_inline__));
    inline Quaternion<T> constexpr &operator*=(T rhs) __attribute__((__always_inline__)) {
inline static int constexpr sqrt_fast(int number) {
      w *= rhs;
  // convert ints to floats and back
      v *= rhs;
  return static_cast<int>(sqrt_inv_fast(static_cast<float>(number)) * static_cast<float>(number));
      return *this;
}
    }


inline static float constexpr sqrt_inv_faster(float number) __attribute__((__always_inline__));
    /**
inline static float constexpr sqrt_inv_faster(float number) {
     * Division operator
     * @param rhs Right hand side argument of binary operator.
  /// Adapted from Quake III's fast inverse square root approximation - one iteration version
     */
  float constexpr const threehalfs = 1.5f;
    inline Quaternion<T> constexpr &operator/=(T rhs) __attribute__((__always_inline__)) {

      w /= rhs;
  float x = number * 0.5f;
      v /= rhs;
  float y = number;
      return *this;
  uint32_t i  = *(uint32_t*)&y;                                                 // evil floating point bit level hacking
    }
  //i = 0x5f3759df - (i >> 1);                                                    // what the fuck?

    /**
  i = 0x5f375a84 - (i >> 1);                                                    // improved magic number from http://jheriko-rtw.blogspot.co.uk/2009/04/understanding-and-improving-fast.html
     * Equality test operator
  y = *(float*)&i;
     * @param rhs Right hand side argument of binary operator.
  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
     * @note Test of equality is based of threshold epsilon value. To be two
  //y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
     * values equal, must satisfy this condition | lhs - rhs | < epsilon,
  return y;
     * for all quaternion coordinates.
}
     */
inline static double constexpr sqrt_inv_faster(double number) __attribute__((__always_inline__));
    inline bool constexpr operator==(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
inline static double constexpr sqrt_inv_faster(double number) {
      #ifdef VMATH_SOFT_COMPARE
  /// Similar to the Quake III fast inverse square root but for doubles
        return (std::abs(w - rhs.w) < static_cast<T>(epsilon)) && v == rhs.v;
  double constexpr const threehalfs = 1.5;
      #else

        return w == rhs.w && v == rhs.v;
  double x = number * 0.5;
      #endif // VMATH_SOFT_COMPARE
  double y = number;
    }
  uint64_t i  = *(uint64_t*)&y;                                                 // evil floating point bit level hacking

    /**
  //i = 0x5fe6eb50c7b537a9ll - (i >> 1);                                          // even more magic than "what the fuck" number
     * Inequality test operator
  uint64_t constexpr const magic = (uint64_t(0x5fe6eb50) << (8 * 4)) + uint64_t(0xc7b537a9); // hack to produce 0x5fe6eb50c7b537a9ll without triggering -Wlong-long warning
     * @param rhs Right hand side argument of binary operator.
  i = magic - (i >> 1);
     * @return not (lhs == rhs) :-P
  y = *(double*)&i;
     */
  y = y * (threehalfs - (x * y * y));                                           // 1st iteration
    inline bool constexpr operator!=(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
  //y = y * (threehalfs - (x * y * y));                                           // 2nd iteration, this can be removed
      return !(*this == rhs);
  return y;
    }
}

template<typename T>
    //-------------[ unary operations ]--------------------------
inline static T constexpr sqrt_faster(T number) __attribute__((__always_inline__));
    /**
template<typename T>
     * Unary negate operator
inline static T constexpr sqrt_faster(T number) {
     * @return negated quaternion
     */
  return sqrt_inv_faster(number) * number;
    inline Quaternion<T> constexpr operator-() const __attribute__((__always_inline__)) {
}
      return Quaternion<T>(-w, -v);
inline static long double constexpr sqrt_faster(long double number) __attribute__((__always_inline__));
    }
inline static long double constexpr sqrt_faster(long double number) {

  // we don't have a way to handle long doubles with the fast approximation, so just cast to double
    /**
  return static_cast<long double>(sqrt_inv_faster(static_cast<double>(number))) * number;
     * Unary conjugate operator
}
     * @return conjugated quaternion
inline static int constexpr sqrt_faster(int number) __attribute__((__always_inline__));
     */
inline static int constexpr sqrt_faster(int number) {
    inline Quaternion<T> constexpr operator~() const __attribute__((__always_inline__)) {
  // convert ints to floats and back
      return Quaternion<T>(w, -v);
    }
  return static_cast<int>(sqrt_inv_faster(static_cast<float>(number)) * static_cast<float>(number));

}
    /**

     * Get length of quaternion.
inline static float sqrt_inv_sse(float number) __attribute__((__always_inline__));
     * @return Length of quaternion.
inline static float sqrt_inv_sse(float number) {
     */
  /// Scalar SSE inverse square root approximation
    inline T constexpr length() const __attribute__((__always_inline__)) {
  float result;
      return static_cast<T>(std::sqrt(lengthSq()));
  _mm_store_ss(&result, _mm_rsqrt_ss(_mm_load_ss(&number)));
    }
  return result;
    /**
}
     * Get length of quaternion, fast approximation.
inline static float sqrt_sse(float number) __attribute__((__always_inline__));
     * @return Length of quaternion.
     */
inline static float sqrt_sse(float number) {
    inline T constexpr length_fast() const __attribute__((__always_inline__)) {
  /// Scalar SSE square root approximation
      return static_cast<T>(sqrt_fast(lengthSq()));
  return sqrt_inv_sse(number) * number;
    }
}
    /**

     * Get length of quaternion, rougher fast approximation.
template<typename T> class Vector2;                                             // forward declarations
     * @return Length of quaternion.
template<typename T> class Vector3;
     */
template<typename T> class Vector4;
    inline T constexpr length_faster() const __attribute__((__always_inline__)) {
template<typename T> class Matrix3;
      return static_cast<T>(sqrt_faster(lengthSq()));
template<typename T> class Matrix4;
    }
template<typename T> class Quaternion;

template<typename T> class Aabb2;
    /**
template<typename T> class Aabb3;
     * Return square of length.

     * @return length ^ 2
/**
     * @note This method is faster then length(). For comparison
 * Class for two dimensional vector.
     * of length of two quaternion can be used just this value, instead
 * There are three ways of accessing vector components.
     * of more expensive length() method.
 * Let's have <code>Vector2f v</code>, you can either:
     */
 * <ul>
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
 *  <li>access as position(x,y) &mdash; <code>v.x = v.y = 3;</code></li>
      return w * w + v.lengthSq();
 *  <li>access as texture coordinate (s,t) &mdash; <code>v.s = v.t = 3;</code></li>
    }

 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = 3;</code></li>
    /**
 * </ul>
     * normalise quaternion
 */
     */
template<typename T>
    inline void constexpr normalise() __attribute__((__always_inline__)) {
class Vector2 {
      T len = length();
  public:
      w /= len;
    union {
      v /= len;
      /**
    }
       * First element of vector, alias for X-coordinate.
    inline void constexpr normalise_fast() __attribute__((__always_inline__)) {
       */
      T len = length_fast();
      T x;
      w /= len;

      v /= len;
      /**
    }
       * First element of vector, alias for S-coordinate.
    inline void constexpr normalise_faster() __attribute__((__always_inline__)) {
       * For textures notation.
      T len = length_faster();
       */
      w /= len;
      T s;
      v /= len;
    };
    }

    inline void constexpr normalize() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
    union {
      normalise();
      /**
    }
       * Second element of vector, alias for Y-coordinate.
    inline void constexpr normalize_fast() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
       */
      normalise_fast();
      T y;
    }

    inline void constexpr normalize_faster() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      /**
      normalise_faster();
    }
       * Second element of vector, alias for Z-coordinate (for x-z 2D grids).
    inline Quaternion<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {
       */
      T const temp(length());
      T z;
      return Quaternion<T>(w / temp, v / temp);

    }
      /**
    inline Quaternion<T> constexpr normalise_copy_fast() const __attribute__((__always_inline__)) {
       * Second element of vector, alias for T-coordinate.
      T const temp(length_fast());
       * For textures notation.
      return Quaternion<T>(w / temp, v / temp);
       */
    }
      T t;
    inline Quaternion<T> constexpr normalise_copy_faster() const __attribute__((__always_inline__)) {
    };
      T const temp(length_faster());

      return Quaternion<T>(w / temp, v / temp);
    //----------------[ constructors ]--------------------------
    }
    /**
    inline Quaternion<T> constexpr normalize_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
     * Creates and sets to (0,0)
      return normalise_copy();
     */
    }
    inline constexpr Vector2() __attribute__((__always_inline__))
    inline Quaternion<T> constexpr normalize_copy_fast() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      : x(0), y(0) {
      return normalise_copy_fast();
    }
    }

    inline Quaternion<T> constexpr normalize_copy_faster() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
    /**
      return normalise_copy_faster();
    }
     * Creates and sets to (x,y)

     * @param nx initial x-coordinate value
    inline void constexpr conjugate() __attribute__((__always_inline__)) {
     * @param ny initial y-coordinate value
      v = -v;
     */
    }
    inline constexpr Vector2(T nx, T ny) __attribute__((__always_inline__))

      : x(nx), y(ny) {
    inline Quaternion<T> constexpr conjugate_copy() const __attribute__((__always_inline__)) {
    }
      return Quaternion<T>(w, -v);

    }
    /**

     * Copy constructor.
    /**
     * @param src Source of data for new created instance.
     * @brief Computes the inverse of this quaternion.
     */
     *
    inline constexpr Vector2(Vector2<T> const &src) __attribute__((__always_inline__))
     * @note This is a general inverse.  If you know a priori
      : x(src.x), y(src.y) {
     * that you're using a unit quaternion (i.e., norm() == 1),
    }
     * it will be significantly faster to use conjugate() instead.

     *
    /**
     * @return The quaternion q such that q * (*this) == (*this) * q
     * Copy casting constructor.
     * == [ 0 0 0 1 ]<sup>T</sup>.
     */
     * @param src Source of data for new created instance.
    inline void constexpr invert() __attribute__((__always_inline__)) {
     */
      T l = length();
    template<typename FromT>
      conjugate();
    inline constexpr Vector2(Vector2<FromT> const &src)
      (*this) /= l;
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)) {
    }
    }


    inline Quaternion<T> constexpr invert_copy() const __attribute__((__always_inline__)) {
    //----------------[ assignment ]-------------------------
      return conjugate_copy() / length();
    /**
    }
     * Sets to (x,y)

     * @param nx initial x-coordinate value
    /**
     * @param ny initial y-coordinate value
     * Creates quaternion for Euler angles.
     */
     * @param x Rotation around x axis (in degrees).
     * @param y Rotation around y axis (in degrees).
    inline void constexpr assign(T nx = 0, T ny = 0) __attribute__((__always_inline__)) {
     * @param z Rotation around z axis (in degrees).
      x = nx;
     * @return Quaternion object representing transformation.
      y = ny;
     */
    }
    inline static Quaternion<T> constexpr fromEulerAngles(T x, T y, T z) __attribute__((__always_inline__)) {

      return Quaternion<T>(fromAxisRot(Vector3<T>(1, 0, 0), x) *
    //----------------[ access operators ]-------------------
                           fromAxisRot(Vector3<T>(0, 1, 0), y) *
    /**
                           fromAxisRot(Vector3<T>(0, 0, 1), z));
     * Copy casting operator
    }
     * @param rhs Right hand side argument of binary operator.

     */
    /**
    template<typename FromT>
     * Creates quaternion for Euler angles, rad version.
    inline Vector2<T> constexpr &operator=(Vector2<FromT> const &rhs) {
     * @param x Rotation around x axis (in radians).
      x = static_cast<T>(rhs.x);
     * @param y Rotation around y axis (in radians).
      y = static_cast<T>(rhs.y);
     * @param z Rotation around z axis (in radians).
      return *this;
     * @return Quaternion object representing transformation.
    }
     */

    inline static Quaternion<T> constexpr fromEulerAngles_rad(T x, T y, T z) __attribute__((__always_inline__)) {
    /**
      return Quaternion<T>(fromAxisRot_rad(Vector3<T>(1, 0, 0), x) *
     * Copy operator
                           fromAxisRot_rad(Vector3<T>(0, 1, 0), y) *
     * @param rhs Right hand side argument of binary operator.
                           fromAxisRot_rad(Vector3<T>(0, 0, 1), z));
     */
    }
    inline Vector2<T> constexpr &operator=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {

    /**
      x = rhs.x;
     * Creates quaternion as rotation around axis.
      y = rhs.y;
     * @param axis Unit vector expressing axis of rotation.
      return *this;
     * @param angleDeg Angle of rotation around axis (in degrees).
    }
     */

    inline static Quaternion<T> constexpr fromAxisRot(Vector3<T> axis, T angleDeg) __attribute__((__always_inline__)) {
    /**
      return fromAxisRot_rad(axis, deg2rad(angleDeg));
     * Array access operator
    }
     * @param n Array index

     * @return For n = 0, reference to x coordinate, else reference to y
    /**
     * y coordinate.
     * Creates quaternion as rotation around axis, rad version.
     */
     * @param axis Unit vector expressing axis of rotation.
    inline T constexpr &operator[](int n) __attribute__((__always_inline__)) {
     * @param angleDeg Angle of rotation around axis (in radians).
      return n == 0 ? x : y;
     */
    }
    inline static Quaternion<T> constexpr fromAxisRot_rad(Vector3<T> axis, T angleRad) __attribute__((__always_inline__)) {

      T temp_sin = static_cast<T>(0);
    /**
      T temp_cos = static_cast<T>(0);
     * Constant array access operator
      sincos_any(angleRad / static_cast<T>(2.0), temp_sin, temp_cos);
     * @param n Array index
      return Quaternion<T>(temp_cos, axis * temp_sin);
     * @return For n = 0, reference to x coordinate, else reference to y
    }

     * y coordinate.
    /**
     */
     * Converts this quaternion to an axis and angle combination
    inline T constexpr const &operator[](int n) const __attribute__((__always_inline__)) {
     * @param angle The angle of rotation
      return n == 0 ? x : y;
     * @param axis The axis around which the rotation is
    }
     */

    inline void constexpr toAngleAxis(T &angle, Vector3<T> &axis) __attribute__((__always_inline__)) {
    //---------------[ vector aritmetic operator ]--------------
      T const squareLength = v.lengthSq();
    /**
      if(squareLength != 0) {
     * Addition operator
        angle = static_cast<T>(2.0) * std::acos(w);
     * @param rhs Right hand side argument of binary operator.
        axis = v / std::pow(squareLength, static_cast<T>(0.5));
     */
      } else {
    inline Vector2<T> constexpr operator+(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
        angle = static_cast<T>(0.0);
      return Vector2<T>(x + rhs.x, y + rhs.y);
        axis.assign(static_cast<T>(1.0), static_cast<T>(0.0), static_cast<T>(0.0));
    }
      }

    }
    /**

     * Subtraction operator
    /**
     * @param rhs Right hand side argument of binary operator.
     * Converts quaternion into rotation matrix.
     */
     * @return Rotation matrix expressing this quaternion.
     */
    inline Vector2<T> constexpr operator-(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
    inline Matrix3<T> constexpr rotMatrix() __attribute__((__always_inline__)) {
      return Vector2<T>(x - rhs.x, y - rhs.y);
      return Matrix3<T>(1 - 2 * (v.y * v.y + v.z * v.z),     2 * (v.x * v.y + v.z * w),       2 * (v.x * v.z - v.y * w),
    }
                            2 * (v.x * v.y - v.z * w),   1 - 2 * (v.x * v.x + v.z * v.z),     2 * (v.y * v.z + v.x * w),

                            2 * (v.x * v.z + v.y * w),       2 * (v.y * v.z - v.x * w),   1 - 2 * (v.x * v.x + v.y * v.y));
    /**
    }
     * Multiplication operator

     * @param rhs Right hand side argument of binary operator.
    /**
     */
     * Converts quaternion into transformation matrix.
    inline Vector2<T> constexpr operator*(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
     * @note This method performs same operation as rotMatrix()
      return Vector2<T>(x * rhs.x, y * rhs.y);
     * conversion method. But returns Matrix of 4x4 elements.
    }
     * @return Transformation matrix expressing this quaternion.

     */
    /**
    inline Matrix4<T> constexpr transform() const __attribute__((__always_inline__)) {
     * Division operator
      return Matrix4<T>(1 - 2 * (v.y * v.y + v.z * v.z),     2 * (v.x * v.y + v.z * w),       2 * (v.x * v.z - v.y * w),   0.0f,
     * @param rhs Right hand side argument of binary operator.
                            2 * (v.x * v.y - v.z * w),   1 - 2 * (v.x * v.x + v.z * v.z),     2 * (v.y * v.z + v.x * w),   0.0f,
     */
                            2 * (v.x * v.z + v.y * w),       2 * (v.y * v.z - v.x * w),   1 - 2 * (v.x * v.x + v.y * v.y), 0.0f,
    inline Vector2<T> constexpr operator/(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
                        0.0f,                            0.0f,                            0.0f,                            1.0f);
      return Vector2<T>(x / rhs.x, y / rhs.y);
    }
    }


    /**
    /**
     * Linear interpolation of two quaternions
     * Addition operator
     * @param fact Factor of interpolation. For translation from position
     * @param rhs Right hand side argument of binary operator.
     * of this vector to quaternion rhs, values of factor goes from 0.0 to 1.0.
     */
     * @param rhs Second Quaternion for interpolation
    inline Vector2<T> constexpr &operator+=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
     * @note However values of fact parameter are reasonable only in interval
      x += rhs.x;
     * [0.0 , 1.0], you can pass also values outside of this interval and you
      y += rhs.y;
     * can get result (extrapolation?)
      return *this;
     */
    }
    inline Quaternion<T> constexpr lerp(T fact, Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {

      return Quaternion<T>((1 - fact) * w + fact * rhs.w, v.lerp(fact, rhs.v));
    /**
    }

     * Substraction operator
    /**
     * @param rhs Right hand side argument of binary operator.
     * Provides output to standard output stream.
     */
     */
    inline Vector2<T> constexpr &operator-=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
    inline friend std::ostream &operator <<(std::ostream &oss, Quaternion<T> const &q) __attribute__((__always_inline__)) {
      x -= rhs.x;
      oss << "Re: " << q.w << " Im: " << q.v;
      y -= rhs.y;
      return oss;
      return *this;
    }
    }


    /**
    /**
     * Gets string representation.
     * Multiplication operator
     */
     * @param rhs Right hand side argument of binary operator.
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
     */
      std::ostringstream oss;
    inline Vector2<T> constexpr &operator*=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
      oss << *this;
      x *= rhs.x;
      return oss.str();
      y *= rhs.y;
    }

      return *this;
    /**
    }
     * Creates quaternion from transform matrix.

     *
    /**
     * @param m Transform matrix used to compute quaternion.
     * Division operator
     * @return Quaternion representing rotation of matrix m.
     * @param rhs Right hand side argument of binary operator.
     */
     */
    // 2011-07-02: Davide Bacchet: changed formula to fix degenerate cases
    inline Vector2<T> constexpr &operator/=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
    inline static Quaternion<T> constexpr fromMatrix(Matrix4<T> const &m) __attribute__((__always_inline__)) {
      x /= rhs.x;
      Quaternion<T> q;
      y /= rhs.y;

      return *this;
      T const tr = m(1, 1) + m(2, 2) + m(3, 3);
    }
      if(tr >= static_cast<T>(epsilon)) {

        T const s = static_cast<T>(0.5) / static_cast<T>(std::sqrt(tr + static_cast<T>(1.0)));
    /**
        q.w = static_cast<T>(0.25) / s;
     * Dot product of two vectors.
        q.v.x = (m(3, 2) - m(2, 3)) * s;
     * @param rhs Right hand side argument of binary operator.
        q.v.y = (m(1, 3) - m(3, 1)) * s;
     */
        q.v.z = (m(2, 1) - m(1, 2)) * s;
    inline T constexpr dotProduct(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
      } else {
      return x * rhs.x + y * rhs.y;
        T d0 = m(1, 1);
    }
        T d1 = m(2, 2);

        T d2 = m(3, 3);

    /**
        char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);
     * Cross product of two vectors

     * @param rhs Right hand side argument of binary operator.
        if(bigIdx == 0) {
     */
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(1, 1) - m(2, 2) - m(3, 3)));
    inline T constexpr crossProduct(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
          q.w = (m(3, 2) - m(2, 3)) / s;
      return x * rhs.y - y * rhs.x;
          q.v.x = static_cast<T>(0.25) * s;
    }
          q.v.y = (m(1, 2) + m(2, 1)) / s;

          q.v.z = (m(1, 3) + m(3, 1)) / s;
    //--------------[ scalar vector operator ]--------------------
        } else if(bigIdx == 1) {
    /**
          T s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(2, 2) - m(1, 1) - m(3, 3)));
     * Addition operator
          q.w = (m(1, 3) - m(3, 1)) / s;
     * @param rhs Right hand side argument of binary operator.
          q.v.x = (m(1, 2) + m(2, 1)) / s;
     */
          q.v.y = static_cast<T>(0.25) * s;
    inline Vector2<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
          q.v.z = (m(2, 3) + m(3, 2)) / s;
        } else {
      return Vector2<T>(x + rhs, y + rhs);
          T s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(3, 3) - m(1, 1) - m(2, 2)));
    }
          q.w = (m(2, 1) - m(1, 2)) / s;

          q.v.x = (m(1, 3) + m(3, 1)) / s;
    /**
          q.v.y = (m(2, 3) + m(3, 2)) / s;
     * Subtraction operator
          q.v.z = static_cast<T>(0.25) * s;
     * @param rhs Right hand side argument of binary operator.
        }
     */
      }
    inline Vector2<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {

      return Vector2<T>(x - rhs, y - rhs);
      return q;
    }
    }


    /**
    /**
     * Multiplication operator
     * Creates quaternion from rotation matrix.
     *
     * @param rhs Right hand side argument of binary operator.
     * @param m Rotation matrix used to compute quaternion.
     */
     * @return Quaternion representing rotation of matrix m.
    inline Vector2<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
     */
      return Vector2<T>(x * rhs, y * rhs);

    }
    // 2011-07-02: Davide Bacchet: changed formula to fix degenerate cases

    inline static Quaternion<T> constexpr fromMatrix(Matrix3<T> const &m) __attribute__((__always_inline__)) {
    /**
      Quaternion<T> q;
     * Division operator

     * @param rhs Right hand side argument of binary operator.
      T const tr = m(1, 1) + m(2, 2) + m(3, 3);
     */
      if(tr >= static_cast<T>(epsilon)) {
    inline Vector2<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
        T const s = static_cast<T>(0.5) / static_cast<T>(std::sqrt(tr + static_cast<T>(1.0)));
      return Vector2<T>(x / rhs, y / rhs);
        q.w = static_cast<T>(0.25) / s;
    }
        q.v.x = (m(3, 2) - m(2, 3)) * s;

        q.v.y = (m(1, 3) - m(3, 1)) * s;
    /**
        q.v.z = (m(2, 1) - m(1, 2)) * s;
     * Addition operator
      } else {
     * @param rhs Right hand side argument of binary operator.
        T d0 = m(1, 1);
     */
        T d1 = m(2, 2);
        T d2 = m(3, 3);
    inline Vector2<T> constexpr &operator+=(T rhs) __attribute__((__always_inline__)) {

      x += rhs;
        char bigIdx = (d0 > d1) ? ((d0 > d2) ? 0 : 2) : ((d1 > d2) ? 1 : 2);
      y += rhs;

      return *this;
        if(bigIdx == 0) {
    }
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(1, 1) - m(2, 2) - m(3, 3)));

          q.w = (m(3, 2) - m(2, 3)) / s;
    /**
          q.v.x = static_cast<T>(0.25) * s;
     * Subtraction operator
          q.v.y = (m(1, 2) + m(2, 1)) / s;
     * @param rhs Right hand side argument of binary operator.
          q.v.z = (m(1, 3) + m(3, 1)) / s;
     */
        } else if(bigIdx == 1) {
    inline Vector2<T> constexpr &operator-=(T rhs) __attribute__((__always_inline__)) {
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(2, 2) - m(1, 1) - m(3, 3)));
      x -= rhs;
          q.w = (m(1, 3) - m(3, 1)) / s;
      y -= rhs;
          q.v.x = (m(1, 2) + m(2, 1)) / s;
      return *this;
          q.v.y = static_cast<T>(0.25) * s;
    }
          q.v.z = (m(2, 3) + m(3, 2)) / s;

        } else {
    /**
          T const s = static_cast<T>(2.0) * static_cast<T>(std::sqrt(static_cast<T>(1.0) + m(3, 3) - m(1, 1) - m(2, 2)));
     * Multiplication operator
          q.w = (m(2, 1) - m(1, 2)) / s;
     * @param rhs Right hand side argument of binary operator.
          q.v.x = (m(1, 3) + m(3, 1)) / s;
     */
          q.v.y = (m(2, 3) + m(3, 2)) / s;
          q.v.z = static_cast<T>(0.25) * s;
    inline Vector2<T> constexpr &operator*=(T rhs) __attribute__((__always_inline__)) {
        }
      x *= rhs;
      }
      y *= rhs;

      return *this;
      return q;
    }
    }


    /**
    /**
     * Division operator
     * Computes spherical interpolation between quaternions (this, q2)
     * @param rhs Right hand side argument of binary operator.
     * using coefficient of interpolation r (in [0, 1]).
     */
     *
    inline Vector2<T> constexpr &operator/=(T rhs) __attribute__((__always_inline__)) {
     * @param r The ratio of interpolation form this (r = 0) to q2 (r = 1).
      x /= rhs;
     * @param q2 Second quaternion for interpolation.
      y /= rhs;
     * @return Result of interpolation.
      return *this;
     */
    }
    inline Quaternion<T> constexpr slerp(T r, Quaternion<T> const &q2) const __attribute__((__always_inline__)) {

      Quaternion<T> ret;
    //--------------[ equality operator ]------------------------
      T const cosTheta = w * q2.w + v.x * q2.v.x + v.y * q2.v.y + v.z * q2.v.z;
    /**
      T const theta = static_cast<T>(std::acos(cosTheta));
     * Equality test operator
      if(std::abs(theta) < static_cast<T>(epsilon)) {
        ret = *this;
     * @param rhs Right hand side argument of binary operator.
      } else {
     * @note Test of equality is based of threshold epsilon value. To be two
        T sinTheta = static_cast<T>(std::sqrt(static_cast<T>(1.0) - cosTheta * cosTheta));
     * values equal, must satisfy this condition | lhs.x - rhs.y | < epsilon,
        if(std::abs(sinTheta) < static_cast<T>(epsilon)) {
     * same for y-coordinate.
          ret.w = static_cast<T>(0.5) * w + static_cast<T>(0.5) * q2.w;
     */
          ret.v = v.lerp(static_cast<T>(0.5), q2.v);
    inline bool constexpr operator==(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
        } else {
      #ifdef VMATH_SOFT_COMPARE
          T rA = static_cast<T>(std::sin((static_cast<T>(1.0) - r) * theta)) / sinTheta;
        return (std::abs(x - rhs.x) < static_cast<T>(epsilon)) &&
          T rB = static_cast<T>(std::sin(r * theta)) / sinTheta;
               (std::abs(y - rhs.y) < static_cast<T>(epsilon));

      #else
          ret.w = w * rA + q2.w * rB;
        return x == rhs.x &&
          ret.v.x = v.x * rA + q2.v.x * rB;
               y == rhs.y;
          ret.v.y = v.y * rA + q2.v.y * rB;
      #endif
          ret.v.z = v.z * rA + q2.v.z * rB;
    }
        }

      }
    /**
      return ret;
    }
     * Inequality test operator

     * @param rhs Right hand side argument of binary operator.
};
     * @return not (lhs == rhs) :-P

     */
/// Quaternion of floats
    inline bool constexpr operator!=(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
using Quatf = Quaternion<float>;
      return !(*this == rhs);
/// Quaternion of doubles
    }
using Quatd = Quaternion<double>;

/// Quaternion of long doubles
    //-------------[ unary operations ]--------------------------
using Quatld = Quaternion<long double>;
    /**

     * Unary negate operator
#ifdef VMATH_NAMESPACE
     * @return negated vector
}
     */
#endif

    inline Vector2<T> constexpr operator-() const __attribute__((__always_inline__)) {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      return Vector2<T>(-x, -y);
//
    }
// Standard C++ library extensions

//
    //-------------[ size operations ]---------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**

     * Return square of length.
// Shortcut defines
     * @return length ^ 2
#ifdef VMATH_NAMESPACE
     * @note This method is faster then length(). For comparison
#define VEC2 VMATH_NAMESPACE::Vector2
     * of length of two vector can be used just this value, instead
#define VEC3 VMATH_NAMESPACE::Vector3
     * of more expensive length() method.
#define VEC4 VMATH_NAMESPACE::Vector4
     */
#else
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
#define VEC2 Vector2
      return x * x + y * y;
#define VEC3 Vector3
    }
#define VEC4 Vector4

#endif

    /**
namespace std {
     * Get length of vector.

     * @return length of vector
/**
     */
 * Gets vector containing minimal values of @a a and @a b coordinates.
    inline T constexpr length() const __attribute__((__always_inline__)) {
 * @return Vector of minimal coordinates.
      return static_cast<T>(std::sqrt(lengthSq()));
 */
    }
template<typename T>
    /**
constexpr VEC2<T> min(VEC2<T> const &a, const VEC2<T> &b) {
     * Get length of vector, fast approximation.
  return VEC2<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y));
     * @return length of vector
}
     */

    inline T constexpr length_fast() const __attribute__((__always_inline__)) {
/**
      return static_cast<T>(sqrt_fast(lengthSq()));
 * Gets vector containing minimal values of @a a and @a b coordinates.
    }
 * @return Vector of minimal coordinates.
    /**
 */
template<typename T>
     * Get length of vector, rougher fast approximation.
constexpr VEC3<T> min(VEC3<T> const &a, const VEC3<T> &b) {
     * @return length of vector
  return VEC3<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y), ::std::min(a.z, b.z));
     */
}
    inline T constexpr length_faster() const __attribute__((__always_inline__)) {

      return static_cast<T>(sqrt_faster(lengthSq()));
/**
    }
 * Gets vector containing minimal values of @a a and @a b coordinates.

 * @return Vector of minimal coordinates.
    /**
 */
     * normalise vector
template<typename T>
     */
constexpr VEC4<T> min(VEC4<T> const &a, const VEC4<T> &b) {
    inline void constexpr normalise() __attribute__((__always_inline__)) {
  return VEC4<T>(::std::min(a.x, b.x), ::std::min(a.y, b.y), ::std::min(a.z, b.z), ::std::min(a.w, b.w));
      T const temp = length();
}
      x /= temp;

      y /= temp;
/**
    }
 * Gets vector containing maximal values of @a a and @a b coordinates.
    inline void constexpr normalise_fast() __attribute__((__always_inline__)) {
 * @return Vector of maximal coordinates.
 */
      T const temp = length_fast();
template<typename T>
      x /= temp;
constexpr VEC2<T> max(VEC2<T> const &a, const VEC2<T> &b) {
      y /= temp;
  return VEC2<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y));
    }
}
    inline void constexpr normalise_faster() __attribute__((__always_inline__)) {

      T const temp = length_faster();
/**
      x /= temp;
 * Gets vector containing maximal values of @a a and @a b coordinates.
      y /= temp;
 * @return Vector of maximal coordinates.
    }
 */
    inline void constexpr normalize() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
template<typename T>
      normalise();
constexpr VEC3<T> max(VEC3<T> const &a, const VEC3<T> &b) {
    }
  return VEC3<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y), ::std::max(a.z, b.z));
}
    inline void constexpr normalize_fast() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {

      normalise_fast();
/**
    }
 * Gets vector containing maximal values of @a a and @a b coordinates.
    inline void constexpr normalize_faster() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
 * @return Vector of maximal coordinates.
      normalise_faster();
 */
    }
template<typename T>
    inline Vector2<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {
constexpr VEC4<T> max(VEC4<T> const &a, const VEC4<T> &b) {
      T const temp(length());
  return VEC4<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y), ::std::max(a.z, b.z), ::std::max(a.w, b.w));
      return Vector2<T>(x / temp, y / temp);
}
    }
}

    inline Vector2<T> constexpr normalise_copy_fast() const __attribute__((__always_inline__)) {
// cleanup shortcut defines
      T const temp(length_fast());
#undef VEC2
      return Vector2<T>(x / temp, y / temp);
#undef VEC3
    }
#undef VEC4
    inline Vector2<T> constexpr normalise_copy_faster() const __attribute__((__always_inline__)) {

      T const temp(length_faster());
#ifdef VMATH_NAMESPACE
      return Vector2<T>(x / temp, y / temp);
namespace VMATH_NAMESPACE {
    }
#endif //VMATH_NAMESPACE
    inline Vector2<T> constexpr normalize_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {

      return normalise_copy();
/**
    }
 * Two-dimensional axis-aligned bounding-box (aka AABB) class.
 *
    inline Vector2<T> constexpr normalize_copy_fast() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
 * This class provides functionality for:
      return normalise_copy_fast();
 * - creating an AABB from a point, or another AABB,
    }
 * - testing if a point of other AABB intersects with it,
    inline Vector2<T> constexpr normalize_copy_faster() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
 * - getting result of intersection with other AABB,
      return normalise_copy_faster();
 * - transforming AABB with 4x4 matrix.
    }
 *
    /**
 * There are also overloaded couple of operators to shorten common operations.
     * normalise vector. with added zero safety check
 * For instance you can use  @c operator<< on AABB to extend it with a passed point or other AABB.
     */
 * @code
    inline void constexpr normalise_safe() __attribute__((__always_inline__)) {
 * Aabb2f aabb;
      T const temp = length();
 * aabb << Vector2f(1, 2) << Aabb2f(-3,-3, 2, 2);
      if(temp == static_cast<T>(0)) {
 * @endcode
        x = static_cast<T>(0);
 */
        y = static_cast<T>(0);
template<typename T>
      } else {
class Aabb2 {
        x /= temp;
  public:
        y /= temp;
    /**
      }
     * Position of Min corner of bounding box.
     */
    }
    Vector2<T> min;
    inline void constexpr normalize_safe() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {

      normalise_safe();
    /**
    }
     * Position of Max corner of bounding box
    inline Vector2<T> constexpr normalise_safe_copy() const __attribute__((__always_inline__)) {
     */
      T const temp(length());
    Vector2<T> max;
      if(temp == static_cast<T>(0)) {

        return Vector2<T>();
    /**
      } else {
     * Constructs invalid axes-aligned bounding-box.
        return Vector2<T>(x / temp, y / temp);
     * @see valid() for explanation of invalid bounding-box usage.
      }
     */
    }
    inline constexpr Aabb2() __attribute__((__always_inline__))
      : min(1, 1), max(-1, -1) {
    inline Vector2<T> constexpr normalize_safe_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
    }
      return normalise_safe_copy();
    }


    /**
    /**
     * Absolute vector (make all values positive)
     * Constructs axes-aligned bound-box containing one point @a point
     */
     * @param point
    inline void constexpr abs() __attribute__((__always_inline__)) {
     */
      x = std::abs(x);
    template<typename SrcT>
      y = std::abs(y);
    inline constexpr Aabb2(Vector2<SrcT> const &point)
    }
      : min(point), max(point) {
    inline Vector2<T> constexpr abs_copy() const __attribute__((__always_inline__)) {
    }

      return Vector2<T>(std::abs(x), std::abs(y));
    /**
    }
     * Constructs axes-aligned bounding-box form two corner points (@a x0, @a y0)

     * and (@a x1, @a y1)
    //-------------[ other operations ]--------------------------
     * @param x0 X-coordinate of first point
    /**
     * @param y0 Y-coordinate of first point
     * Rotate 2D vector clockwise.
     * @param x1 X-coordinate of second point
     * @param aa Angle (in degrees) to be rotated.
     * @param y1 Y-coordinate of second point
     */
     */
    inline void constexpr rotate(T aa) __attribute__((__always_inline__)) {
    template<typename SrcT>
      rotate_rad(deg2rad(aa));
    inline constexpr Aabb2(SrcT x0, SrcT y0, SrcT x1, SrcT y1)
    }
      : min(std::min(x0, x1), std::min(y0, y1)),

        max(std::max(x0, x1), std::max(y0, y1)) {
    /**
    }

     * Rotate 2D vector clockwise, radian version.
    /**
     * @param aa Angle (in radians) to be rotated.
     * Constructs axes-aligned bounding-box from @a min and @b max
     */
     * @param min X-coordinate of first point
    inline void constexpr rotate_rad(T aa) __attribute__((__always_inline__)) {
     * @param max X-coordinate of second point
      T temp_sin = static_cast<T>(0);
     */
      T temp_cos = static_cast<T>(0);
    template<typename SrcT>
      sincos_any(-aa, temp_sin, temp_cos);
    inline constexpr Aabb2(Vector2<SrcT> const &new_min, Vector2<SrcT> const &new_max)
      T const nx = static_cast<T>((temp_cos * x) - (temp_sin * y));
      : min(new_min),
      T const ny = static_cast<T>((temp_cos * y) + (temp_sin * x));
        max(new_max) {
      x = nx;
    }
      y = ny;

    }
    /**

     * Constructs axes-aligned bounding-box containing point (@a x, @a y)
    /**
     * @param x X-coordinate of point
     * Linear interpolation of two vectors
     * @param y Y-coordinate of point
     */
     * @param fact Factor of interpolation. For translation from position
    template<typename SrcT>
     * of this vector to vector r, values of factor goes from 0.0 to 1.0.
    inline constexpr Aabb2(SrcT x, SrcT y)
     * @param r Second Vector for interpolation
      : min(x, y), max(x, y) {
     * @note However values of fact parameter are reasonable only in interval
    }
     * [0.0 , 1.0], you can pass also values outside of this interval and you

     * can get result (extrapolation?)
    /**
     */
     * Creates copy of axis-aligned bounding-box
    inline Vector2<T> constexpr lerp(T fact, Vector2<T> const &r) const __attribute__((__always_inline__)) {
     * @param src Source bounding-box
      return (*this) + (r - (*this)) * fact;
     */
    }
    template<typename SrcT>

    inline constexpr Aabb2(Aabb2<SrcT> const &src)
      : min(src.min), max(src.max) {
    //-------------[ conversion ]-----------------------------
    }
    /**

     * Conversion to pointer operator
    /**
     * @return Pointer to internally stored (in management of class Vector2<T>)
     * Assign operator
     * used for passing Vector2<T> values to gl*2[fd] functions.
     * @param rhs source bounding-box
     */
     * @return refenrence to this
    inline constexpr operator T*() __attribute__((__always_inline__)) {
     */
      return reinterpret_cast<T*>(this);
    template<typename SrcT>
    }
    inline Aabb2<T> constexpr &operator=(Aabb2<SrcT> const &rhs) {
    /**
      min = rhs.min;
     * Conversion to pointer operator
      max = rhs.max;
      return *this;
     * @return Constant Pointer to internally stored (in management of class Vector2<T>)
    }
     * used for passing Vector2<T> values to gl*2[fd] functions.

     */
    /**
    inline constexpr operator const T*() const __attribute__((__always_inline__)) {
     * Checks if bounding-box is valid. Valid bounding-box has non-negative size.
      return reinterpret_cast<T const*>(this);
     * If an invalid bounding-box is extended by point or another bounding-box, the target
    }
     * bounding box becomes valid and contains solely the source point or bounding-box respectively.

     * @return True if box is valid, otherwise false
    //-------------[ output operator ]------------------------
     */
    /**
    inline bool constexpr valid() const __attribute__((__always_inline__)) {
     * Output to stream operator
      return min.x <= max.x && min.y <= max.y;
     * @param lhs Left hand side argument of operator (commonly ostream instance).
    }

     * @param rhs Right hand side argument of operator.
    /**
     * @return Left hand side argument - the ostream object passed to operator.
     * Makes this bounding-box invalid. So calling valid() gets false.
     */
     * @see valid() method for more info on usage of invalid bounding-boxes.
    inline friend std::ostream &operator<<(std::ostream &lhs, Vector2<T> const &rhs) __attribute__((__always_inline__)) {
     */
      lhs << "[" << rhs.x << "," << rhs.y << "]";
    inline void constexpr invalidate() __attribute__((__always_inline__)) {
      return lhs;
      min = Vector2<T>( 1,  1);
    }
      max = Vector2<T>(-1, -1);

    }
    /**

     * Gets string representation.
    /**
     */
     * Extends this bounding-box by a point @a point.
    inline std::string constexpr toString() const __attribute__((__always_inline__)) {
     * @param point A point to extend bounding-box by.
      std::ostringstream oss;
     */
      oss << *this;
    template<typename SrcT>
      return oss.str();
    inline void constexpr extend(Vector2<SrcT> const &point) {
    }
      if(!valid()) {

        min = max = point;
    /**
      } else {
        min = std::min(min, point);
     * Check whether two lines intersect, and if so obtain the intersection point
        max = std::max(max, point);
     * and store it in this vector.
      }
     * @param line1start The start coordinates of the first line
    }
     * @param line1end The end coordinates of the first line

     * @param line2start The start coordinates of the first line
    /**
     * @param line2end The end coordinates of the first line
     * Extends this bounding-box by a box @a box.
     */
     * @param box A box to extend this bounding-box by.
    template<typename FromT>
     */
    inline bool constexpr get_line_intersection(Vector2<FromT> const &line1start,
    template<typename SrcT>
                                                Vector2<FromT> const &line1end,
    inline void constexpr extend(Aabb2<SrcT> const &box) {
      if(!valid()) {
                                                Vector2<FromT> const &line2start,
        min = box.min;
                                                Vector2<FromT> const &line2end) {
        max = box.max;
      Vector2<T> const temp_s1(line1end.x - line1start.x,
      } else {
                               line1end.y - line1start.y);
        min = std::min(min, box.min);
      Vector2<T> const temp_s2(line2end.x - line2start.x,
        max = std::max(max, box.max);
                               line2end.y - line2start.y);
      }
      T const temp_s = (-temp_s1.y * (line1start.x - line2start.x) + temp_s1.x * (line1start.y - line2start.y)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
    }

      T const temp_t = ( temp_s2.x * (line1start.y - line2start.y) - temp_s2.y * (line1start.x - line2start.x)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
    /**
      if(temp_s >= 0 && temp_s <= 1 && temp_t >= 0 && temp_t <= 1) {
     * Gets a copy of this bounding-box extend by a point @a point.
        x = line1start.x + (temp_t * temp_s1.x);
     * @param point A point to extend the box by
        y = line1start.y + (temp_t * temp_s1.y);
     * @return Copy of extended bounding-box
        return true;
     */
      }
    template<typename SrcT>
      return false;
    inline constexpr Aabb2<T> extended(Vector2<SrcT> const &point) const {
    }
      Aabb2<T> ret(*this);

      ret.extend(point);
    /**
      return ret;
     * Free function to check whether two lines intersect.
    }
     * @param line1start The start coordinates of the first line

     * @param line1end The end coordinates of the first line
    /**
     * @param line2start The start coordinates of the first line
     * Gets a copy of this bounding-box extnended by box @a box.
     * @param line2end The end coordinates of the first line
     * @param box A box to extend the copy be.
     */
     * @return Copy of extended bounding-box
     */
    template<typename FromT>
    template<typename SrcT>
    inline static bool constexpr do_lines_intersect(Vector2<FromT> const &line1start,
    inline constexpr Aabb2<T> extended(Aabb2<SrcT> const &box) const {
                                                    Vector2<FromT> const &line1end,
      Aabb2<T> ret(*this);
                                                    Vector2<FromT> const &line2start,
      ret.extend(box);
                                                    Vector2<FromT> const &line2end) {
      return *this;
      Vector2<T> constexpr temp_s1(line1end.x - line1start.x,
    }
                              line1end.y - line1start.y);

      Vector2<T> constexpr temp_s2(line2end.x - line2start.x,
    /**
                              line2end.y - line2start.y);
     * Tests if the point @a point is within this bounding-box
     * @param point A point to be tested
      T constexpr temp_s = (-temp_s1.y * (line1start.x - line2start.x) + temp_s1.x * (line1start.y - line2start.y)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
     * @return True if point @a point lies within bounding-box, otherwise false.
      T constexpr temp_t = ( temp_s2.x * (line1start.y - line2start.y) - temp_s2.y * (line1start.x - line2start.x)) / (-temp_s2.x * temp_s1.y + temp_s1.x * temp_s2.y);
     */
      if(temp_s >= 0 && temp_s <= 1 && temp_t >= 0 && temp_t <= 1) {
    template<typename SrcT>
        return true;
    inline bool constexpr intersects(Vector2<SrcT> const &point) const {
      }
      return min.x <= point.x && point.x <= max.x &&
      return false;
             min.y <= point.y && point.y <= max.y;
    }
    }


    /**
    /**
     * Gets a 3D vector equivalent populating the X and Y axes
     * Tests if other bounding-box @a box intersects (even partially) with this bounding-box.
     */
     * @param box A box to be tested for intersection.
    inline Vector3<T> constexpr to_3D_XY() const __attribute__((__always_inline__)) {
     * @return True if there's intersection between boxes, otherwise false.
      return Vector3<T>(x, y, 0);
     */
    }
    template<typename SrcT>
    inline Vector3<T> constexpr to_3d_xy() const __attribute__((__always_inline__)) {
    inline bool constexpr intersects(Aabb2<SrcT> const &box) const {
      return Vector3<T>(x, y, 0);
      return max.x >= box.min.x && min.x <= box.max.x &&
    }
             max.y >= box.min.y && min.y <= box.max.y;

    }
    /**

    /**
     * Gets a 3D vector equivalent populating the X and Z axes
     * Gets result of intersection of this bounding-box with @a other bounding-box.
     */
     * In case the boxes don't intersect, the returned bounding-box is invalid.
    inline Vector3<T> constexpr to_3D_XZ() const __attribute__((__always_inline__)) {
     * @param other Box to be tested
      return Vector3<T>(x, 0, z);
     * @return Result of intersection.
    }
     * @see valid() method for more information on invalid bounding-boxes.
    inline Vector3<T> constexpr to_3d_xz() const __attribute__((__always_inline__)) {
     */
      return Vector3<T>(x, 0, z);
    template<typename SrcT>
    }
    inline constexpr Aabb2<T> intersection(Aabb2<SrcT> const &other) const {
};
      return (max.x < other.min.x || min.x > other.max.x ||

              max.y < other.min.y || min.y > other.max.y) ? Aabb2<T>() : Aabb2<T>(std::max(min, other.min), std::min(max, other.max));
//--------------------------------------
    }
// Typedef shortcuts for 2D vector

//-------------------------------------
    /**
/// Two dimensional Vector of floats
     * Tests if a ray @b from origin @a intersects with this bounding-box.
using Vector2f = Vector2<float>;
     * @param origin Origin of the intersecting ray
/// Two dimensional Vector of doubles
     * @param ray The intersecting ray; does not need to be normalised
using Vector2d = Vector2<double>;
     * @return True if the ray intersects the box, otherwise false.
     */
/// Two dimensional Vector of long doubles
    template<typename SrcT>
using Vector2ld = Vector2<long double>;
    inline bool constexpr ray_intersects(Vector2<SrcT> const &ray, Vector2<SrcT> const &origin = Vector2<SrcT>()) const {
/// Two dimensional Vector of ints
      // adapted from http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
using Vector2i = Vector2<int>;
      /*

      SrcT const dist_min_x = (min.x - origin.x) / ray.x;
/**
      SrcT const dist_max_x = (max.x - origin.x) / ray.x;
 * Class for three dimensional vector.
      SrcT dist_min = std::min(dist_min_x, dist_max_x);
 * There are four ways of accessing vector components.
      SrcT dist_max = std::max(dist_min_x, dist_max_x);
 * Let's have <code>Vector3f v</code>, you can either:

 * <ul>
      SrcT const dist_min_y = (min.y - origin.y) / ray.y;
 *  <li>access as position (x,y,z) &mdash; <code>v.x = v.y = v.z = 1;</code></li>
      SrcT const dist_max_y = (max.y - origin.y) / ray.y;
 *  <li>access as texture coordinate (s,t,u) &mdash; <code>v.s = v.t = v.u = 1;</code></li>
      dist_min = std::max(dist_min, std::min(dist_min_y, dist_max_y));
 *  <li>access as colour (r,g,b) &mdash; <code>v.r = v.g = v.b = 1;</code></li>
      dist_max = std::min(dist_max, std::max(dist_min_y, dist_max_y));
 *  <li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = 1;</code></li>

 * </ul>
      return dist_max >= std::max(dist_min, 0.0f);
 */
      */
template<typename T>
      // constexpr-compatible reformulation:
class Vector3 {
      return std::min(std::max((min.x - origin.x) / ray.x,
  public:
                               (max.x - origin.x) / ray.x),
    //T x, y, z;
                      std::max((min.y - origin.y) / ray.y,
    union {
                               (max.y - origin.y) / ray.y)) >=
      /**
             std::max(std::max(std::min((min.x - origin.x) / ray.x,
       * First element of vector, alias for X-coordinate.
                                        (max.x - origin.x) / ray.x),
       */
                               std::min((min.y - origin.y) / ray.y,
      T x;
                                        (max.y - origin.y) / ray.y)), 0.0f);

    }

      /**
    /**
       * First element of vector, alias for S-coordinate.
     * Gets center point of bounding-box.
       * For textures notation.
     * @return A center point of bounding-box.
       */
     */
      T s;
    inline constexpr Vector2<T> center() const __attribute__((__always_inline__)) {

      return (min + max) * static_cast<T>(0.5);
      /**
    }
       * First element of vector, alias for R-coordinate.

       * For colour notation.
    /**
       */
     * Gets extent of bounding-box.
      T r;
     * @return Extent of bounding-box.
    };
     */

    inline constexpr Vector2<T> extent() const __attribute__((__always_inline__)) {
    union {
      return (max - min) * static_cast<T>(0.5);
      /**
    }

       * Second element of vector, alias for Y-coordinate.
    /**
       */
     * Gets diagonal size of bounding-box
      T y;
     * @return Sizes for particular dimensions.
      /**
     */
       * Second element of vector, alias for T-coordinate.
    inline constexpr Vector2<T> size() const __attribute__((__always_inline__)) {
       * For textures notation.
      return max - min;
       */
    }
      T t;

      /**
    /**
       * Second element of vector, alias for G-coordinate.
     * Gets all 4 corner-points of bounding box
       * For colour notation.
     * @param i An index of bounding-box corner point. Valid values are 0 .. 3.
       */
     * @return A position of @a i-th corner-point.
      T g;
     * @note The order of points is as follows (where @c + denotes max-point and @c - min-point):
    };
     * 1. (@c + @c + @c +)

     * 2. (@c - @c + @c +)
    union {
     * 3. (@c + @c - @c +)
      /**
     * 4. (@c - @c - @c +)
       * Third element of vector, alias for Z-coordinate.
     *
       */
     */
      T z;
    inline constexpr Vector2<T> point(unsigned int i) const __attribute__((__always_inline__)) {

      return Vector2<T>(i & 1 ? min.x : max.x,
      /**
                        i & 2 ? min.y : max.y);
    }
       * Third element of vector, alias for U-coordinate.

       * For textures notation.
    //-------------------------------------------------------------------------------------------------------------
       */
    // operators
      T u;
    //-------------------------------------------------------------------------------------------------------------
      /**
    /**
       * Third element of vector, alias for B-coordinate.
     * Tests if @a rhs is equal to this bounding-box
       * For colour notation.
     * @param rhs Right-hand side
       */
     * @return True if @a rhs and this bounding-boxes are equal, otherwise false
      T b;
     */
    };
    template<typename RhsT>

    inline bool constexpr operator==(Aabb2<RhsT> const &rhs) const {
    //----------------[ constructors ]--------------------------
      return min == rhs.min && max == rhs.max;
    /**
    }
     * Creates and sets to (0,0,0)

     */
    /**
    inline constexpr Vector3() __attribute__((__always_inline__))
     * Tests if @a rhs is not equal to this bounding-box
      : x(0), y(0), z(0) {
     * @param rhs Right-hand side
    }
     * @return True if @a rhs and this bounding-boxes are not equal, otherwise false

     */
    /**
    template<typename RhsT>
     * Creates and sets to (x,y,z)
    inline bool constexpr operator!=(Aabb2<RhsT> const &rhs) const {
     * @param nx initial x-coordinate value
      return min != rhs.min || max != rhs.max;
    }
     * @param ny initial y-coordinate value

     * @param nz initial z-coordinate value
    /**
     */
     * Extends this bounding-box by point @a rhs.
    inline constexpr Vector3(T nx, T ny, T nz) __attribute__((__always_inline__))
     * @param rhs A point to extend this bounding-box by
      : x(nx), y(ny), z(nz) {
     * @return Reference to this
    }
     */

    template<typename SrcT>
    /**
    inline Aabb2<T> constexpr &operator<<(Vector2<SrcT> const &rhs) {
     * Copy constructor.
      extend(rhs);
     * @param src Source of data for new created Vector3 instance.
      return *this;
     */
    }
    inline constexpr Vector3(Vector3<T> const &src) __attribute__((__always_inline__))

    /**
      : x(src.x), y(src.y), z(src.z) {
     * Extends this bounding-box by box @a rhs.
    }
     * @param rhs A box to extend this bounding-box by

     * @return Reference to this
    /**
     */
     * Copy casting constructor.
    template<typename SrcT>
     * @param src Source of data for new created Vector3 instance.
    inline Aabb2<T> constexpr &operator<<(Aabb2<SrcT> const &rhs) {
     */
      extend(rhs);
    template<typename FromT>
      return *this;
    inline constexpr Vector3(Vector3<FromT> const &src)
    }
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)) {

    }
    /**

     * Union of this and @a rhs bounding-boxes
    inline constexpr Vector3(Vector2<T> const &src, T new_z)
     * @param rhs Right-hand side of union
      : x(src.x), y(src.y), z(new_z) {
     * @return A resulting bounding-box representing union
    }
     */

    template<typename RhsT>
    template<typename FromT>
    inline Aabb2<T> constexpr operator|(Aabb2<RhsT> const &rhs) const {
      return extended(rhs);
    inline constexpr Vector3(Vector2<FromT> const &src, FromT new_z)
    }
      : x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(new_z)) {

    }
    /**

     * Intersection of this and @a rhs bounding-boxed
    //----------------[ assignment ]-------------------------
     * @param rhs Right-hand side
    /**
     * @return Resulting bounding-box representing the intersection.
     * Sets to (x,y,z)
     */
    template<typename RhsT>
     * @param nx initial x-coordinate value
    inline Aabb2<T> constexpr operator&(Aabb2<RhsT> const &rhs) const {
     * @param ny initial y-coordinate value
      return intersection(rhs);
     * @param nz initial z-coordinate value
    }
     */

    inline void constexpr assign(T nx = 0, T ny = 0, T nz = 0) __attribute__((__always_inline__)) {
    /**
      x = nx;
     * Outputs string representation of bounding-box @a rhs to output stream @a lhs
      y = ny;
     * @param lhs Output stream to write to
      z = nz;
     * @param rhs Bounding-box to write to output stream.
    }
     * @return Reference to output stream @a lhs

     */
    //----------------[ access operators ]-------------------
    inline friend std::ostream &operator<<(std::ostream &lhs, const Aabb2<T> &rhs) __attribute__((__always_inline__)) {
    /**
      lhs << rhs.min << " x " << rhs.max;
     * Copy operator
      return lhs;
    }
     * @param rhs Right hand side argument of binary operator.
};
     */

    inline Vector3<T> constexpr operator=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
/**
      x = rhs.x;
 * Three-dimensional axis-aligned bounding-box (aka AABB) class.
      y = rhs.y;
 *
      z = rhs.z;
 * This class provides functionality for:
      return *this;
 * - creating an AABB from a point, or another AABB,
    }
 * - testing if a point of other AABB intersects with it,

 * - getting result of intersection with other AABB,
    /**
 * - transforming AABB with 4x4 matrix.
     * Copy casting operator.
 *
     * @param rhs Right hand side argument of binary operator.
 * There are also overloaded couple of operators to shorten common operations.
     */
 * For instance you can use  @c operator<< on AABB to extend it with a passed point or other AABB.
    template<typename FromT>
 * @code
    inline Vector3<T> constexpr operator=(Vector3<FromT> const &rhs) {
 * Aabb3f aabb;
      x = static_cast<T>(rhs.x);
 * aabb << Vector3f(1, 1, 2) << Aabb3f(-3,-3,-3, 2, 2, 2);
      y = static_cast<T>(rhs.y);
 * @endcode
 */
      z = static_cast<T>(rhs.z);
template<typename T>
      return *this;
class Aabb3 {
    }
  public:

    /**
    /**
     * Position of Min corner of bounding box.
     * Copy operator to Vector3
     */
     * @param rhs Right hand side argument of binary operator.
    Vector3<T> min;
     */

    inline Vector3<T> constexpr operator=(Vector4<T> const &rhs) __attribute__((__always_inline__)) {
    /**
      x = rhs.x;
     * Position of Max corner of bounding box
      y = rhs.y;
     */
      z = rhs.z;
    Vector3<T> max;

      return *this;
    /**
    }
     * Constructs invalid axes-aligned bounding-box.

     * @see valid() for explanation of invalid bounding-box usage.
    /**
     */
     * Copy casting operator to Vector3
    inline constexpr Aabb3() __attribute__((__always_inline__))
     * @param rhs Right hand side argument of binary operator.
      : min(1, 1, 1), max(-1, -1, -1) {
     */
    }
    template<typename FromT>

    inline Vector3<T> constexpr operator=(Vector4<FromT> const &rhs) {
    /**
      x = static_cast<T>(rhs.x);
     * Constructs axes-aligned bound-box containing one point @a point
      y = static_cast<T>(rhs.y);
     * @param point
      z = static_cast<T>(rhs.z);
     */
      return *this;
    template<typename SrcT>
    }
    inline constexpr Aabb3(Vector3<SrcT> const &point)

      : min(point), max(point) {
    }
    /**

     * Array access operator
    /**
     * @param n Array index
     * Constructs axes-aligned bounding-box from two corner points (@a x0, @a y0, @a z0)
     * @return For n = 0, reference to x coordinate, n = 1
     * and (@a x1, @a y1, @a z1)
     * reference to y, else reference to z
     * @param x0 X-coordinate of first point
     * y coordinate.
     * @param y0 Y-coordinate of first point
     */
     * @param z0 Z-coordinate of first point
    inline T constexpr &operator[](int n) __attribute__((__always_inline__)) {
     * @param x1 X-coordinate of second point
      return n == 0 ? x : (n == 1 ? y : z);
     * @param y1 Y-coordinate of second point
    }
     * @param z1 Z-coordinate of second point

     */
    /**
    template<typename SrcT>
     * Constant array access operator
    inline constexpr Aabb3(SrcT x0, SrcT y0, SrcT z0, SrcT x1, SrcT y1, SrcT z1)
     * @param n Array index
      : min(std::min(x0, x1), std::min(y0, y1), std::min(z0, z1)),
     * @return For n = 0, reference to x coordinate, n = 1
        max(std::max(x0, x1), std::max(y0, y1), std::max(z0, z1)) {
    }
     * reference to y, else reference to z

     * y coordinate.
    /**
     */
     * Constructs axes-aligned bounding-box from @a min and @b max
    inline T constexpr const &operator[](int n) const __attribute__((__always_inline__)) {
     * @param min X-coordinate of first point
      return n == 0 ? x : (n == 1 ? y : z);
     * @param max X-coordinate of second point
    }
     */

    template<typename SrcT>
    //---------------[ vector arithmetic operator ]--------------
    inline constexpr Aabb3(Vector3<SrcT> const &new_min, Vector3<SrcT> const &new_max)
    /**
      : min(new_min),
     * Addition operator
        max(new_max) {
     * @param rhs Right hand side argument of binary operator.
    }
     */

    /**
    inline Vector3<T> constexpr operator+(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
     * Constructs axes-aligned bounding-box containing point (@a x, @a y, @a z)
      return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
     * @param x X-coordinate of point
    }
     * @param y Y-coordinate of point

     * @param z Z-coordinate of point
    /**
     */
     * Subtraction operator
    template<typename SrcT>
     * @param rhs Right hand side argument of binary operator.
    inline constexpr Aabb3(SrcT x, SrcT y, SrcT z)
     */
      : min(x, y, z), max(x, y, z) {
    inline Vector3<T> constexpr operator-(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
    }
      return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);

    }
    /**

     * Creates copy of axis-aligned bounding-box
    /**
     * @param src Source bounding-box
     * Multiplication operator
     */
    template<typename SrcT>
     * @param rhs Right hand side argument of binary operator.
    inline constexpr Aabb3(Aabb3<SrcT> const &src)
     */
      : min(src.min), max(src.max) {
    inline Vector3<T> constexpr operator*(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
    }
      return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);

    }
    /**

     * Assign operator
    /**
     * @param rhs source bounding-box
     * Division operator
     * @return refenrence to this
     * @param rhs Right hand side argument of binary operator.
     */
     */
    template<typename SrcT>
    inline Vector3<T> constexpr operator/(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
    inline Aabb3<T> constexpr &operator=(Aabb3<SrcT> const &rhs) {
      min = rhs.min;
      return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
      max = rhs.max;
    }
      return *this;

    }
    /**

     * Addition operator
    /**
     * @param rhs Right hand side argument of binary operator.
     * Checks if bounding-box is valid. Valid bounding-box has non-negative size.
     */
     * If an invalid bounding-box is extended by point or another bounding-box, the target
    inline Vector3<T> constexpr &operator+=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
     * bounding box becomes valid and contains solely the source point or bounding-box respectively.
      x += rhs.x;
     * @return True if box is valid, otherwise false
      y += rhs.y;
     */
      z += rhs.z;
    inline bool constexpr valid() const __attribute__((__always_inline__)) {
      return *this;
      return min.x <= max.x && min.y <= max.y && min.z <= max.z;
    }
    }


    /**
    /**
     * Subtraction operator
     * Makes this bounding-box invalid. So calling valid() gets false.
     * @param rhs Right hand side argument of binary operator.
     * @see valid() method for more info on usage of invalid bounding-boxes.
     */
     */
    inline void constexpr invalidate() __attribute__((__always_inline__)) {
    inline Vector3<T> constexpr &operator-=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
      min = Vector3<T>(1, 1, 1);
      x -= rhs.x;
      max = Vector3<T>(-1, -1, -1);
      y -= rhs.y;
    }
      z -= rhs.z;

      return *this;
    /**
    }
     * Extends this bounding-box by a point @a point.

     * @param point A point to extend bounding-box by.
    /**
     */
     * Multiplication operator
    template<typename SrcT>
     * @param rhs Right hand side argument of binary operator.
    inline void constexpr extend(Vector3<SrcT> const &point) {
     */
      if(!valid()) {
    inline Vector3<T> constexpr &operator*=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
        min = max = point;
      x *= rhs.x;
      } else {
      y *= rhs.y;
        min = std::min(min, point);
      z *= rhs.z;
        max = std::max(max, point);
      return *this;
      }
    }
    }


    /**
    /**
     * Division operator
     * Extends this bounding-box by a box @a box.
     * @param rhs Right hand side argument of binary operator.
     * @param box A box to extend this bounding-box by.
     */
     */
    template<typename SrcT>
    inline Vector3<T> constexpr &operator/=(Vector3<T> const &rhs) __attribute__((__always_inline__)) {
    inline void constexpr extend(Aabb3<SrcT> const &box) {
      x /= rhs.x;
      if(!valid()) {
      y /= rhs.y;
        min = box.min;
      z /= rhs.z;
        max = box.max;
      return *this;
      } else {
    }
        min = std::min(min, box.min);

        max = std::max(max, box.max);
    /**
      }
     * Addition operator with a 2-vector
    }
     * @param rhs Right hand side argument of binary operator.

     */
    /**
    inline Vector3<T> constexpr operator+(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
     * Gets a copy of this bounding-box extend by a point @a point.
      return Vector3<T>(x + rhs.x, y + rhs.y, z);
     * @param point A point to extend the box by
    }
     * @return Copy of extended bounding-box

     */
    /**
    template<typename SrcT>
     * Subtraction operator with a 2-vector
    inline constexpr Aabb3<T> extended(Vector3<SrcT> const &point) const {
      Aabb3<T> ret(*this);
     * @param rhs Right hand side argument of binary operator.
      ret.extend(point);
     */
      return ret;
    inline Vector3<T> constexpr operator-(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
    }
      return Vector3<T>(x - rhs.x, y - rhs.y, z);

    }
    /**

     * Gets a copy of this bounding-box extnended by box @a box.
    /**
     * @param box A box to extend the copy be.
     * Multiplication operator with a 2-vector
     * @return Copy of extended bounding-box
     * @param rhs Right hand side argument of binary operator.
     */
     */
    template<typename SrcT>
    inline Vector3<T> constexpr operator*(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
    inline constexpr Aabb3<T> extended(Aabb3<SrcT> const &box) const {
      Aabb3<T> ret(*this);
      return Vector3<T>(x * rhs.x, y * rhs.y, z);
      ret.extend(box);
    }
      return *this;

    }
    /**

     * Division operator with a 2-vector
    /**
     * @param rhs Right hand side argument of binary operator.
     * Tests if the point @a point is within this bounding-box
     */
     * @param point A point to be tested
    inline Vector3<T> constexpr operator/(Vector2<T> const &rhs) const __attribute__((__always_inline__)) {
     * @return True if point @a point lies within bounding-box, otherwise false.
      return Vector3<T>(x / rhs.x, y / rhs.y, z);
     */
    }
    template<typename SrcT>

    inline bool constexpr intersects(Vector3<SrcT> const &point) const {
    /**
      return min.x <= point.x && point.x <= max.x &&
     * Addition operator with a 2-vector
             min.y <= point.y && point.y <= max.y &&
     * @param rhs Right hand side argument of binary operator.
             min.z <= point.z && point.z <= max.z;
     */
    }
    inline Vector3<T> constexpr &operator+=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {

      x += rhs.x;
    /**
      y += rhs.y;
     * Tests if other bounding-box @a box intersects (even partially) with this bounding-box.
      return *this;
     * @param box A box to be tested for intersection.
    }
     * @return True if there's intersection between boxes, otherwise false.

     */
    /**
    template<typename SrcT>
     * Subtraction operator with a 2-vector
    inline bool constexpr intersects(Aabb3<SrcT> const &box) const {
     * @param rhs Right hand side argument of binary operator.
      return max.x >= box.min.x && min.x <= box.max.x &&
     */
             max.y >= box.min.y && min.y <= box.max.y &&
             max.z >= box.min.z && min.z <= box.max.z;
    inline Vector3<T> constexpr &operator-=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
    }
      x -= rhs.x;

      y -= rhs.y;
    /**
      return *this;
     * Gets result of intersection of this bounding-box with @a other bounding-box.
    }
     * In case the boxes don't intersect, the returned bounding-box is invalid.

     * @param other Box to be tested
    /**
     * @return Result of intersection.
     * Multiplication operator with a 2-vector
     * @see valid() method for more information on invalid bounding-boxes.
     * @param rhs Right hand side argument of binary operator.
     */
     */
    template<typename SrcT>
    inline Vector3<T> constexpr &operator*=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
    inline constexpr Aabb3<T> intersection(Aabb3<SrcT> const &other) const {
      x *= rhs.x;
      return (max.x < other.min.x || min.x > other.max.x ||
      y *= rhs.y;
              max.y < other.min.y || min.y > other.max.y ||
      return *this;
              max.z < other.min.z || min.z > other.max.z) ? Aabb3<T>() : Aabb3<T>(std::max(min, other.min), std::min(max, other.max));
    }
    }


    /**
    /**
     * Division operator with a 2-vector
     * Tests if a ray @b from origin @a intersects with this bounding-box.
     * @param rhs Right hand side argument of binary operator.
     * @param ray The intersecting ray; does not need to be normalised
     */
     * @param origin Origin of the intersecting ray
    inline Vector3<T> constexpr &operator/=(Vector2<T> const &rhs) __attribute__((__always_inline__)) {
     * @return True if the ray intersects the box, otherwise false.
      x /= rhs.x;
     */
      y /= rhs.y;
    template<typename SrcT>
      return *this;
    inline bool constexpr ray_intersects(Vector3<SrcT> const &ray, Vector3<SrcT> const &origin = Vector3<SrcT>()) const {
    }
      // adapted from http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/

      /*
    /**
      SrcT const dist_min_x = (min.x - origin.x) / ray.x;
     * Dot product of two vectors.
      SrcT const dist_max_x = (max.x - origin.x) / ray.x;
      SrcT dist_min = std::min(dist_min_x, dist_max_x);
     * @param rhs Right hand side argument of binary operator.
      SrcT dist_max = std::max(dist_min_x, dist_max_x);
     */

    inline T constexpr dotProduct(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      SrcT const dist_min_y = (min.y - origin.y) / ray.y;
      return x * rhs.x + y * rhs.y + z * rhs.z;
      SrcT const dist_max_y = (max.y - origin.y) / ray.y;
    }
      dist_min = std::max(dist_min, std::min(dist_min_y, dist_max_y));

      dist_max = std::min(dist_max, std::max(dist_min_y, dist_max_y));
    /**

     * Cross product of two vectors
      SrcT const dist_min_z = (min.z - origin.z) / ray.z;
     * @param rhs Right hand side argument of binary operator.
      SrcT const dist_max_z = (max.z - origin.z) / ray.z;
     */
      dist_min = std::max(dist_min, std::min(dist_min_z, dist_max_z));
    inline Vector3<T> constexpr crossProduct(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      dist_max = std::min(dist_max, std::max(dist_min_z, dist_max_z));

      return Vector3<T>(y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
      return dist_max >= std::max(dist_min, 0.0f);
    }
      */

      // constexpr-compatible reformulation:
    //--------------[ rotation with quaternions ]-----------------
      return std::min(std::min(std::max((min.x - origin.x) / ray.x,
    // see http://mollyrocket.com/forums/viewtopic.php?t=833&sid=3a84e00a70ccb046cfc87ac39881a3d0
                                        (max.x - origin.x) / ray.x),
    /**
                               std::max((min.y - origin.y) / ray.y,
     * Multiplication by quaternion operator (rotation by quaternion)
                                        (max.y - origin.y) / ray.y)),
     * @param rhs Right hand side argument of binary operator.
                      std::max((min.z - origin.z) / ray.z,
     */
                               (max.z - origin.z) / ray.z)) >=
    inline Vector3<T> constexpr operator*(Quaternion<T> const &rhs) const __attribute__((__always_inline__)) {
             std::max(std::max(std::max(std::min((min.x - origin.x) / ray.x,
      return *this + (rhs.v.crossProduct(*this) * static_cast<T>(2) * rhs.w) + rhs.v.crossProduct(rhs.v.crossProduct(*this) * static_cast<T>(2));
                                                 (max.x - origin.x) / ray.x),
    }
                                        std::min((min.y - origin.y) / ray.y,

                                                 (max.y - origin.y) / ray.y)),
    /**
                               std::min((min.z - origin.z) / ray.z,
     * Multiplication by quaternion operator (rotation by quaternion)
                                        (max.z - origin.z) / ray.z)), 0.0f);
     * @param rhs Right hand side argument of binary operator.
    }
     */

    /**
    inline Vector3<T> constexpr &operator*=(Quaternion<T> const &rhs) __attribute__((__always_inline__)) {
     * Gets center point of bounding-box.
      Vector3<T> const temp = rhs.v.crossProduct(*this) * static_cast<T>(2);
     * @return A center point of bounding-box.
      *this += (temp * rhs.w) + rhs.v.crossProduct(temp);
     */
      return *this;
    inline Vector3<T> constexpr center() const __attribute__((__always_inline__)) {
    }
      return (min + max) * static_cast<T>(0.5);

    }
    //--------------[ scalar vector operator ]--------------------

    /**
    /**
     * Addition operator
     * Gets extent of bounding-box.
     * @param rhs Right hand side argument of binary operator.
     * @return Extent of bounding-box.
     */
     */
    inline Vector3<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
    inline Vector3<T> constexpr extent() const __attribute__((__always_inline__)) {
      return Vector3<T>(x + rhs, y + rhs, z + rhs);
      return (max - min) * static_cast<T>(0.5);
    }
    }


    /**
    /**
     * Gets diagonal size of bounding-box
     * Subtraction operator
     * @return Sizes for particular dimensions.
     * @param rhs Right hand side argument of binary operator.
     */
     */
    inline Vector3<T> constexpr size() const __attribute__((__always_inline__)) {
    inline Vector3<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return max - min;
      return Vector3<T>(x - rhs, y - rhs, z - rhs);
    }
    }


    /**
    /**
     * Gets all 8 corner-points of bounding box
     * Multiplication operator
     * @param i An index of bounding-box corner point. Valid values are 0 .. 7.
     * @param rhs Right hand side argument of binary operator.
     * @return A position of @a i-th corner-point.
     */
     * @note The order of points is as follows (where @c + denotes max-point and @c - min-point):
    inline Vector3<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
     * 1. (@c + @c + @c +)
      return Vector3<T>(x * rhs, y * rhs, z * rhs);
     * 2. (@c - @c + @c +)
    }
     * 3. (@c + @c - @c +)

     * 4. (@c - @c - @c +)
    /**
     * 5. (@c + @c + @c -)
     * Division operator
     * 6. (@c - @c + @c -)
     * @param rhs Right hand side argument of binary operator.
     * 7. (@c + @c - @c -)
     */
     * 8. (@c - @c - @c -)
     *
    inline Vector3<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
     */
      return Vector3<T>(x / rhs, y / rhs, z / rhs);
    inline Vector3<T> constexpr point(unsigned int i) const __attribute__((__always_inline__)) {
    }
      return Vector3<T>(i & 1 ? min.x : max.x, i & 2 ? min.y : max.y, i & 4 ? min.z : max.z);

    }
    /**

     * Addition operator
    /**
     * @param rhs Right hand side argument of binary operator.
     * Gets transformed bounding-box by transform @a t
     */
     * @param t A transform matrix
    inline Vector3<T> constexpr &operator+=(T rhs) __attribute__((__always_inline__)) {
     * @return Transformed bounding-box
      x += rhs;
     */
      y += rhs;
    inline Aabb3<T> constexpr transformed(Matrix4<T> const &t) const __attribute__((__always_inline__)) {
      z += rhs;
      Aabb3<T> ret;
      return *this;
      for(unsigned int i = 0; i != 8; ++i) {
    }
        const Vector4<T> p(point(i), 1);

        ret.extend((t * p).xyz());
    /**
      }
     * Subtraction operator
      return ret;
    }
     * @param rhs Right hand side argument of binary operator.

     */
    //-------------------------------------------------------------------------------------------------------------
    inline Vector3<T> constexpr &operator-=(T rhs) __attribute__((__always_inline__)) {
    // operators
      x -= rhs;
    //-------------------------------------------------------------------------------------------------------------
      y -= rhs;
    /**
      z -= rhs;
     * Tests if @a rhs is equal to this bounding-box
      return *this;
     * @param rhs Right-hand side
    }
     * @return True if @a rhs and this bounding-boxes are equal, otherwise false

     */
    /**
    template<typename RhsT>
     * Multiplication operator
    inline bool constexpr operator==(Aabb3<RhsT> const &rhs) const {
     * @param rhs Right hand side argument of binary operator.
      return min == rhs.min && max == rhs.max;
     */
    }
    inline Vector3<T> constexpr &operator*=(T rhs) __attribute__((__always_inline__)) {

      x *= rhs;
    /**
      y *= rhs;
     * Tests if @a rhs is not equal to this bounding-box
      z *= rhs;
     * @param rhs Right-hand side
      return *this;
     * @return True if @a rhs and this bounding-boxes are not equal, otherwise false
    }
     */

    template<typename RhsT>
    /**
    inline bool constexpr operator!=(Aabb3<RhsT> const &rhs) const {
     * Division operator
      return min != rhs.min || max != rhs.max;
    }
     * @param rhs Right hand side argument of binary operator.

     */
    /**
    inline Vector3<T> constexpr &operator/=(T rhs) __attribute__((__always_inline__)) {
     * Gets transformed bounding-box by transform @a rhs.
      x /= rhs;
     * @param rhs Matrix 4x4 representing the transform
      y /= rhs;
     * @return Transformed bounding-box
      z /= rhs;
     */
      return *this;
    inline Aabb3<T> constexpr operator*(Matrix4<T> const &rhs) const __attribute__((__always_inline__)) {
    }
      return transformed(rhs);

    }
    //-------------[ matrix operations ]-------------------------


    /**
    /**
     * Apply transform @a rhs to this bounding-box
     * Multiplication by 3x3 matrix operator
     * @param rhs A transform to be applied
     * @param rhs Right hand side argument of binary operator.
     * @return Reference to this
     */
     */
    inline Vector3<T> constexpr &operator*=(Matrix3<T> rhs) __attribute__((__always_inline__)) {
    inline Aabb3<T> constexpr &operator*=(Matrix4<T> const &rhs) __attribute__((__always_inline__)) {
      *this = rhs * *this;
      *this = transformed(rhs);
      return *this;
      return *this;
    }
    }


    /**
    /**
     * Multiplication by 4x4 matrix operator
     * Extends this bounding-box by point @a rhs.
     * @param rhs Right hand side argument of binary operator.
     * @param rhs A point to extend this bounding-box by
     */
     * @return Reference to this
     */
    inline Vector3<T> constexpr &operator*=(Matrix4<T> rhs) __attribute__((__always_inline__)) {
    template<typename SrcT>
      *this = rhs * *this;
    inline Aabb3<T> constexpr &operator<<(Vector3<SrcT> const &rhs) {
      return *this;
      extend(rhs);
    }
      return *this;

    }
    //-------------[ equality operator ]-------------------------

    /**
    /**
     * Equality test operator
     * Extends this bounding-box by box @a rhs.
     * @param rhs Right hand side argument of binary operator.
     * @param rhs A box to extend this bounding-box by
     * @note Test of equality is based of threshold epsilon value. To be two
     * @return Reference to this
     * values equal, must satisfy this condition | lhs.x - rhs.y | < epsilon,
     */
     * same for y-coordinate, and z-coordinate.
    template<typename SrcT>
     */
    inline Aabb3<T> constexpr &operator<<(Aabb3<SrcT> const &rhs) {
      extend(rhs);
    inline bool constexpr operator==(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
      return *this;
      #ifdef VMATH_SOFT_COMPARE
    }
        return std::abs(x - rhs.x) < static_cast<T>(epsilon) &&

               std::abs(y - rhs.y) < static_cast<T>(epsilon) &&
    /**
               std::abs(z - rhs.z) < static_cast<T>(epsilon);
     * Union of this and @a rhs bounding-boxes
      #else
     * @param rhs Right-hand side of union
        return x == rhs.x &&
     * @return A resulting bounding-box representing union
               y == rhs.y &&
     */
               z == rhs.z;
    template<typename RhsT>
      #endif // VMATH_SOFT_COMPARE
    inline Aabb3<T> constexpr operator|(Aabb3<RhsT> const &rhs) const {
    }
      return extended(rhs);

    }
    /**

    /**
     * Inequality test operator
     * Intersection of this and @a rhs bounding-boxed
     * @param rhs Right hand side argument of binary operator.
     * @param rhs Right-hand side
     * @return not (lhs == rhs) :-P
     * @return Resulting bounding-box representing the intersection.
     */
     */
    inline bool constexpr operator!=(Vector3<T> const &rhs) const __attribute__((__always_inline__)) {
    template<typename RhsT>
      return !(*this == rhs);
    inline Aabb3<T> constexpr operator&(Aabb3<RhsT> const &rhs) const {
    }
      return intersection(rhs);

    }
    //-------------[ unary operations ]--------------------------

    /**
    /**
     * Unary negate operator
     * Outputs string representation of bounding-box @a rhs to output stream @a lhs
     * @return negated vector
     * @param lhs Output stream to write to
     */
     * @param rhs Bounding-box to write to output stream.
    inline Vector3<T> constexpr operator-() const __attribute__((__always_inline__)) {
     * @return Reference to output stream @a lhs
     */
      return Vector3<T>(-x, -y, -z);
    inline friend std::ostream &operator<<(std::ostream &lhs, const Aabb3<T> &rhs) __attribute__((__always_inline__)) {
    }
      lhs << rhs.min << " x " << rhs.max;

      return lhs;
    //-------------[ size operations ]---------------------------
    }
    /**
};
     * Return square of length.

     * @return length ^ 2
/// 2D axis-aligned bounding box of floats
     * @note This method is faster then length(). For comparison
using Aabb2f = Aabb2<float>;
     * of length of two vector can be used just this value, instead
/// 2D axis-aligned bounding box of doubles
     * of more expensive length() method.
using Aabb2d = Aabb2<double>;
     */
/// 2D axis-aligned bounding box of long doubles
using Aabb2ld = Aabb2<long double>;
    inline T constexpr lengthSq() const __attribute__((__always_inline__)) {
/// 2D axis-aligned bounding box of integers
      return x * x + y * y + z * z;
using Aabb2i = Aabb2<int>;
    }


/// 3D axis-aligned bounding box of floats
    /**
using Aabb3f = Aabb3<float>;
     * Get length of vector.
/// 3D axis-aligned bounding box of doubles
     * @return length of vector
using Aabb3d = Aabb3<double>;
     */
/// 3D axis-aligned bounding box of long doubles
    inline T constexpr length() const __attribute__((__always_inline__)) {
using Aabb3ld = Aabb3<long double>;
      return static_cast<T>(std::sqrt(lengthSq()));
/// 2D axis-aligned bounding box of integers
    }
using Aabb3i = Aabb3<int>;
    /**

     * Get length of vector, fast approximation.
#ifdef VMATH_NAMESPACE
     * @return length of vector
}
     */
#endif //VMATH_NAMESPACE

    inline T constexpr length_fast() const __attribute__((__always_inline__)) {
#endif // __vmath_Header_File__
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
    }
    inline Vector4<T> constexpr normalise_copy() const __attribute__((__always_inline__)) {
      T const temp(length());
      return Vector4<T>(x / temp, y / temp, z / temp, w / temp);
    }
    inline Vector4<T> constexpr normalise_copy_fast() const __attribute__((__always_inline__)) {
      T const temp(length_fast());
      return Vector4<T>(x / temp, y / temp, z / temp, w / temp);
    }
    inline Vector4<T> constexpr normalise_copy_faster() const __attribute__((__always_inline__)) {
      T const temp(length_faster());
      return Vector4<T>(x / temp, y / temp, z / temp, w / temp);
    }
    inline Vector4<T> constexpr normalize_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      return normalise_copy();
    }
    inline Vector4<T> constexpr normalize_copy_fast() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      return normalise_copy_fast();
    }
    inline Vector4<T> constexpr normalize_copy_faster() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
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
        w = static_cast<T>(0);
      } else {
        x /= temp;
        y /= temp;
        z /= temp;
        w /= temp;
      }
    }
    inline void constexpr normalize_safe() __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      normalise_safe();
    }
    inline Vector4<T> constexpr normalise_safe_copy() const __attribute__((__always_inline__)) {
      T const temp(length());
      if(temp == static_cast<T>(0)) {
        return Vector4<T>();
      } else {
        return Vector4<T>(x / temp, y / temp, z / temp, w / temp);
      }
    }
    inline Vector4<T> constexpr normalize_safe_copy() const __attribute__((__always_inline__)) __attribute__((__deprecated__("Proper English, please!"))) {
      return normalise_safe_copy();
    }

    /**
     * Absolute vector (make all values positive)
     */
    inline void constexpr abs() __attribute__((__always_inline__)) {
      x = std::abs(x);
      y = std::abs(y);
      z = std::abs(z);
      w = std::abs(w);
    }
    inline Vector4<T> constexpr abs_copy() const __attribute__((__always_inline__)) {
      return Vector4<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
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
    inline Vector4<T> constexpr lerp(T fact, Vector4<T> const &new_r) const __attribute__((__always_inline__)) {
      return (*this) + (new_r - (*this)) * fact;
    }

    //-------------[ conversion ]-----------------------------

    /**
     * Conversion to pointer operator
     * @return Pointer to internally stored (in management of class Vector4<T>)
     * used for passing Vector4<T> values to gl*4[fd] functions.
     */
    inline constexpr operator T*() __attribute__((__always_inline__)) {
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
    inline Vector3<T> constexpr xyz() const __attribute__((__always_inline__)) {
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
template<typename T>
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
    template<typename FromT>
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
    inline void constexpr identity() __attribute__((__always_inline__)) {
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
      //T sin_xRads, cos_xRads, sin_yRads, cos_yRads, sin_zRads, cos_zRads;
      // zero-initialisation is required for gcc not to complain when the function is constexpr
      // the static cast is to avoid narrowing conversion warnings when used with ints
      T sin_xRads = static_cast<T>(0);
      T cos_xRads = static_cast<T>(0);
      T sin_yRads = static_cast<T>(0);
      T cos_yRads = static_cast<T>(0);
      T sin_zRads = static_cast<T>(0);
      T cos_zRads = static_cast<T>(0);
      sincos_any(xRads, sin_xRads, cos_xRads);
      sincos_any(yRads, sin_yRads, cos_yRads);
      sincos_any(zRads, sin_zRads, cos_zRads);
      T const temp_sin_z_sin_y = sin_zRads * sin_yRads;
      T const temp_cos_z_sin_y = cos_zRads * sin_yRads;
      T const temp_cos_z_cos_y = cos_zRads * cos_yRads;
      return Matrix3<T>(temp_cos_z_cos_y,
                        temp_cos_z_sin_y * sin_xRads - sin_zRads * cos_xRads,
                        temp_cos_z_sin_y * cos_xRads + sin_zRads * sin_xRads,

                        sin_zRads * cos_yRads,
                        temp_sin_z_sin_y * sin_xRads + cos_zRads * cos_xRads,
                        temp_sin_z_sin_y * cos_xRads - cos_zRads * sin_xRads,

                       -temp_cos_z_sin_y,
                        temp_cos_z_cos_y * sin_xRads,
                        temp_cos_z_cos_y * cos_xRads);
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
      // zero-initialisation is required for gcc not to complain when the function is constexpr
      // the static cast is to avoid narrowing conversion warnings when used with ints
      T sin_a = static_cast<T>(0);
      T cos_a = static_cast<T>(0);
      sincos_any(angle, sin_a, cos_a);
      T const cos_a_inv = static_cast<T>(1) - cos_a;
      return Matrix3<T>(axis.x * axis.x * cos_a_inv + cos_a,
                        axis.y * axis.x * cos_a_inv - sin_a * axis.z,
                        axis.z * axis.x * cos_a_inv + sin_a * axis.y,

                        axis.x * axis.y * cos_a_inv + sin_a * axis.z,
                        axis.y * axis.y * cos_a_inv + cos_a,
                        axis.z * axis.y * cos_a_inv - sin_a * axis.x,

                        axis.x * axis.z * cos_a_inv - sin_a * axis.y,
                        axis.y * axis.z * cos_a_inv + sin_a * axis.x,
                        axis.z * axis.z * cos_a_inv + cos_a);
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
      Vector3<T> const cross(to.crossProduct(from));
      T const dot = from.dotProduct(to);
      T const temp_k = static_cast<T>(1) / (static_cast<T>(1) + dot);
      return Matrix3<T>(cross.x * cross.x * temp_k + dot,
                        cross.y * cross.x * temp_k - cross.z,
                        cross.z * cross.x * temp_k + cross.y,

                        cross.x * cross.y * temp_k + cross.z,
                        cross.y * cross.y * temp_k + dot,
                        cross.z * cross.y * temp_k - cross.x,

                        cross.x * cross.z * temp_k - cross.y,
                        cross.y * cross.z * temp_k + cross.x,
                        cross.z * cross.z * temp_k + dot);

    }

    /**
     * Creates rotation matrix from ODE Matrix.
     */
    template<typename It>
    inline static Matrix3<T> constexpr fromOde(It const *mat) {
      return Matrix3<T>(static_cast<T>(mat[0]), static_cast<T>(mat[4]), static_cast<T>(mat[8]),
                        static_cast<T>(mat[1]), static_cast<T>(mat[5]), static_cast<T>(mat[9]),
                        static_cast<T>(mat[2]), static_cast<T>(mat[6]), static_cast<T>(mat[10]));
    }

    /**
     * Creates new matrix 3x3 from array that represents such matrix 3x3
     * as array of tightly packed elements in row major order.
     * @param arr An array of elements for 3x3 matrix in row major order.
     * @return An instance of Matrix3<T> representing @a arr
     */
    template<typename FromT>
    inline static Matrix3<T> constexpr fromRowMajorArray(FromT const *arr) {
      return Matrix3<T>(static_cast<T>(arr[0]), static_cast<T>(arr[3]), static_cast<T>(arr[6]),
                        static_cast<T>(arr[1]), static_cast<T>(arr[4]), static_cast<T>(arr[7]),
                        static_cast<T>(arr[2]), static_cast<T>(arr[5]), static_cast<T>(arr[8]));
    }

    /**
     * Creates new matrix 3x3 from array that represents such matrix 3x3
     * as array of tightly packed elements in column major order.
     * @param arr An array of elements for 3x3 matrix in column major order.
     * @return An instance of Matrix3<T> representing @a arr
     */
    template<typename FromT>
    inline static Matrix3<T> constexpr fromColumnMajorArray(FromT const *arr) {
      return Matrix3<T>(static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]),
                        static_cast<T>(arr[3]), static_cast<T>(arr[4]), static_cast<T>(arr[5]),
                        static_cast<T>(arr[6]), static_cast<T>(arr[7]), static_cast<T>(arr[8]));
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
        return std::abs(data[0] - rhs.data[0]) < static_cast<T>(epsilon) &&
               std::abs(data[1] - rhs.data[1]) < static_cast<T>(epsilon) &&
               std::abs(data[2] - rhs.data[2]) < static_cast<T>(epsilon) &&
               std::abs(data[3] - rhs.data[3]) < static_cast<T>(epsilon) &&
               std::abs(data[4] - rhs.data[4]) < static_cast<T>(epsilon) &&
               std::abs(data[5] - rhs.data[5]) < static_cast<T>(epsilon) &&
               std::abs(data[6] - rhs.data[6]) < static_cast<T>(epsilon) &&
               std::abs(data[7] - rhs.data[7]) < static_cast<T>(epsilon) &&
               std::abs(data[8] - rhs.data[8]) < static_cast<T>(epsilon);
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
    inline T constexpr &at(int x, int y) __attribute__((__always_inline__)) {
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
    inline T constexpr &operator()(int i, int j) __attribute__((__always_inline__)) {
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
     * Returns transform (4x4) matrix including this as the rotationc component.
     */
    inline Matrix4<T> constexpr getTranslation() const __attribute__((__always_inline__)) {
      return Matrix4<T>(data[0],           data[1],           data[2],           static_cast<T>(0),
                        data[3],           data[4],           data[5],           static_cast<T>(0),
                        data[6],           data[7],           data[8],           static_cast<T>(0),
                        static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr &operator=(Matrix3<T> const &rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs.data.data(), sizeof(T) * 9);
      return *this;
    }

    /**
     * Copy casting operator
     * @param rhs Right hand side argument of binary operator.
     */
    template<typename FromT>
    inline Matrix3<T> constexpr &operator=(Matrix3<FromT> const &rhs) {
      data[0] = static_cast<T>(rhs.data[0]);
      data[1] = static_cast<T>(rhs.data[1]);
      data[2] = static_cast<T>(rhs.data[2]);
      data[3] = static_cast<T>(rhs.data[3]);
      data[4] = static_cast<T>(rhs.data[4]);
      data[5] = static_cast<T>(rhs.data[5]);
      data[6] = static_cast<T>(rhs.data[6]);
      data[7] = static_cast<T>(rhs.data[7]);
      data[8] = static_cast<T>(rhs.data[8]);
      return *this;
    }

    /**
     * Copy operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr &operator=(T const *rhs) __attribute__((__always_inline__)) {
      std::memcpy(data.data(), rhs, sizeof(T) * 9);
      return *this;
    }

    /*
    inline Matrix3<T> constexpr &operator=(T const *rhs) __attribute__((__always_inline__)) {
      for(int i = 0; i != 9; ++i) {
        data[i] = (T)rhs[i];
      }
      return *this;
    }
    */

    //--------------------[ matrix with matrix operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator+(Matrix3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>(data[0] + rhs.data[0], data[1] + rhs.data[1], data[2] + rhs.data[2],
                        data[3] + rhs.data[3], data[4] + rhs.data[4], data[5] + rhs.data[5],
                        data[6] + rhs.data[6], data[7] + rhs.data[7], data[8] + rhs.data[8]);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator-(Matrix3<T> const &rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>(data[0] - rhs.data[0], data[1] - rhs.data[1], data[2] - rhs.data[2],
                        data[3] - rhs.data[3], data[4] - rhs.data[4], data[5] - rhs.data[5],
                        data[6] - rhs.data[6], data[7] - rhs.data[7], data[8] - rhs.data[8]);
    }

    //--------------------[ matrix with scalar operations ]---------------------
    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator+(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>(data[0] + rhs, data[1] + rhs, data[2] + rhs,
                        data[3] + rhs, data[4] + rhs, data[5] + rhs,
                        data[6] + rhs, data[7] + rhs, data[8] + rhs);
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator-(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>(data[0] - rhs, data[1] - rhs, data[2] - rhs,
                        data[3] - rhs, data[4] - rhs, data[5] - rhs,
                        data[6] - rhs, data[7] - rhs, data[8] - rhs);

    }

    /**
     * Multiplication operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator*(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>(data[0] * rhs, data[1] * rhs, data[2] * rhs,
                        data[3] * rhs, data[4] * rhs, data[5] * rhs,
                        data[6] * rhs, data[7] * rhs, data[8] * rhs);
    }

    /**
     * Division operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr operator/(T rhs) const __attribute__((__always_inline__)) {
      return Matrix3<T>(data[0] / rhs, data[1] / rhs, data[2] / rhs,
                        data[3] / rhs, data[4] / rhs, data[5] / rhs,
                        data[6] / rhs, data[7] / rhs, data[8] / rhs);
    }

    /**
     * Addition operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr &operator+=(T rhs) __attribute__((__always_inline__)) {
      *this = *this + rhs;
      return *this;
    }

    /**
     * Subtraction operator
     * @param rhs Right hand side argument of binary operator.
     */
    inline Matrix3<T> constexpr &operator-=(T rhs) __attribute__((__always_inline__)) {
      *this = *this - rhs;
      return *this;
    }

    /**
     * Multiplication operator
