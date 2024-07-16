#pragma once

#include <cmath>
#include <memory.h>
#include <limits>

namespace Math {
	const float Pi = 3.1415926535f;
	const float TwoPi = Pi * 2.0f;
	const float PiOver2 = Pi / 2.0f; // divide Pi by 2
	const float Infinity = std::numeric_limits<float>::infinity(); // positive infinity 
	const float NegInfinity = -std::numeric_limits<float>::infinity(); // negative infinity
	
	inline float ToRadian(float degrees) {
		return degrees * Pi / 180.0f;
	}

	inline float ToDegrees(float radian) {
		return radian * 180.0f / Pi;
	}

	inline bool NearZero(float val, float epsilon = 0.001f) {
		if (fabs(val) <= epsilon) {
			return true;
		}
		else {
			return false;
		}
	}

	template <typename T>
	T Max(const T& a, const T& b) {
		return (a < b ? b : a);
	}

	template <typename T>
	T Min(const T& a, const T& b) {
		return (a < b ? a : b);
	}

	template <typename T>
	T Clamp(const T& value, const T& lower, const T& upper) {
		return Min(upper, Max(lower, value));
	}

	inline float Abs(float value) {
		return fabs(value);
	}

	inline float Cos(float angle) {
		return cosf(angle);
	}

	inline float Sin(float angle) {
		return sinf(angle);
	}

	inline float Tan(float angle) {
		return tanf(angle);
	}
	// return value;
	inline float Acos(float value) {
		return acosf(value);
	}

	inline float Atan2(float y, float x) {
		return atan2f(y, x);
	}
	// return angle;
	inline float Cot(float angle) {
		return 1.0f / Tan(angle);
	}
	// return value;

	inline float Lerp(float a, float b, float f) {
		return a + f * (b - a);
	}
	inline float Sqrt(float value) {
		return sqrtf(value);
	}

	inline float Fmod(float numer, float denom) {
		return fmod(numer, denom);
	}
}

class Vector2 {
public:
	float x;
	float y;

	Vector2()
		:x(0.0f)
		, y(0.0f)
	{}

	explicit Vector2(float inX, float inY)
		:x(inX)
		, y(inY)
	{}

	void Set(float inX, float inY) {
		x = inX;
		y = inY;
	} // set voth components in one line

	friend Vector2 operator+(const Vector2& a, const Vector2& b) {
		return Vector2(a.x + b.x, a.y + b.y);
	} // vector addition (a + b)

	friend Vector2 operator-(const Vector2& a, const Vector2& b) {
		return Vector2(a.x - b.x, a.y - b.y);
	} // vector subtraction (a - b)

	friend Vector2 operator*(const Vector2& a, const Vector2& b) {
		return Vector2(a.x * b.x, a.y * b.y);
	} // vector components-wise multiplication

	friend Vector2 operator*(const Vector2& vec, float scalar) {
		return Vector2(vec.x * scalar, vec.y * scalar);
	}
	
	friend Vector2 operator*(float scalar, const Vector2& vec) {
		return Vector2(vec.x * scalar, vec.y * scalar);
	}

