#pragma once
#include "CVector.h"
namespace Cmatrix {
	using namespace Cvector;

	class Mat4x4 {
	
	private:
		
	public:
		CVector4 value[4];
		Mat4x4() {
			value[0] = CVector4(1, 0, 0, 0);
			value[1] = CVector4(0, 1, 0, 0);
			value[2] = CVector4(0, 0, 1, 0);
			value[3] = CVector4(0, 0, 0, 1);
		};

		Mat4x4(float n) {
			value[0] = CVector4(n, n, n, n);
			value[1] = CVector4(n, n, n, n);
			value[2] = CVector4(n, n, n, n);
			value[3] = CVector4(n, n, n, n);
		};

		Mat4x4(CVector4 a, CVector4 b, CVector4 c, CVector4 d) {
			value[0] = a;
			value[1] = b;
			value[2] = c;
			value[3] = d;
		};

		Mat4x4(float a1, float a2, float a3, float a4,
			float a5, float a6, float a7, float a8,
			float a9, float a10, float a11, float a12,
			float a13, float a14, float a15,float a16) {
			value[0] = CVector4(a1, a2, a3, a4);
			value[1] = CVector4(a5, a6, a7, a8);
			value[2] = CVector4(a9, a10, a11, a12);
			value[3] = CVector4(a13, a14, a15, a16);
		}

		CVector4& operator[](int n) {
			return value[n];
		}

		CVector4 operator [](int n)const {
			return value[n];
		}

		float* value_ptr() {
			return &value[0][0];
		}

		CVector4 operator* (CVector4& other) {
			CVector4 v1 = value[0] * other.x;
			CVector4 v2 = value[1] * other.y;
			CVector4 add1 = v1 + v2;

			CVector4 v3 = value[2] * other.z;
			CVector4 v4 = value[3] * other.w;
			CVector4 add2 = v3 + v4;


			return CVector4(add1 + add2);
		};

		CVector4 operator* (const CVector4& other) {
			CVector4 v1 = value[0] * other.x;
			CVector4 v2 = value[1] * other.y;
			CVector4 add1 = v1 + v2;

			CVector4 v3 = value[2] * other.z;
			CVector4 v4 = value[3] * other.w;
			CVector4 add2 = v3 + v4;


			return CVector4(add1 + add2);
		};

		Mat4x4 operator* (float other) {
			return Mat4x4(value[0] * other, value[1] * other, value[2] * other, value[3] * other);
		};

		Mat4x4 operator* (Mat4x4& other) {
			Mat4x4 result;
			CVector4 SrcA0 = value[0];
			CVector4 SrcA1 = value[1];
			CVector4 SrcA2 = value[2];
			CVector4 SrcA3 = value[3];

			CVector4 SrcB0 = other[0];
			CVector4 SrcB1 = other[1];
			CVector4 SrcB2 = other[2];
			CVector4 SrcB3 = other[3];

			Mat4x4 Result;
			Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
			Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
			Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
			Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
			return Result;

		}

		Mat4x4 operator* (const Mat4x4& other) {
			Mat4x4 result;
			CVector4 SrcA0 = value[0];
			CVector4 SrcA1 = value[1];
			CVector4 SrcA2 = value[2];
			CVector4 SrcA3 = value[3];

			CVector4 SrcB0 = other[0];
			CVector4 SrcB1 = other[1];
			CVector4 SrcB2 = other[2];
			CVector4 SrcB3 = other[3];

			Mat4x4 Result;
			Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
			Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
			Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
			Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
			return Result;

		}

		
		
	};

	Mat4x4 perspective(float fovy, float aspect, float zNear, float zFar);
	Mat4x4 lookAt(CVector const& eye, CVector const& center, CVector const& up);
	Mat4x4 rotate(const Mat4x4& m, float angle, CVector const& v);
	Mat4x4 scale(Mat4x4 const& m, CVector const& v);
	Mat4x4 translate(Mat4x4 const& m, CVector const& v);	
	Mat4x4 inverse(Mat4x4 const& m);

	Mat4x4 OrthogonalizeMatrix(const Mat4x4& Matrix);
	
}

