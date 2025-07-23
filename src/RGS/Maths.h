#pragma once
#include <string>

#define PI 3.14159265359
#define EPSILON 1e-5f

namespace RGS {
	struct Vec2
	{
		float X, Y;

		constexpr Vec2()
			: X(0.0f), Y(0.0f){}
		constexpr Vec2(float x, float y)
			: X(x), Y(y){}
	};

	struct Vec3
	{
		float X, Y, Z;

		constexpr Vec3()
			: X(0.0f), Y(0.0f), Z(0.0f){}
		constexpr Vec3(float x, float y, float z)
			: X(x), Y(y), Z(z){}

		operator Vec2() const { return { X, Y }; }
	};

	struct Vec4
	{
		float X, Y, Z, W;

		constexpr Vec4()
			: X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
		constexpr Vec4(float x, float y, float z, float w)
			: X(x), Y(y), Z(z), W(w){}
		constexpr Vec4(const Vec3& vec3, float w)
			: X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(w){}

		operator Vec2() const { return { X,Y }; }
		operator Vec3() const { return { X,Y,Z }; }

		operator std::string() const
		{
			std::string res;
			res += "(";
			res += std::to_string(X);
			res += ",";
			res += std::to_string(Y);
			res += ",";
			res += std::to_string(Z);
			res += ",";
			res += std::to_string(W);
			res += ")";
			return res;
		}

	};

	struct Mat4
	{
		float M[4][4];

		Mat4()
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					M[i][j] = 0.0f;
				}
			}
		}

		Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);

		operator const std::string() const
		{
			std::string res;
			res += "(";
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					res += std::to_string(M[i][j]);
					res += (i == 3 && j == 3) ? ")" : ",";
				}
				return res;
			}
		}
	};

	float Dot(const Vec3& left, const Vec3& right);
	Vec3 Cross(const Vec3& left, const Vec3& right);

	Vec2 operator+ (const Vec2& left, const Vec2& right);
	Vec2 operator* (const float left, const Vec2& right);
	Vec2 operator* (const Vec2& left, const float right);
	Vec2 operator- (const Vec2& left, const Vec2& right);
	Vec2 operator/ (const Vec2& left, const float right);
	Vec3 operator+ (const Vec3& left, const Vec3& right);
	Vec3 operator* (const float left, const Vec3& right);
	Vec3 operator* (const Vec3& left, const float right);
	Vec3 operator- (const Vec3& left, const Vec3& right);
	Vec3 operator/ (const Vec3& left, const float right);
	Vec4 operator+ (const Vec4& left, const Vec4& right);
	Vec4 operator- (const Vec4& left, const Vec4& right);
	Vec4 operator* (const float left, const Vec4& right);
	Vec4 operator* (const Vec4& left, const float right);
	Vec4 operator/ (const Vec4& left, const float right);

	Vec4 operator* (const Mat4& mat4, const Vec4& vec4);
	Mat4 operator* (const Mat4& left, const Mat4& right);
	Mat4& operator*= (Mat4& left, const Mat4& right);

	Mat4 Mat4Identity();
	Mat4 Mat4Scale(float sx, float sy, float sz);
	Mat4 Mat4Translate(float tx, float ty, float tz);
	Mat4 Mat4RotateX(float angle);
	Mat4 Mat4RotateY(float angle);
	Mat4 Mat4RotateZ(float angle);
	Mat4 Mat4Perspective(float fovy, float aspect, float near, float far);
	
	float Lerp(const float start, const float end, const float t);
	Vec3 Lerp(const Vec3& start, const Vec3& end, const float t);
	float Clamp(const float val, const float min, const float max);

	unsigned char Float2UChar(const float f);
	float UChar2Float(const unsigned char c);
}