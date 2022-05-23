#include "Math.h"

using namespace Math;


/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------Math Functions---------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------Clamp()----------------------------------------------------*/
/// <summary>
/// Clamps the float value between the 2 given values
/// </summary>
/// <param name="tar"> : Value to clamp</param>
/// <param name="min"> : Minimum value to be clamped to</param>
/// <param name="max"> : Maximum value to be clamped to</param>
/// <returns></returns>
float Math::Clamp(float target, float min, float max)
{
    float temp = (std::min)((std::max)(target, min), max);
    return temp;
}

/*-------------------------------------------------------Sign()-----------------------------------------------------*/
/// <summary>
/// Returns -1 if tar < 0, 1 if tar > 0
/// </summary>
/// <param name="tar"></param>
/// <returns></returns>
float Math::Sign(float tar)
{
    return tar / fabs(tar);
}

/*-------------------------------------------------------ToDegrees()------------------------------------------------*/
/// <summary>
/// Converts tar from Radians to Degrees
/// </summary>
/// <returns></returns>
float Math::ToDegrees(float tar)
{
    return tar * 180 / XM_PI;
}

/*-------------------------------------------------------ToRadians()------------------------------------------------*/
/// <summary>
/// Converts tar from Degrees to Radians
/// </summary>
/// <returns></returns>
float Math::ToRadians(float tar)
{
    return tar / 180 * XM_PI;
}

/*-------------------------------------------------------Lerp()------------------------------------------------*/
/// <summary>
/// Lerp the 2 values
/// </summary>
/// <returns></returns>
float Math::Lerp(float f1, float f2, float factor)
{
    return f1 + ((f2 - f1) * factor);
}


/*------------------------------------------------------------------------------------------*/
/*---------------------------------Vector2 Class--------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*---------------------------------Vector2 Operators----------------------------------------*/

void Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;
}

/*------------------------------------------------------------------------------------------*/

bool Vector2::operator==(const Vector2& v)
{
	return x == v.x && y == v.y;
}

/*------------------------------------------------------------------------------------------*/

bool Vector2::operator!=(const Vector2& v)
{
	return x != v.x || y != v.y;
}

/*------------------------------------------------------------------------------------------*/

Vector2 Vector2::operator +(const Vector2& v)
{
    return Vector2(x + v.x, y + v.y);
}

/*------------------------------------------------------------------------------------------*/

Vector2 Vector2::operator -(const Vector2& v)
{
    return Vector2(x - v.x, y - v.y);
}

/*------------------------------------------------------------------------------------------*/

Vector2 Vector2::operator *(const float& f)
{
    return Vector2(x * f, y * f);
}

/*------------------------------------------------------------------------------------------*/

Vector2 Vector2::operator /(const float& f)
{
    return Vector2(x * f, y * f);
}

/*------------------------------------------------------------------------------------------*/

float Vector2::operator *(Vector2& v)
{
    return Dot(v);
}

/*------------------------------------------------------------------------------------------*/

void Vector2::operator +=(const Vector2& v)
{
    x += v.x;
    y += v.y;
}

/*------------------------------------------------------------------------------------------*/

void Vector2::operator -=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
}

/*------------------------------------------------------------------------------------------*/

void Vector2::operator *=(const float& f)
{
    x *= f;
    y *= f;
}

/*------------------------------------------------------------------------------------------*/

void Vector2::operator /=(const float& f)
{
    x /= f;
    y /= f;
}

/*-----------------------------------------Vector2 Length()-------------------------------------------------*/
/// <summary>
/// Returns the length of the Vector
/// </summary>
/// <returns></returns>
float Vector2::Length()
{
    return sqrtf(x * x + y * y);
}

/*-----------------------------------------Vector2 LengthSq()-------------------------------------------------*/
/// <summary>
/// Returns the squared length of the vector
/// </summary>
/// <returns></returns>
float Vector2::LengthSq()
{
    return x * x + y * y;
}

/*-----------------------------------------Vector2 Dot()-------------------------------------------------*/
/// <summary>
/// Returns the Dot product of the 2 Vectors
/// </summary>
/// <returns></returns>
float Vector2::Dot(Vector2& v)
{
    return x * v.x + y * v.y;
}

/*-----------------------------------------Vector2 Normalize()-------------------------------------------------*/
/// <summary>
/// Normalizes the vector
/// </summary>
/// <returns></returns>
void Vector2::Normalize()
{
    float l{ Length() };
    x /= l;
    y /= l;
}

/*-----------------------------------------Vector2 XMF2()-------------------------------------------------*/
/// <summary>
/// Returns the XMFLOAT2 version of the vector
/// </summary>
/// <returns></returns>
XMFLOAT2 Vector2::XMF2()
{
    return XMFLOAT2(x, y);
}

