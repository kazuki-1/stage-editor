#pragma once
#include <DirectXMath.h>
#include <memory>
#include <Windows.h>
#include <x3daudio.h>
#include "CerealHeaders.h"
using namespace DirectX;

/// <summary>
/// Math namespace. 
/// </summary>
namespace Math
{
    /// <summary>
    /// Clamps the float value between the 2 given values
    /// </summary>
    /// <param name="tar"> : Value to clamp</param>
    /// <param name="min"> : Minimum value to be clamped to</param>
    /// <param name="max"> : Maximum value to be clamped to</param>
    /// <returns></returns>
    extern float Clamp(float tar, float min, float max);
    /// <summary>
    /// Returns -1 if tar < 0, 1 if tar > 0
    /// </summary>
    /// <param name="tar"></param>
    /// <returns></returns>
    extern float Sign(float tar);
    /// <summary>
    /// Converts tar from Radians to Degrees
    /// </summary>
    /// <returns></returns>
    extern float ToDegrees(float tar);
    /// <summary>
    /// Converts tar from Degrees to Radians
    /// </summary>
    /// <returns></returns>
    extern float ToRadians(float tar);
    /// <summary>
    /// Lerp the 2 values
    /// </summary>
    /// <returns></returns>
    extern float Lerp(float f1, float f2, float factor);


    class Vector2
    {
    public:
        float x{}, y{};
        Vector2() {};
        Vector2(float valX, float valY) : x(valX), y(valY) {};
        Vector2(const Vector2& v) : x(v.x), y(v.y) {}
        void operator=(const Vector2& v);
        bool operator ==(const Vector2& v);
        bool operator !=(const Vector2& v);
        Vector2 operator +(const Vector2& v);
        Vector2 operator -(const Vector2& v);
        Vector2 operator *(const float& f);
        Vector2 operator /(const float& f);
        float operator *(Vector2& v);
        void operator +=(const Vector2& v);
        void operator -=(const Vector2& v);
        void operator *=(const float& f);
        void operator /=(const float& f);

        /// <summary>
        /// Returns the length of the Vector
        /// </summary>
        /// <returns></returns>
        float Length();
        /// <summary>
        /// Returns the squared length of the vector
        /// </summary>
        /// <returns></returns>
        float LengthSq();
        /// <summary>
        /// Returns the Dot product of the 2 Vectors
        /// </summary>
        /// <returns></returns>
        float Dot(Vector2& v);
        /// <summary>
        /// Normalizes the vector
        /// </summary>
        /// <returns></returns>
        void Normalize();
        /// <summary>
        /// Returns the XMFLOAT2 version of the vector
        /// </summary>
        /// <returns></returns>
        XMFLOAT2 XMF2();
        /// <summary>
        /// Returns the XMVECTOR version of the vector
        /// </summary>
        /// <returns></returns>
        XMVECTOR XMV();
        /// <summary>
        /// Static function. Returns the dot product of the 2 vectors
        /// </summary>
        static float Dot(Vector2& v1, Vector2& v2);
        /// <summary>
        /// Static function. Returns the length of the vector
        /// </summary>
        /// <returns></returns>
        static float Length(Vector2& v);
        /// <summary>
        /// Static function. Returns the distance vector between the 2 vectors
        /// </summary>
        /// <returns></returns>
        static float LengthSq(Vector2& v);
        /// <summary>
        /// Get the angle difference between the 2 vectors
        /// </summary>
        /// <returns></returns>
        static float GetAngle(Vector2& v1, Vector2& v2);
        /// <summary>
        /// Static function. Returns the distance vector between the 2 vectors
        /// </summary>
        /// <returns></returns>
        static Vector2 Distance(Vector2& v1, Vector2& v2);
        /// <summary>
        /// Static function. Lerps the 2 vectors and returns the output
        /// </summary>
        /// <returns></returns>
        static Vector2 Lerp(Vector2& v1, Vector2& v2, float factor);
        template <class T>
        void serialize(T& t)
        {
            t(x, y);
        }


    };

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
        void operator = (const Vector3& v);
        bool operator ==(const Vector3& v);
        bool operator !=(const Vector3& v);
        float operator *(const Vector3& v);
        Vector3 operator +(const Vector3& v);
        Vector3 operator +(const XMFLOAT3& v);
        Vector3 operator -(const Vector3& v);
        Vector3 operator *(const float& f);
        Vector3 operator /(const float& f);
        Vector3 operator /(const Vector3& v);
        Vector3 operator -();
        void operator +=(const Vector3& v);
        void operator -=(const Vector3& v);
        void operator *=(const float& f);
        void operator /=(const float& f);
        void operator /=(const Vector3& v);