	Vector2& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	} // scalar *=

	Vector2& operator+=(const Vector2& right) {
		x += right.x;
		y += right.y;
		return *this;
	} // vector +=

	Vector2& operator-=(const Vector2& right) {
		x -= right.x;
		y -= right.y;
		return *this;
	} // vector +=

	float LengthSq() const {
		return (x * x + y * y);
	}

	float Length() const {
		return (Math::Sqrt(LengthSq()));
	} //length of vector

	void Noramlize() {
		float length = Length();
		x /= length;
		y /= length;
	} 

	static Vector2 Normalize(const Vector2& vec) {
		Vector2 temp = vec;
		temp.Noramlize();
		return temp;
	} // normalize the provided vector

	static float Dot(const Vector2& a, const Vector2& b) {
		return (a.x * b.x + a.y * b.y);
	} // Dot product

	static Vector2 Lerp(const Vector2& a, const Vector2& b, float f) {
		return Vector2(a + f * (b - a));
	}

	static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f); // transform vector by matrix

	static const Vector2 Zero;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 NegUnitX;
	static const Vector2 NegUnitY;
};

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3()
		:x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{}

	explicit Vector3(float inX, float inY, float inZ)
		:x(inX)
		, y(inY)
		, z(inZ)
	{}

	const float* GetAsFloatPtr() const {
		return reinterpret_cast<const float*>(&x);
	} // cast to a const float pointer

	friend Vector3 operator+(const Vector3& a, const Vector3& b) {
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	} // vector addition

	friend Vector3 operator-(const Vector3& a, const Vector3& b) {
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	} // vector subtraction

	friend Vector3 operator*(const Vector3& left, const Vector3& right) {
		return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
	} // component-wise multiplication

	friend Vector3 operator*(const Vector3& vec, float scalar) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	} 

	friend Vector3 operator*(float scalar, const Vector3& vec) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	} // scalar multiplication

	Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	} // scalar *=

	Vector3& operator+=(const Vector3& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	} // vector +=

	Vector3& operator-=(const Vector3& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	} // vector -=

	float LengthSq() const {
		return (x * x + y * y + z * z);
	}

	float Length() const {
		return (Math::Sqrt(LengthSq()));
	} // length of vector

	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
	}

	static Vector3 Normalize(const Vector3& vec) {
		Vector3 temp = vec;
		temp.Normalize();
		return temp;
	} // normalize the provide vector

	static float Dot(const Vector3& a, const Vector3& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	} // dot product

	static Vector3 Cross(const Vector3& a, const Vector3& b) {
		Vector3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	} // cross product

	static Vector3 Lerp(const Vector3& a, const Vector3& b, float f) {
		return Vector3(a + f * (b - a));
	} // Lerp form A to B by f

	static Vector3 Reflect(const Vector3& v, const Vector3& n) {
		return v - 2.0f * Vector3::Dot(v, n) * n;
	} // reflect v about (nomalized) n

	static Vector3 Transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
	static Vector3 TransformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f); // transform the vector and renormalize the w component

	static Vector3 Transform(const Vector3& v, const class Quaternion& q);

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 NegUnitX;
	static const Vector3 NegUnitY;
	static const Vector3 NegUnitZ;
	static const Vector3 Infinity;
	static const Vector3 NegInfinity;
};

class Matrix3 {
public:
	float mat[3][3];

	Matrix3() {
		*this = Matrix3::Identity;
	}
	
	explicit Matrix3(float inMat[3][3]) {
		memcpy(mat, inMat, 9 * sizeof(float));
	}

	const float* GetAsFloatPtr() const {
		return reinterpret_cast<const float*>(&mat[0][0]);
	} // cast to a const float pointer

	friend Matrix3 operator*(const Matrix3& left, const Matrix3& right) {
		Matrix3 retVal;

		retVal.mat[0][0] =
			left.mat[0][0] * right.mat[0][0] +
			left.mat[0][1] * right.mat[1][0] +
			left.mat[0][2] * right.mat[2][0];

		retVal.mat[0][1] =
			left.mat[0][0] * right.mat[0][1] +
			left.mat[0][1] * right.mat[1][1] +
			left.mat[0][2] * right.mat[2][1];

		retVal.mat[0][2] =
			left.mat[0][0] * right.mat[0][2] +
			left.mat[0][1] * right.mat[1][2] +
			left.mat[0][2] * right.mat[2][2];

		retVal.mat[1][0] =
			left.mat[1][0] * right.mat[0][0] +
			left.mat[1][1] * right.mat[1][0] +
			left.mat[1][2] * right.mat[2][0];

		retVal.mat[1][1] =
			left.mat[1][0] * right.mat[0][1] +
			left.mat[1][1] * right.mat[1][1] +
			left.mat[1][2] * right.mat[2][1];

		retVal.mat[1][2] =
			left.mat[1][0] * right.mat[0][2] +
			left.mat[1][1] * right.mat[1][2] +
			left.mat[1][2] * right.mat[2][2];

		retVal.mat[2][0] =
			left.mat[2][0] * right.mat[0][0] +
			left.mat[2][1] * right.mat[1][0] +
			left.mat[2][2] * right.mat[2][0];

		retVal.mat[2][1] =
			left.mat[2][0] * right.mat[0][1] +
			left.mat[2][1] * right.mat[1][1] +
			left.mat[2][2] * right.mat[2][1];

		retVal.mat[2][2] =
			left.mat[2][0] * right.mat[0][2] +
			left.mat[2][1] * right.mat[1][2] +
			left.mat[2][2] * right.mat[2][2];

		return retVal;
	}
	