/*-----------------------------------------Vector2 XMV()-------------------------------------------------*/
/// <summary>
/// Returns the XMVECTOR version of the vector
/// </summary>
/// <returns></returns>
XMVECTOR Vector2::XMV()
{
    XMFLOAT2 temp{ XMF2() };
    return XMLoadFloat2(&temp);
}

/*------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------Vector2 Static Functions-----------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------Vector2 Dot()------------------------------------------------------------*/
/// <summary>
/// Static function. Returns the dot product of the 2 vectors
/// </summary>
float Vector2::Dot(Vector2& v1, Vector2& v2)
{
    return v1.Dot(v2);
}

/*-----------------------------------------Vector2 Length()---------------------------------------------------------*/
/// <summary>
/// Static function. Returns the length of the vector
/// </summary>
/// <returns></returns>
float Vector2::Length(Vector2& v)
{
    return v.Length();
}

/*-----------------------------------------Vector2 LengthSq()-------------------------------------------------------*/
/// <summary>
/// Static function. Returns the distance vector between the 2 vectors
/// </summary>
/// <returns></returns>
float Vector2::LengthSq(Vector2& v)
{
    return v.Length();
}

/*-----------------------------------------Vector2 GetAngle()-------------------------------------------------------*/
/// <summary>
/// Get the angle difference between the 2 vectors
/// </summary>
/// <returns></returns>
float Vector2::GetAngle(Vector2& v1, Vector2& v2)
{
    Vector2 Cur{ v1 }, Tar{ v2 };
    Vector2 dist{ Distance(Cur, Tar)};
    dist.Normalize();
    Tar.Normalize();
    float dot{ Tar.Dot(dist) };
    Clamp(dot, -1.0f, 1.0f);
    float angle{ ToDegrees(acosf(dot)) };
    return acosf(dot);
}

/*-------------------------------------------Vector2 Distance()-----------------------------------------------------*/
/// <summary>
/// Static function. Returns the distance vector between the 2 vectors
/// </summary>
/// <returns></returns>
Vector2 Vector2::Distance(Vector2& v1, Vector2& v2)
{
    return v2 - v1;
}

/*-----------------------------------------Vector2 LengthSq()-------------------------------------------------------*/
/// <summary>
/// Static function. Lerps the 2 vectors and returns the output
/// </summary>
/// <returns></returns>
Vector2 Vector2::Lerp(Vector2& v1, Vector2& v2, float factor)
{
    return v1 + ((v2 - v1) * factor);
}


/*------------------------------------------------------------------------------------------*/
/*---------------------------------Vector3 Class--------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/*---------------------------------Vector3 Operators----------------------------------------*/

void Vector3::operator = (const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

/*------------------------------------------------------------------------------------------*/

bool Vector3::operator ==(const Vector3& v)
{
    return x == v.x && y == v.y && z == v.z;
}

/*------------------------------------------------------------------------------------------*/

bool Vector3::operator !=(const Vector3& v)
{
    return x != v.x || y != v.y || z != v.z;
}

/*------------------------------------------------------------------------------------------*/

float Vector3::operator *(const Vector3& v)
{
    return Dot(v);
}

/*------------------------------------------------------------------------------------------*/

Vector3 Vector3::operator +(const Vector3& v)
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

/*------------------------------------------------------------------------------------------*/

Vector3 Vector3::operator +(const XMFLOAT3& v)
{
    return *this + Vector3(v);
}

/*------------------------------------------------------------------------------------------*/

Vector3 Vector3::operator -(const Vector3& v)
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

/*------------------------------------------------------------------------------------------*/

Vector3 Vector3::operator *(const float& f)
{
    return Vector3(x * f, y * f, z * f);
}

/*------------------------------------------------------------------------------------------*/

Vector3 Vector3::operator /(const float& f)
{
    return Vector3(x / f, y / f, z / f);
}

/*------------------------------------------------------------------------------------------*/

Vector3 Vector3::operator /(const Vector3& v)
{
    return Cross(v);
}

/*------------------------------------------------------------------------------------------*/

Vector3 Vector3::operator -()
{
    return *this * -1;;
}

/*------------------------------------------------------------------------------------------*/

void Vector3::operator +=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

/*------------------------------------------------------------------------------------------*/

void Vector3::operator -=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

}

/*-------------------------------------------------------------------------------------------------------------------*/

void Vector3::operator *=(const float& f)
{
    x *= f;
    y *= f;
    z *= f;

}

/*-------------------------------------------------------------------------------------------------------------------*/

