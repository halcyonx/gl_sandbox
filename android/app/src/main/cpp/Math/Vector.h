#pragma once

#include <cassert>

namespace Math
{
    template <typename T, int size>
    class Vector
    {
    public:
        Vector() = default;

        Vector(T x, T y, T z, T w)
        {
            v[0] = x;
            v[1] = y;
            v[2] = z;
            v[3] = w;
        }

        const T& operator[](int i) const
        {
            assert(i >= 0 && i < size);
            return v[i];
        }

        T& operator[](int i)
        {
            assert(i >= 0 && i < size);
            return v[i];
        }

        T v[size];
    };

    using Vec2 = Vector<float, 2>;
    using Vec3 = Vector<float, 3>;
    using Vec4 = Vector<float, 4>;

}