	Matrix3& operator*=(const Matrix3& right) {
		*this = *this * right;
		return *this;
	} // matrix multiplication

	static Matrix3 CreateScale(float xScale, float yScale) {
		float temp[3][3] =
		{
			{ xScale, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return Matrix3(temp);
	} // create a scale matrix with x and y scale

	static Matrix3 CreateScale(const Vector2& scaleVector) {
		return CreateScale(scaleVector.x, scaleVector.y);
	} // create a scale matrix with vector

	static Matrix3 CreateScale(float scale) {
		return CreateScale(scale, scale);
	} // create a scale matrix with a uniform factor

	static Matrix3 CreateRotation(float theta) {
		float temp[3][3] = {
			{ Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 1.0f},
		};
		return Matrix3(temp);
	} // create a rotation matrix about the Z-axis(theta is in radians)

	static Matrix3 CreateTranslation(const Vector2& trans) {
		float temp[3][3] = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, 1.0f},
		};
		return Matrix3(temp);
	} // create a translation matrix (on the xy-plane)
	
	static const Matrix3 Identity; // identity matrix
};

class Matrix4 {
public:
	float mat[4][4];

	Matrix4() {
		*this = Matrix4::Identity;
	}

	explicit Matrix4(float inMat[4][4]) {
		memcpy(mat, inMat, 16 * sizeof(float));
	}

	const float* GetAsFloatPtr() const {
		return reinterpret_cast<const float*>(&mat[0][0]);
	} // Cast to a const float pointer

	friend Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
		Matrix4 retVal;
		// row 0
		retVal.mat[0][0] =
			a.mat[0][0] * b.mat[0][0] +
			a.mat[0][1] * b.mat[1][0] +
			a.mat[0][2] * b.mat[2][0] +
			a.mat[0][3] * b.mat[3][0];

		retVal.mat[0][1] =
			a.mat[0][0] * b.mat[0][1] +
			a.mat[0][1] * b.mat[1][1] +
			a.mat[0][2] * b.mat[2][1] +
			a.mat[0][3] * b.mat[3][1];

		retVal.mat[0][2] =
			a.mat[0][0] * b.mat[0][2] +
			a.mat[0][1] * b.mat[1][2] +
			a.mat[0][2] * b.mat[2][2] +
			a.mat[0][3] * b.mat[3][2];

		retVal.mat[0][3] =
			a.mat[0][0] * b.mat[0][3] +
			a.mat[0][1] * b.mat[1][3] +
			a.mat[0][2] * b.mat[2][3] +
			a.mat[0][3] * b.mat[3][3];

		// row 1
		retVal.mat[1][0] =
			a.mat[1][0] * b.mat[0][0] +
			a.mat[1][1] * b.mat[1][0] +
			a.mat[1][2] * b.mat[2][0] +
			a.mat[1][3] * b.mat[3][0];

		retVal.mat[1][1] =
			a.mat[1][0] * b.mat[0][1] +
			a.mat[1][1] * b.mat[1][1] +
			a.mat[1][2] * b.mat[2][1] +
			a.mat[1][3] * b.mat[3][1];

		retVal.mat[1][2] =
			a.mat[1][0] * b.mat[0][2] +
			a.mat[1][1] * b.mat[1][2] +
			a.mat[1][2] * b.mat[2][2] +
			a.mat[1][3] * b.mat[3][2];

		retVal.mat[1][3] =
			a.mat[1][0] * b.mat[0][3] +
			a.mat[1][1] * b.mat[1][3] +
			a.mat[1][2] * b.mat[2][3] +
			a.mat[1][3] * b.mat[3][3];