void Vector3::operator /=(const float& f)
{
    x /= f;
    y /= f;
    z /= f;

}

/*-------------------------------------------------------------------------------------------------------------------*/

void Vector3::operator /=(const Vector3& v)
{
    Cross(v);
}

/*-------------------------------------------Vector3 Load()----------------------------------------------------------*/
/// <summary>
/// Loads the XMVECTOR into the Vector3
/// </summary>
/// <param name="v"></param>
void Vector3::Load(XMVECTOR v)
{
    XMFLOAT3 f;
    XMStoreFloat3(&f, v);
    *this = f;
}

/*-------------------------------------------Vector3 Length()--------------------------------------------------------*/
/// <summary>
/// Returns the length of the Vector3
/// </summary>
/// <returns></returns>
float Vector3::Length()
{
    return sqrtf(x * x + y * y + z * z);
}

/*-------------------------------------------Vector3 LengthSq()------------------------------------------------------*/
/// <summary>
/// Returns the square length of the Vector3
/// </summary>
/// <returns></returns>
float Vector3::LengthSq()
{
    return x * x + y * y + z * z;
}

/*-------------------------------------------Vector3 Dot()-----------------------------------------------------------*/
/// <summary>
/// Returns the dot of this and the target Vector3
/// </summary>
/// <param name="tar"> : Target Vector3</param>
/// <returns></returns>
float Vector3::Dot(Vector3 tar)
{
    return x * tar.x + y * tar.y + z * tar.z;
}

/*-------------------------------------------Vector3 Normalize()-----------------------------------------------------*/
/// <summary>
/// Returns the normalized version of this Vector3
/// </summary>
/// <returns></returns>
void Vector3::Normalize()
{
    float l{ Length() };
    x /= l;
    y /= l;
    z /= l;
};

/*-------------------------------------------Vector3 Cross()---------------------------------------------------------*/
/// <summary>
/// Returns the cross product between this and the target Vector3
/// </summary>
/// <param name="tar"> : Target Vector3</param>
/// <returns></returns>
Vector3 Vector3::Cross(Vector3 tar)
{
    return Vector3(
        y * tar.z - z * tar.y,
        z * tar.x - x * tar.z,
        x * tar.y - y * tar.x);
}

/*-------------------------------------------Vector3 XMF3()----------------------------------------------------------*/
/// <summary>
/// Returns the XMFLOAT3 version of the vector
/// </summary>
/// <returns></returns>
XMFLOAT3 Vector3::XMF3()
{
    return XMFLOAT3(x, y, z);
}

/*-------------------------------------------Vector3 XMV()-----------------------------------------------------------*/
/// <summary>
/// Returns the XMVECTOR version of the vector
/// </summary>
/// <returns></returns>
XMVECTOR Vector3::XMV()
{
    XMFLOAT3 temp{ XMF3() };

    return XMLoadFloat3(&temp);
}

/*-------------------------------------------Vector3 X3DV()-----------------------------------------------------------*/

X3DAUDIO_VECTOR Vector3::X3DV()
{
    return X3DAUDIO_VECTOR(x, y, z);
}


/*-------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------Vector3 Static Functions------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------Vector3 Length()--------------------------------------------------------*/
/// <summary>
/// Static function. Returns the length of the Vector3
/// </summary>
/// <returns></returns>
float Vector3::Length(Vector3& v)
{
    return v.Length();
}

/*-------------------------------------------Vector3 LengthSq()------------------------------------------------------*/
/// <summary>
/// Static function. Returns the square length of the Vector3
/// </summary>
/// <returns></returns>
float Vector3::LengthSq(Vector3& v)
{
    return v.LengthSq();
}

/*-------------------------------------------Vector3 Dot()-----------------------------------------------------------*/
/// <summary>
/// Static function. Returns the dot product of the 2 vectors
/// </summary>
/// <returns></returns>
float Vector3::Dot(Vector3& v1, Vector3& v2)
{
    return v1.Dot(v2);
}

/*-------------------------------------------Vector3 GetAngle()-----------------------------------------------------------*/
/// <summary>
/// Returns the angle difference of the 2 vectors
/// </summary>
/// <returns></returns>
float Vector3::GetAngle(Vector3& v1, Vector3& v2)
{
    Vector3 Cur{ v1 }, Tar{ v2 };
    Vector3 dist{ Distance(v1, v2)};
    if (dist.Length() == 0)
        return 0.0f;
    dist.Normalize();
    Tar.Normalize();
    float dot{ Tar.Dot(Cur) };
    Clamp(dot, -1.0f, 1.0f);
    float angle{ ToDegrees(acosf(dot)) };
    return acosf(dot);
}

