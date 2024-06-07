#include "CMatrix.h"
#include <cmath>
namespace Cmatrix {

	Mat4x4 perspective(float fovy, float aspect, float zNear, float zFar) {
		

		float const tanHalfFovy = tan(fovy / static_cast<float>(2));

		Mat4x4 Result(static_cast<float>(0));
		Result[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
		Result[1][1] = static_cast<float>(1) / (tanHalfFovy);
		Result[2][2] = -(zFar + zNear) / (zFar - zNear);
		Result[2][3] = -static_cast<float>(1);
		Result[3][2] = -(static_cast<float>(2) * zFar * zNear) / (zFar - zNear);

		return Result;
	}


	Mat4x4 lookAt(CVector const& eye, CVector const& center, CVector const& up)
	{
		CVector  f = center - eye;
		f.Normalize();
		CVector  s = f.crossMul(up);
		s.Normalize();
		CVector u = s.crossMul(f);

		Mat4x4 Result;
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] = -f.x;
		Result[1][2] = -f.y;
		Result[2][2] = -f.z;
		Result[3][0] = -s.dotMul(eye);
		Result[3][1] = -u.dotMul(eye);
		Result[3][2] = f.dotMul(eye);
		return Result;
	}

	Mat4x4 OrthogonalizeMatrix(const Mat4x4& Matrix) {
		Mat4x4 Result = Matrix;

		// 正交化第一个向量
		Result.value[0] = Normalize(Result.value[0]);

		// 计算第二个向量
		Result.value[1] -= DotProduct(Result.value[1], Result.value[0]) * Result.value[0];
		Result.value[1] = Normalize(Result.value[1]);

		// 计算第三个向量
		Result.value[2] -= DotProduct(Result.value[2], Result.value[0]) * Result.value[0];
		Result.value[2] -= DotProduct(Result.value[2], Result.value[1]) * Result.value[1];
		Result.value[2] = Normalize(Result.value[2]);

		// 计算第四个向量
		Result.value[3] -= DotProduct(Result.value[3], Result.value[0]) * Result.value[0];
		Result.value[3] -= DotProduct(Result.value[3], Result.value[1]) * Result.value[1];
		Result.value[3] -= DotProduct(Result.value[3], Result.value[2]) * Result.value[2];
		Result.value[3] = Normalize(Result.value[3]);

		return Result;

}
	

	Mat4x4 rotate(const Mat4x4& m, float angle, CVector const& v)
	{
		float const a = angle;
		float const c = cos(a);
		float const s = sin(a);

		CVector axis = (CVector::normalize(v));
		CVector temp(axis * (float(1) - c));

		Mat4x4 Rotate;
		Rotate[0][0] = c + temp[0] * axis[0];
		Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
		Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

		Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
		Rotate[1][1] = c + temp[1] * axis[1];
		Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

		Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
		Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
		Rotate[2][2] = c + temp[2] * axis[2];

		Mat4x4 Result;
		Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
		Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
		Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
		Result[3] = m[3];
		return Result;
	}

	Mat4x4 scale(Mat4x4 const& m, CVector const& v)
	{
		Mat4x4 Result;
		Result[0] = m[0] * v[0];
		Result[1] = m[1] * v[1];
		Result[2] = m[2] * v[2];
		Result[3] = m[3];
		return Result;
	}

	Mat4x4 translate(Mat4x4 const& m, CVector const& v)
	{
		Mat4x4 Result(m);
		Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return Result;
	}


	Mat4x4 inverse(Mat4x4 const& m)
	{
		float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Cvector::CVector4 Fac0(Coef00, Coef00, Coef02, Coef03);
		Cvector::CVector4 Fac1(Coef04, Coef04, Coef06, Coef07);
		Cvector::CVector4 Fac2(Coef08, Coef08, Coef10, Coef11);
		Cvector::CVector4 Fac3(Coef12, Coef12, Coef14, Coef15);
		Cvector::CVector4 Fac4(Coef16, Coef16, Coef18, Coef19);
		Cvector::CVector4 Fac5(Coef20, Coef20, Coef22, Coef23);

		Cvector::CVector4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		Cvector::CVector4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		Cvector::CVector4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		Cvector::CVector4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		Cvector::CVector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		Cvector::CVector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		Cvector::CVector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);	
		Cvector::CVector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);
		Cvector::CVector4 SignA(+1, -1, +1, -1);
		Cvector::CVector4 SignB(-1, +1, -1, +1);
		Mat4x4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		Cvector::CVector4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		Cvector::CVector4 Dot0(m[0] * Row0);
		float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		float OneOverDeterminant = static_cast<float>(1) / Dot1;

		return Inverse * OneOverDeterminant;
	}
}