		// row 2
		retVal.mat[2][0] =
			a.mat[2][0] * b.mat[0][0] +
			a.mat[2][1] * b.mat[1][0] +
			a.mat[2][2] * b.mat[2][0] +
			a.mat[2][3] * b.mat[3][0];

		retVal.mat[2][1] =
			a.mat[2][0] * b.mat[0][1] +
			a.mat[2][1] * b.mat[1][1] +
			a.mat[2][2] * b.mat[2][1] +
			a.mat[2][3] * b.mat[3][1];

		retVal.mat[2][2] =
			a.mat[2][0] * b.mat[0][2] +
			a.mat[2][1] * b.mat[1][2] +
			a.mat[2][2] * b.mat[2][2] +
			a.mat[2][3] * b.mat[3][2];

		retVal.mat[2][3] =
			a.mat[2][0] * b.mat[0][3] +
			a.mat[2][1] * b.mat[1][3] +
			a.mat[2][2] * b.mat[2][3] +
			a.mat[2][3] * b.mat[3][3];

		// row 3
		retVal.mat[3][0] =
			a.mat[3][0] * b.mat[0][0] +
			a.mat[3][1] * b.mat[1][0] +
			a.mat[3][2] * b.mat[2][0] +
			a.mat[3][3] * b.mat[3][0];

		retVal.mat[3][1] =
			a.mat[3][0] * b.mat[0][1] +
			a.mat[3][1] * b.mat[1][1] +
			a.mat[3][2] * b.mat[2][1] +
			a.mat[3][3] * b.mat[3][1];

		retVal.mat[3][2] =
			a.mat[3][0] * b.mat[0][2] +
			a.mat[3][1] * b.mat[1][2] +
			a.mat[3][2] * b.mat[2][2] +
			a.mat[3][3] * b.mat[3][2];

		retVal.mat[3][3] =
			a.mat[3][0] * b.mat[0][3] +
			a.mat[3][1] * b.mat[1][3] +
			a.mat[3][2] * b.mat[2][3] +
			a.mat[3][3] * b.mat[3][3];