/*-------------------------------------------Vector3 Distance()------------------------------------------------------*/
/// <summary>
/// Static function. Returns the distance vector of the 2 vectors
/// </summary>
/// <returns></returns>
 Vector3 Vector3::Distance(Vector3& v1, Vector3& v2)
{
    return v2 - v1;
}

/*-------------------------------------------Vector3 Cross()---------------------------------------------------------*/
/// <summary>
/// Static function. Returns the cross product of the 2 vectors
/// </summary>
/// <returns></returns>
 Vector3 Vector3::Cross(Vector3& v1, Vector3& v2)
{
    return v1.Cross(v2);
}

/*-------------------------------------------Vector3 Normalize()-----------------------------------------------------*/
/// <summary>
/// Static function. Returns the normalized vector of this vector
/// </summary>
/// <returns></returns>
 Vector3 Vector3::Normalize(Vector3& v)
{
    Vector3 out{ v };
    out.Normalize();
    return out;
}

/*-------------------------------------------Vector3 Lerp()---------------------------------------------------------*/
/// <summary>
/// Static function. Lerps the 2 vectors and returns the output
/// </summary>
/// <returns></returns>
Vector3 Vector3::Lerp(Vector3& v1, Vector3& v2, float factor)
{
    return v1 + ((v2 - v1) * factor);
}

/*-------------------------------------------Vector3 ToRadians()----------------------------------------------------*/
/// <summary>
/// Converts the members from degree to angle and return
/// </summary>
/// <returns></returns>
Vector3 Vector3::ToRadians(Vector3& v)
{
    return{ Math::ToRadians(v.x), Math::ToRadians(v.y), Math::ToRadians(v.z) };
}


/*------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------Vector4 Class----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------Vector4 operators-------------------------------------------------------*/

void Vector4::operator =(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool Vector4::operator == (const Vector4& v)
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

/*------------------------------------------------------------------------------------------------------------------*/

bool Vector4::operator != (const Vector4& v)
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

/*------------------------------------------------------------------------------------------------------------------*/

Vector4 Vector4::operator +(const Vector4& v)
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

/*------------------------------------------------------------------------------------------------------------------*/

Vector4 Vector4::operator -(const Vector4& v)
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

/*------------------------------------------------------------------------------------------------------------------*/

Vector4 Vector4::operator *(const float& v)
{
    return Vector4(x * v, y * v, z * v, w * v);
}


/*------------------------------------------------------------------------------------------------------------------*/

Vector4 Vector4::operator /(const float& v)
{
    return Vector4(x / v, y / v, z / v, w / v);
}

/*------------------------------------------------------------------------------------------------------------------*/

void Vector4::operator +=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

}

/*------------------------------------------------------------------------------------------------------------------*/

void Vector4::operator -=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

/*------------------------------------------------------------------------------------------------------------------*/

void Vector4::operator *=(const float& v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;

}

/*------------------------------------------------------------------------------------------------------------------*/

void Vector4::operator /=(const float& v)
{
    x /= v;
    y /= v;
    z /= v;
    w /= v;

}

/*-------------------------------------------------------------Vector4 XMF4()---------------------------------------*/

XMFLOAT4 Vector4::XMF4()
{
    return XMFLOAT4(x, y, z, w);
}

/*-------------------------------------------------------------Vector4 XMV()----------------------------------------*/

XMVECTOR Vector4::XMV()
{
    XMFLOAT4 temp{ x, y, z, w };
    return XMLoadFloat4(&temp);
}

/*-------------------------------------------------------------Vector4 Load()---------------------------------------*/

void Vector4::Load(XMVECTOR v)
{
    XMFLOAT4 temp;
    XMStoreFloat4(&temp, v);
    *this = temp;
}

/*------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------Vector4 Static Functions-----------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------Vector4 ToQuaternionMatrix()-------------------------------------------*/
/// <summary>
/// Creates a quaternion rotation matrix from the Vector4
/// </summary>
/// <returns></returns>
XMMATRIX Vector4::ToQuaternionMatrix(Vector4& v)
{
    Vector4 out;
    out.Load(XMQuaternionRotationRollPitchYawFromVector(v.XMV()));
    return XMMatrixRotationQuaternion(out.XMV());
}

/*-------------------------------------------Vector4 Quaternion()---------------------------------------------------*/
/// <summary>
/// Creates a quaternion from a Vector3
/// </summary>
Vector4 Vector4::Quaternion(Vector3 v)
{
    Vector4 q;
    q.Load(XMQuaternionRotationRollPitchYawFromVector(v.XMV()));
    return q;
}

