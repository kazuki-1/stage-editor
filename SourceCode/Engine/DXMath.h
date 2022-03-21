#pragma once
#include <DirectXMath.h>
#include <memory>
#include "CerealHeaders.h"
using namespace DirectX;
/// <summary>
/// This is only for Vector3
/// </summary>
namespace Math
{
#pragma region MATH FUNCTIONS
    /// <summary>
    /// Clamps the float value between the 2 given values
    /// </summary>
    /// <param name="tar"> : Value to clamp</param>
    /// <param name="min"> : Minimum value to be clamped to</param>
    /// <param name="max"> : Maximum value to be clamped to</param>
    /// <returns></returns>
    inline float Clamp(float tar, float min, float max)
    {
        float temp = (std::min)((std::max)(tar, min), max);
        return temp;
    }
    /// <summary>
    /// Returns -1 if tar < 0, 1 if tar > 0
    /// </summary>
    /// <returns></returns>
    inline float Sign(float tar)
    {
        return tar / fabs(tar);
    }
    /// <summary>
    /// Converts tar from Radians to Degrees
    /// </summary>
    /// <returns></returns>
    inline float ToDegrees(float tar)
    {
        return tar * 180 / XM_PI;
    }
    /// <summary>
    /// Converts tar from Degrees to Radians
    /// </summary>
    /// <returns></returns>
    inline float ToRadians(float tar)
    {
        return tar / 180 * XM_PI;
    }
    /// <summary>
    /// Returns the dot of the 2 vectors
    /// </summary>
    /// <param name="v1"> : Position of object</param>
    /// <param name="v2"> : Position of target</param>
    /// <returns></returns>
    inline float Dot(XMVECTOR v1, XMVECTOR v2)
    {
        XMFLOAT3 V1, V2;
        XMStoreFloat3(&V1, v1);
        XMStoreFloat3(&V2, v2);
        return V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;
    }
    /// <summary>
    /// Returns the cross product between the 2 vectors
    /// </summary>
    /// <param name="v1"> : Position of object</param>
    /// <param name="v2"> : Position of target</param>
    /// <returns></returns>
    inline XMVECTOR Cross(XMVECTOR v1, XMVECTOR v2)
    {
        return XMVector3Cross(v1, v2);
    }


    /// <summary>
    /// Returns the angle between the 2 coordinates
    /// </summary>
    /// <param name="cur"> : Position of object</param>
    /// <param name="tar"> : Position of target</param>
    /// <returns></returns>
    /// <summary>
    /// Return the length of the distance between the two vectors
    /// </summary>
    /// <param name="cur"> : Position of object</param>
    /// <param name="tar"> : Position of target</param>
    /// <returns></returns>
    //inline float Distance(XMFLOAT3 cur, XMFLOAT3 tar)
    //{
    //    XMVECTOR Cur{ XMLoadFloat3(&cur) }, Tar{XMLoadFloat3(&tar)};
    //    XMVECTOR Dist{ Cur - Tar };
    //    return Length(Dist);

    //}

#pragma endregion

#pragma region VECTOR2

    class VECTOR2
    {
    public:
        float x{}, y{};
        VECTOR2() {};
        VECTOR2(float valX, float valY) : x(valX), y(valY) {};
        VECTOR2(const VECTOR2& v) : x(v.x), y(v.y) {}
        void operator=(const VECTOR2& v)
        {
            x = v.x;
            y = v.y;
        }

        bool operator ==(const VECTOR2& v)
        {
            return x == v.x && y == v.y;
        }
        bool operator /=(const VECTOR2& v)
        {
            return x != v.x || y != v.y;
        }
        VECTOR2 operator +(const VECTOR2& v)
        {
            return VECTOR2(x + v.x, y + v.y);
        }
        VECTOR2 operator -(const VECTOR2& v)
        {
            return VECTOR2(x - v.x, y - v.y);
        }
        VECTOR2 operator *(const float& f)
        {
            return VECTOR2(x * f, y * f);
        }
        VECTOR2 operator /(const float& f)
        {
            return VECTOR2(x * f, y * f);
        }

        float operator *(VECTOR2& v)
        {
            return Dot(v);
        }

        void operator +=(const VECTOR2& v)
        {
            x += v.x;
            y += v.y;
        }
        void operator -=(const VECTOR2& v)
        {
            x -= v.x;
            y -= v.y;
        }
        void operator *=(const float& f)
        {
            x*= f;
            y*= f;
        }
        void operator /=(const float& f)
        {
            x /= f;
            y /= f;
        }

