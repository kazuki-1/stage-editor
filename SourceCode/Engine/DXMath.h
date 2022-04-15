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

#pragma region Vector2

    class Vector2
    {
    public:
        float x{}, y{};
        Vector2() {};
        Vector2(float valX, float valY) : x(valX), y(valY) {};
        Vector2(const Vector2& v) : x(v.x), y(v.y) {}
        void operator=(const Vector2& v)
        {
            x = v.x;
            y = v.y;
        }

        bool operator ==(const Vector2& v)
        {
            return x == v.x && y == v.y;
        }
        bool operator /=(const Vector2& v)
        {
            return x != v.x || y != v.y;
        }
        Vector2 operator +(const Vector2& v)
        {
            return Vector2(x + v.x, y + v.y);
        }
        Vector2 operator -(const Vector2& v)
        {
            return Vector2(x - v.x, y - v.y);
        }
        Vector2 operator *(const float& f)
        {
            return Vector2(x * f, y * f);
        }
        Vector2 operator /(const float& f)
        {
            return Vector2(x * f, y * f);
        }

        float operator *(Vector2& v)
        {
            return Dot(v);
        }

        void operator +=(const Vector2& v)
        {
            x += v.x;
            y += v.y;
        }
        void operator -=(const Vector2& v)
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
        float Dot(Vector2& v)
        {
            return x * v.x + y * v.y;
        }
        /// <summary>
        /// Returns the Normalized version of the vector
        /// </summary>
        /// <returns></returns>
        void Normalize(Vector2* v)
        {
            float l{ Length() };
            Vector2 temp{ x / l, y / l };
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
        Vector2 DistanceTo(Vector2& tar)
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

        static Vector2 Distance(Vector2& v1, Vector2& v2)
        {
            return v2 - v1;
        }
        static float Dot(Vector2& v1, Vector2& v2)
        {
            return v1.Dot(v2);
        }
        static float Length(Vector2& v)
        {
            return v.Length();
        }
        static float LengthSq(Vector2& v)
        {
            return v.LengthSq();
        }
        template <class T>
        void serialize(T& t)
        {
            t(x, y);
        }


    };

#pragma endregion
#pragma region Vector3

    class Vector3
    {
    public:
        float x{}, y{}, z{};
        Vector3() {};
        Vector3(float valX, float valY, float valZ) : x(valX), y(valY), z(valZ)
        {

        }
        Vector3(const Vector3& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        Vector3(XMFLOAT3 p)
        {
            x = p.x;
            y = p.y;
            z = p.z;
        }

        void operator = (const Vector3& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        
        bool operator ==(const Vector3& v)
        {
            return x == v.x && y == v.y && z == v.z;
        }
        bool operator !=(const Vector3& v)
        {
            return x != v.x || y != v.y || z != v.z;
        }
        float operator *(const Vector3& v)
        {
            return Dot(v);
        }
        Vector3 operator +(const Vector3& v)
        {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }
        Vector3 operator +(const XMFLOAT3& v)
        {
            return *this + Vector3(v);
        }
        Vector3 operator -(const Vector3& v)
        {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }
        Vector3 operator *(const float& f)
        {
            return Vector3(x * f, y * f, z * f);
        }
        Vector3 operator /(const float& f)
        {
            return Vector3(x / f, y / f, z / f);
        }
        Vector3 operator /(const Vector3& v)
        {
            return Cross(v);
        }
        Vector3 operator -()
        {
            return *this * -1;;
        }
        void operator +=(const Vector3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }
        void operator -=(const Vector3& v)
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
        void operator /=(const Vector3& v)
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
        float Dot(Vector3 tar)
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
        void Normalize(Vector3* tar)
        {
            Vector3 v;
            v = *this;
            v.Normalize();
            tar = &v;
        }
        /// <summary>
        /// Returns a normalized Vector3 of this vector
        /// </summary>
        /// <returns></returns>
        Vector3 Normalized()
        {
            float l{ Length() };
            return { x / l, y / l, z / l };
        }
        /// <summary>
        /// Returns the cross product between this and the target Vector3
        /// </summary>
        /// <param name="tar"> : Target Vector3</param>
        /// <returns></returns>
        Vector3 Cross(Vector3 tar)
        {
            return Vector3(
                y * z - tar.z * tar.y,
                z * x - tar.x * tar.z,
                x * y - tar.y * tar.x);
        }
        /// <summary>
        /// Returns the distance between this and the target Vector3 
        /// </summary>
        /// <param name="tar"> : Target Vector3</param>
        /// <returns></returns>
        Vector3 DistanceTo(Vector3 tar)
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
        /// <summary>
        /// Returns the length of the Vector3
        /// </summary>
        /// <returns></returns>
        static float Length(Vector3& v)
        {
            return v.Length();
        }
        /// <summary>
        /// Returns the square length of the Vector3
        /// </summary>
        /// <returns></returns>
        static float LengthSq(Vector3& v)
        {
            return v.LengthSq();
        }
        static float Dot(Vector3& v1, Vector3& v2)
        {
            return v1.Dot(v2);
        }
        static Vector3 Distance(Vector3& v1, Vector3& v2)
        {
            return v2 - v1;
        }
        static Vector3 Cross(Vector3& v1, Vector3& v2)
        {
            return v1.Cross(v2);
        }

        template <class T>
        void serialize(T& t)
        {
            t(x, y, z);
        }

    };

#pragma endregion
    class Vector4
    {
    public:
        float x{}, y{}, z{}, w{};
        Vector4() : x(0), y(0), z(0), w(0) {};
        Vector4(float valX, float valY, float valZ, float valW) : x(valX), y(valY), z(valZ), w(valW) {};
        Vector4(const Vector4& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }
        Vector4(XMFLOAT4 f) : x(f.x), y(f.y), z(f.z), w(f.w) {}
        Vector4(XMVECTOR v)
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
        void operator =(const Vector4& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }
        bool operator == (const Vector4& v)
        {
            return x == v.x && y == v.y && z == v.z && w == v.w;
        }
        bool operator != (const Vector4& v)
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

        void Rotate(Vector3 rotation)
        {

        }
    };

    // TODO : FIX AIMING FOR ENEMY
    inline float GetAngle(Vector3 cur, Vector3 tar)
    {

        //VECTOR3 temp{ 0, 0, 1 };
        Vector3 Cur{ cur }, Tar{ tar };
        Vector3 dist{ tar - cur };
        dist.Normalize();
        Tar.Normalize();
        float dot{ Tar.Dot(dist) };
        Clamp(dot, -1.0f, 1.0f);
        float angle{ ToDegrees(acosf(dot)) };
        return acosf(dot);
    }

    inline float GetDirection(Vector3 cur, Vector3 tar)
    {
        Vector3 Cur{ cur }, Tar{ tar };

        Tar.Normalize();
        Cur.Normalize();
        Vector3 temp{ Tar - Cur };
        if (temp.Length() == 0)
        {
            return 0;
        }
        float dot{ Tar.Dot(Cur) };
        Clamp(dot, -1.0f, 1.0f);
        return acosf(dot);

    }

    inline float Length(Vector3& v1, Vector3& v2)
    {
        Vector3 temp{ v1 - v2 };
        return temp.Length();
    }

    inline float Length(Vector3 v1)
    {
        return v1.Length();
    }

    inline Vector3 ToRadians(Vector3 v)
    {
        return{ ToRadians(v.x), ToRadians(v.y), ToRadians(v.z) };
    }

    inline Vector3 Lerp(Vector3 v1, Vector3 v2, float factor) {
        return v1 + (v2 - v1) * factor;
    }
    inline float Lerp(float f1, float f2, float factor)
    {
        return f1 + (f2 - f1) * factor;
    }
    inline Vector4 Quaternion(Vector3 r)
    {
        Vector4 q;
        q.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));
        return q;
    }
    //inline float Distance(VECTOR3& v1, VECTOR3& v2)
    //{
    //    VECTOR3 dist{ v1 - v2 };
    //    return Length(dist);
    //}
}