		return retVal;
	} // matrix multiplication (a * b)

	Matrix4& operator*=(const Matrix4& right) {
		*this = *this * right;
		return *this;
	}

	void Invert(); // invert the matrix - super slow

	Vector3 GetTranslation() const {
		return Vector3(mat[3][0], mat[3][1], mat[3][2]);
	} // get the translation component of the matrix

	Vector3 GetXAxis() const {
		return Vector3::Normalize(Vector3(mat[0][0], mat[0][1], mat[0][2]));
	} // get thr X axis of the matrix (forward)

	Vector3 GetYAxis() const {
		return Vector3::Normalize(Vector3(mat[1][0], mat[1][1], mat[1][2]));
	} // get the Y axis of the matrix (left)

	Vector3 GetZAxis() const {
		return Vector3::Normalize(Vector3(mat[2][0], mat[2][1], mat[2][2]));
	} // get the Z axis of the matrix (up)

	Vector3 GetScale() const {
		Vector3 retVal;
		retVal.x = Vector3(mat[0][0], mat[0][1], mat[0][2]).Length();
		retVal.y = Vector3(mat[1][0], mat[1][1], mat[1][2]).Length();
		retVal.z = Vector3(mat[2][0], mat[2][1], mat[2][2]).Length();
		return retVal;
	} // extract the scale component from the matrix

	static Matrix4 CreateScale(float xScale, float yScale, float zScale) {
		float temp[4][4] = {
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, zScale, 0.0f },
			{ 0.0f, 0.0f, 0.0, 1.0f },
		};
		return Matrix4(temp);
	} // create a scale matrix with x, y and z scales

	static Matrix4 CreateScale(const Vector3& scaleVector) {
		return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
	} // create a scale matrix with vector

	static Matrix4 CreateScale(float scale) {
		return CreateScale(scale, scale, scale);
	} // create a scale matrix with a uniform factor

	static Matrix4 CreateRotationX(float theta) {
		float temp[4][4] = {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ 0.0f, -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	} // rotation about x-axis

	static Matrix4 CreateRotationY(float theta) {
		float temp[4][4] = {
			{ Math::Cos(theta), 0.0f, -Math::Sin(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ Math::Sin(theta), 0.0f, Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	} // rotation about y-axis

	static Matrix4 CreateRotationZ(float theta) {
		float temp[4][4] = {
			{ Math::Cos(theta), Math::Sin(theta), 0.0f, 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	} // rotation about z-axis

	static Matrix4 CreateFromQuaternion(const class Quaternion& q); // create a rotation matrix from a quaternion

	static Matrix4 CreateTranslation(const Vector3& trans) {
		float temp[4][4] = {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
		Vector3 zaxis = Vector3::Normalize(target - eye);
		Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
		Vector3 yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
		Vector3 trans;
		trans.x = -Vector3::Dot(xaxis, eye);
		trans.y = -Vector3::Dot(yaxis, eye);
		trans.z = -Vector3::Dot(zaxis, eye);

		float temp[4][4] = {
			{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
			{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
			{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateOrtho(float width, float height, float near, float far) {
		float temp[4][4] = {
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
			{ 0.0f, 0.0f, near / (near - far), 1.0f },
		};
		return Matrix4(temp);
	}

	static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far) {
		float yScale = Math::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		float temp[4][4] = {
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, far / (far - near), 1.0f },
			{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateSimpleViewProj(float width, float height) {
		float temp[4][4] = {
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		};
		return Matrix4(temp);
	} // create "simple" view-projection matrix from chapter 6

	static const Matrix4 Identity;
};

class Quaternion {
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion() {
		*this = Quaternion::Identity;
	}

	explicit Quaternion(float inX, float inY, float inZ, float inW) {
		Set(inX, inY, inZ, inW);
	}

	explicit Quaternion(const Vector3& axis, float angle) {
		float scalar = Math::Sin(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = Math::Cos(angle / 2.0f);
	}

	void Set(float inX, float inY, float inZ, float inW) {
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	void Conjugate() {
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	float LengthSq() const {
		return (x * x + y * y + z * z + w * w);
	}

	float Length() const {
		return Math::Sqrt(LengthSq());
	}

	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	static Quaternion Normalize(const Quaternion& q) {
		Quaternion retVal = q;
		retVal.Normalize();
		return retVal;
	} // nomalize with provided quaternion

	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f) {
		Quaternion retVal;
		retVal.x = Math::Lerp(a.x, b.x, f);
		retVal.y = Math::Lerp(a.y, b.y, f);
		retVal.z = Math::Lerp(a.z, b.z, f);
		retVal.w = Math::Lerp(a.w, b.w, f);
		retVal.Normalize();
		return retVal;
	}

	static float Dot(const Quaternion& a, const Quaternion& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f)
	{
		float rawCosm = Quaternion::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}

		float scale0, scale1;

		if (cosom < 0.9999f)
		{
			const float omega = Math::Acos(cosom);
			const float invSin = 1.f / Math::Sin(omega);
			scale0 = Math::Sin((1.f - f) * omega) * invSin;
			scale1 = Math::Sin(f * omega) * invSin;
		}
		else
		{
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}

		Quaternion retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		retVal.Normalize();
		return retVal;
	} // spherical linear interpolation

	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p) {
		Quaternion retVal;

		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

		return retVal;
	} // rotation by q FOLLOWED BY p (concentrate)

	static const Quaternion Identity;
};

namespace Color
{
	static const Vector3 Black(0.0f, 0.0f, 0.0f);
	static const Vector3 White(1.0f, 1.0f, 1.0f);
	static const Vector3 Red(1.0f, 0.0f, 0.0f);
	static const Vector3 Green(0.0f, 1.0f, 0.0f);
	static const Vector3 Blue(0.0f, 0.0f, 1.0f);
	static const Vector3 Yellow(1.0f, 1.0f, 0.0f);
	static const Vector3 LightYellow(1.0f, 1.0f, 0.88f);
	static const Vector3 LightBlue(0.68f, 0.85f, 0.9f);
	static const Vector3 LightPink(1.0f, 0.71f, 0.76f);
	static const Vector3 LightGreen(0.56f, 0.93f, 0.56f);
}