        /// <summary>
        /// Returns the length of the Vector
        /// </summary>
        /// <returns></returns>
        float Length()
        {
            return sqrtf(x * x + y * y);
        }
        /// <summary>
        /// Returns the squared length of the vector
        /// </summary>
        /// <returns></returns>
        float LengthSq()
        {
            return x * x + y * y;
        }
        /// <summary>
        /// Returns the Dot product of the 2 Vectors
        /// </summary>
        /// <returns></returns>
        float Dot(VECTOR2& v)
        {
            return x * v.x + y * v.y;
        }
        /// <summary>
        /// Returns the Normalized version of the vector
        /// </summary>
        /// <returns></returns>
        void Normalize(VECTOR2* v)
        {
            float l{ Length() };
            VECTOR2 temp{ x / l, y / l };
            v = &temp;
        }
        void Normalize()
        {
            float l{ Length() };
            x /= l;
            y /= l;
        }
        /// <summary>
        /// Returns the distance vector between the 2 vectors
        /// </summary>
        /// <returns></returns>
        VECTOR2 DistanceTo(VECTOR2& tar)
        {
            return *this - tar;
        }
        /// <summary>
        /// Returns the XMFLOAT2 version of the vector
        /// </summary>
        /// <returns></returns>
        XMFLOAT2 XMF2()
        {
            return XMFLOAT2(x, y);
        }
        /// <summary>
        /// Returns the XMVECTOR version of the vector
        /// </summary>
        /// <returns></returns>
        XMVECTOR XMV()
        {
            XMFLOAT2 temp{ XMF2() };
            return XMLoadFloat2(&temp);
        }


        template <class T>
        void serialize(T& t)
        {
            t(x, y);
        }


    };

#pragma endregion
#pragma region VECTOR3