        /// <summary>
        /// Loads the XMVECTOR into the Vector3
        /// </summary>
        /// <param name="v"></param>
        void Load(XMVECTOR v);
        /// <summary>
        /// Returns the length of the Vector3
        /// </summary>
        /// <returns></returns>
        float Length();
        /// <summary>
        /// Returns the square length of the Vector3
        /// </summary>
        /// <returns></returns>
        float LengthSq();
        /// <summary>
        /// Returns the dot of this and the target Vector3
        /// </summary>
        /// <param name="tar"> : Target Vector3</param>
        /// <returns></returns>
        float Dot(Vector3 tar);
        /// <summary>
        /// Returns the normalized version of this Vector3
        /// </summary>
        /// <returns></returns>
        void Normalize();
        /// <summary>
        /// Returns the cross product between this and the target Vector3
        /// </summary>
        /// <param name="tar"> : Target Vector3</param>
        /// <returns></returns>
        Vector3 Cross(Vector3 tar);
        /// <summary>
        /// Returns the XMFLOAT3 version of the vector
        /// </summary>
        /// <returns></returns>
        XMFLOAT3 XMF3();
        /// <summary>
        /// Returns the XMVECTOR version of the vector
        /// </summary>
        /// <returns></returns>
        XMVECTOR XMV();
        X3DAUDIO_VECTOR X3DV();
        /// <summary>
        /// Static function. Returns the length of the Vector3
        /// </summary>
        /// <returns></returns>
        static float Length(Vector3& v);
        /// <summary>
        /// Static function. Returns the square length of the Vector3
        /// </summary>
        /// <returns></returns>
        static float LengthSq(Vector3& v);
        /// <summary>
        /// Static function. Returns the dot product of the 2 vectors
        /// </summary>
        /// <returns></returns>
        static float Dot(Vector3& v1, Vector3& v2);
        /// <summary>
        /// Returns the angle difference of the 2 vectors
        /// </summary>
        /// <returns></returns>
        static float GetAngle(Vector3& v1, Vector3& v2);
        /// <summary>
        /// Static function. Returns the distance vector of the 2 vectors
        /// </summary>
        /// <returns></returns>
        static Vector3 Distance(Vector3& v1, Vector3& v2);
        /// <summary>
        /// Static function. Returns the cross product of the 2 vectors
        /// </summary>
        /// <returns></returns>
        static Vector3 Cross(Vector3& v1, Vector3& v2);
        /// <summary>
        /// Static function. Returns the normalized vector of this vector
        /// </summary>
        /// <returns></returns>
        static Vector3 Normalize(Vector3& v);
        /// <summary>
        /// Static function. Lerps the 2 vectors and returns the output
        /// </summary>
        /// <returns></returns>
        static Vector3 Lerp(Vector3& v1, Vector3& v2, float factor);
        /// <summary>
        /// Converts the members from degree to angle and return
        /// </summary>
        /// <returns></returns>
        static Vector3 ToRadians(Vector3& v);
        template <class T>
        void serialize(T& t)
        {
            t(x, y, z);
        }

    };

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

        void operator =(const Vector4& v);
        bool operator == (const Vector4& v);
        bool operator != (const Vector4& v);
        Vector4 operator +(const Vector4& v);
        Vector4 operator -(const Vector4& v);
        Vector4 operator *(const float& v);
        Vector4 operator /(const float& v);
        void operator +=(const Vector4& v);
        void operator -=(const Vector4& v);
        void operator *=(const float& v);
        void operator /=(const float& v);


        XMFLOAT4 XMF4();
        XMVECTOR XMV();
        void Load(XMVECTOR v);
        /// <summary>
        /// Creates a quaternion rotation matrix from the Vector4
        /// </summary>
        /// <returns></returns>
        static XMMATRIX ToQuaternionMatrix(Vector4& v);
        /// <summary>
        /// Creates a quaternion from a Vector3
        /// </summary>
        static Vector4 Quaternion(Vector3 v);

        template <class T>
        void serialize(T& t)
        {
            t(x, y, z, w);
        }

    };

}