    class VECTOR3
    {
    public:
        float x{}, y{}, z{};
        VECTOR3() {};
        VECTOR3(float valX, float valY, float valZ) : x(valX), y(valY), z(valZ)
        {

        }
        VECTOR3(const VECTOR3& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        VECTOR3(XMFLOAT3 p)
        {
            x = p.x;
            y = p.y;
            z = p.z;
        }

        void operator = (const VECTOR3& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        
        bool operator ==(const VECTOR3& v)
        {
            return x == v.x && y == v.y && z == v.z;
        }
        bool operator !=(const VECTOR3& v)
        {
            return x != v.x || y != v.y || z != v.z;
        }
        float operator *(const VECTOR3& v)
        {
            return Dot(v);
        }
        VECTOR3 operator +(const VECTOR3& v)
        {
            return VECTOR3(x + v.x, y + v.y, z + v.z);
        }
        VECTOR3 operator +(const XMFLOAT3& v)
        {
            return *this + VECTOR3(v);
        }
        VECTOR3 operator -(const VECTOR3& v)
        {
            return VECTOR3(x - v.x, y - v.y, z - v.z);
        }
        VECTOR3 operator *(const float& f)
        {
            return VECTOR3(x * f, y * f, z * f);
        }
        VECTOR3 operator /(const float& f)
        {
            return VECTOR3(x / f, y / f, z / f);
        }
        VECTOR3 operator /(const VECTOR3& v)
        {
            return Cross(v);
        }
        VECTOR3 operator -()
        {
            return *this * -1;;
        }
        void operator +=(const VECTOR3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }
        void operator -=(const VECTOR3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;

        }
        void operator *=(const float& f)
        {
            x *= f;
            y *= f;
            z *= f;

        }
        void operator /=(const float& f)
        {
            x /= f;
            y /= f;
            z /= f;

        }
        void operator /=(const VECTOR3& v)
        {
            Cross(v);
        }


        void Load(XMVECTOR v)
        {
            XMFLOAT3 f;
            XMStoreFloat3(&f, v);
            *this = f;
        }
        /// <summary>
        /// Returns the length of the Vector3
        /// </summary>
        /// <returns></returns>
        float Length()
        {
            return sqrtf(x * x + y * y + z * z);
        }
        /// <summary>
        /// Returns the square length of the Vector3
        /// </summary>
        /// <returns></returns>
        float LengthSq()
        {
            return x * x + y * y + z * z;
        }
        /// <summary>
        /// Returns the dot of this and the target Vector3
        /// </summary>
        /// <param name="tar"> : Target Vector3</param>
        /// <returns></returns>
        float Dot(VECTOR3 tar)
        {
            return x * tar.x + y * tar.y + z * tar.z;
        }
        /// <summary>
        /// Returns the normalized version of this Vector3
        /// </summary>
        /// <returns></returns>
        void Normalize()
        {
            float l{ Length() };
            x /= l;
            y /= l;
            z /= l;
        };
        void Normalize(VECTOR3* tar)
        {
            VECTOR3 v;
            v = *this;
            v.Normalize();
            tar = &v;
        }
        /// <summary>
        /// Returns the cross product between this and the target Vector3
        /// </summary>
        /// <param name="tar"> : Target Vector3</param>
        /// <returns></returns>
        VECTOR3 Cross(VECTOR3 tar)
        {
            return VECTOR3(
                y * z - tar.z * tar.y,
                z * x - tar.x * tar.z,
                x * y - tar.y * tar.x);
        }
        /// <summary>
        /// Returns the distance between this and the target Vector3 
        /// </summary>
        /// <param name="tar"> : Target Vector3</param>
        /// <returns></returns>
        VECTOR3 DistanceTo(VECTOR3 tar)
        {
            return *this - tar;
        }
        /// <summary>
        /// Returns the XMFLOAT3 version of the vector
        /// </summary>
        /// <returns></returns>
        XMFLOAT3 XMF3()
        {
            return XMFLOAT3(x, y, z);
        }
        /// <summary>
        /// Returns the XMVECTOR version of the vector
        /// </summary>
        /// <returns></returns>
        XMVECTOR XMV()
        {
            XMFLOAT3 temp{ XMF3() };

            return XMLoadFloat3(&temp);
        }



        template <class T>
        void serialize(T& t)
        {
            t(x, y, z);
        }

    };

#pragma endregion
    class VECTOR4
    {
    public:
        float x{}, y{}, z{}, w{};
        VECTOR4() : x(0), y(0), z(0), w(0) {};
        VECTOR4(float valX, float valY, float valZ, float valW) : x(valX), y(valY), z(valZ), w(valW) {};
        VECTOR4(const VECTOR4& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }
        VECTOR4(XMFLOAT4 f) : x(f.x), y(f.y), z(f.z), w(f.w) {}
        VECTOR4(XMVECTOR v)
        {
            XMFLOAT4 temp;
            XMStoreFloat4(&temp, v);
            *this = temp;
        }
        XMFLOAT4 XMF4()
        {
            return XMFLOAT4(x, y, z, w);
        }
        XMVECTOR XMV()
        {
            XMFLOAT4 temp{ x, y, z, w };
            return XMLoadFloat4(&temp);
        }
        XMMATRIX ToQuaternionMatrix()
        {
            Load(XMQuaternionRotationRollPitchYawFromVector(XMV()));
            return XMMatrixRotationQuaternion(XMV());
        }
        void Load(XMVECTOR v)
        {
            XMFLOAT4 temp;
            XMStoreFloat4(&temp, v);
            *this = temp;
        }
        void operator =(const VECTOR4& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }
        bool operator == (const VECTOR4& v)
        {
            return x == v.x && y == v.y && z == v.z && w == v.w;
        }
        bool operator != (const VECTOR4& v)
        {
            return x != v.x || y != v.y || z != v.z || w != v.w;
        }
        template <class T>
        void serialize(T& t)
        {
            t(x, y, z, w);
        }

    };

    class Matrix
    {
        float val[4][4] = {};

        void Rotate(VECTOR3 rotation)
        {

        }
    };

    // TODO : FIX AIMING FOR ENEMY
    inline float GetAngle(VECTOR3 cur, VECTOR3 tar)
    {

        //VECTOR3 temp{ 0, 0, 1 };
        VECTOR3 Cur{ cur }, Tar{ tar };
        VECTOR3 dist{ tar - cur };
        dist.Normalize();
        Tar.Normalize();
        float dot{ Tar.Dot(dist) };
        Clamp(dot, -1.0f, 1.0f);
        float angle{ ToDegrees(acosf(dot)) };
        return acosf(dot);
    }

    inline float GetDirection(VECTOR3 cur, VECTOR3 tar)
    {
        VECTOR3 Cur{ cur }, Tar{ tar };

        Tar.Normalize();
        Cur.Normalize();
        VECTOR3 temp{ Tar - Cur };
        if (temp.Length() == 0)
        {
            return 0;
        }
        float dot{ Tar.Dot(Cur) };
        Clamp(dot, -1.0f, 1.0f);
        return acosf(dot);

    }

    inline float Length(VECTOR3& v1, VECTOR3& v2)
    {
        VECTOR3 temp{ v1 - v2 };
        return temp.Length();
    }

    inline float Length(VECTOR3 v1)
    {
        return v1.Length();
    }

    inline VECTOR3 ToRadians(VECTOR3 v)
    {
        return{ ToRadians(v.x), ToRadians(v.y), ToRadians(v.z) };
    }

    inline VECTOR3 Lerp(VECTOR3 v1, VECTOR3 v2, float factor) {
        return v1 + (v2 - v1) * factor;
    }
    inline float Lerp(float f1, float f2, float factor)
    {
        return f1 + (f2 - f1) * factor;
    }
    inline VECTOR4 Quaternion(VECTOR3 r)
    {
        VECTOR4 q;
        q.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));
        return q;
    }
    //inline float Distance(VECTOR3& v1, VECTOR3& v2)
    //{
    //    VECTOR3 dist{ v1 - v2 };
    //    return Length(dist);
    //}
}

