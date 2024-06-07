#pragma once
#include "CVector.h"
#include "CRotator.h"


namespace Quaternion {
	class CQuaternion
	{
	public:
		float w, x, y, z;
		CQuaternion(float _w = 1.0f, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : w(_w), x(_x), y(_y), z(_z) {}
		CQuaternion operator *(float a) {
			return CQuaternion(w * a, x * a, y * a, z * a);
		}
		CQuaternion operator +(CQuaternion other) {
			return CQuaternion(w + other.w, x + other.x, y + other.y, z + other.z);
		}
		CQuaternion operator -(CQuaternion other) {
			return CQuaternion(w - other.w, x -other.x, y - other.y, z - other.z);
		}

	};

	 static Cvector::CVector EulerToVector(const CRotator::CRotator& euler) {
		 Cvector::CVector vec;
		 CRotator::NormalizeRotator(euler); 
		 float cosPitch = cosf(euler.Pitch * 0.5f);
		 float sinPitch = sinf(euler.Pitch * 0.5f);
		 float cosYaw = cosf(euler.Yaw * 0.5f);
		 float sinYaw = sinf(euler.Yaw * 0.5f);
		 float cosRoll = cosf(euler.Roll * 0.5f);
		 float sinRoll = sinf(euler.Roll * 0.5f);

		 Cvector::CVector result;
		 result.x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		 result.y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		 result.z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;

		 return result;
	}


	 // 向量转换欧拉角
	 static CRotator::CRotator VectorToEuler(const Cvector::CVector& vector) {
		 CRotator::CRotator result;

		 // 计算Pitch
		 result.Pitch = asinf(vector.y);

		 // 计算Yaw
		 float cosPitch = cosf(result.Pitch);
		 if (cosPitch != 0.0f) {
			 result.Yaw = atan2f(-vector.x / cosPitch, vector.z / cosPitch);
		 }
		 else {
			 // 在Pitch为±90度时，无法确定Yaw，可以将Yaw设为0或π
			 result.Yaw = 0.0f;
		 }

		 // 计算Roll
		 float sinPitch = sinf(result.Pitch);
		 float cosYaw = cosf(result.Yaw);
		 float sinYaw = sinf(result.Yaw);
		 result.Roll = atan2f(cosYaw * vector.z - sinYaw * vector.x, cosYaw * vector.x + sinYaw * vector.z);

		 return result;
	 }

	 static CQuaternion MatrixToQuaternion(const Cmatrix::Mat4x4& Matrix) {
		 float trace = Matrix.value[0].x + Matrix.value[1].y + Matrix.value[2].z;
		 float w, x, y, z;

		 if (trace > 0) {
			 float s = 0.5f / sqrtf(trace + 1.0f);
			 w = 0.25f / s;
			 x = (Matrix.value[1].z - Matrix.value[2].y) * s;
			 y = (Matrix.value[2].x - Matrix.value[0].z) * s;
			 z = (Matrix.value[0].y - Matrix.value[1].x) * s;
		 }
		 else {
			 if (Matrix.value[0].x > Matrix.value[1].y && Matrix.value[0].x > Matrix.value[2].z) {
				 float s = 2.0f * sqrtf(1.0f + Matrix.value[0].x - Matrix.value[1].y - Matrix.value[2].z);
				 w = (Matrix.value[1].z - Matrix.value[2].y) / s;
				 x = 0.25f * s;
				 y = (Matrix.value[1].x + Matrix.value[0].y) / s;
				 z = (Matrix.value[2].x + Matrix.value[0].z) / s;
			 }
			 else if (Matrix.value[1].y > Matrix.value[2].z) {
				 float s = 2.0f * sqrtf(1.0f + Matrix.value[1].y - Matrix.value[0].x - Matrix.value[2].z);
				 w = (Matrix.value[2].x - Matrix.value[0].z) / s;
				 x = (Matrix.value[1].x + Matrix.value[0].y) / s;
				 y = 0.25f * s;
				 z = (Matrix.value[2].y + Matrix.value[1].z) / s;
			 }
			 else {
				 float s = 2.0f * sqrtf(1.0f + Matrix.value[2].z - Matrix.value[0].x - Matrix.value[1].y);
				 w = (Matrix.value[0].y - Matrix.value[1].x) / s;
				 x = (Matrix.value[2].x + Matrix.value[0].z) / s;
				 y = (Matrix.value[1].y + Matrix.value[2].z) / s;
				 z = 0.25f * s;
			 }
		 }

		 return CQuaternion(w, x, y, z);
	 }

	
	 static Cmatrix::Mat4x4 QuaternionToMatrix(const CQuaternion& Quat) {
		 Cmatrix::Mat4x4 Result;
		 float x2 = Quat.x + Quat.x;
		 float y2 = Quat.y + Quat.y;
		 float z2 = Quat.z + Quat.z;
		 float xx2 = Quat.x * x2;
		 float yy2 = Quat.y * y2;
		 float zz2 = Quat.z * z2;
		 float xy2 = Quat.x * y2;
		 float yz2 = Quat.y * z2;
		 float xz2 = Quat.x * z2;
		 float wx2 = Quat.w * x2;
		 float wy2 = Quat.w * y2;
		 float wz2 = Quat.w * z2;

		 Result.value[0] = Cvector::CVector4(1.0f - yy2 - zz2, xy2 - wz2, xz2 + wy2, 0);
		 Result.value[1] = Cvector::CVector4(xy2 + wz2, 1.0f - xx2 - zz2, yz2 - wx2, 0);
		 Result.value[2] = Cvector::CVector4(xz2 - wy2, yz2 + wx2, 1.0f - xx2 - yy2, 0);
		 Result.value[3] = Cvector::CVector4(0, 0, 0, 1);

		 return Result;
	 }

	 static CQuaternion NormalizeQuaternion(const CQuaternion& Quat) {
		 float Length = sqrtf(Quat.w * Quat.w + Quat.x * Quat.x + Quat.y * Quat.y + Quat.z * Quat.z);
		 return CQuaternion(Quat.w / Length, Quat.x / Length, Quat.y / Length, Quat.z / Length);
	 }

	 static CQuaternion MultiplyQuaternions(const CQuaternion& Quat1, const CQuaternion& Quat2) {
		 float w = Quat1.w * Quat2.w - Quat1.x * Quat2.x - Quat1.y * Quat2.y - Quat1.z * Quat2.z;
		 float x = Quat1.w * Quat2.x + Quat1.x * Quat2.w + Quat1.y * Quat2.z - Quat1.z * Quat2.y;
		 float y = Quat1.w * Quat2.y - Quat1.x * Quat2.z + Quat1.y * Quat2.w + Quat1.z * Quat2.x;
		 float z = Quat1.w * Quat2.z + Quat1.x * Quat2.y - Quat1.y * Quat2.x + Quat1.z * Quat2.w;
		 return CQuaternion(w, x, y, z);
	 };
	
	 static CQuaternion SubtractQuaternions(const CQuaternion& Quat1, const CQuaternion& Quat2) {
		 return CQuaternion(Quat1.w - Quat2.w, Quat1.x - Quat2.x, Quat1.y - Quat2.y, Quat1.z - Quat2.z);
	 };

	 static float DotProductQuaternions(const CQuaternion& Quat1, const CQuaternion& Quat2) {
		 return Quat1.w * Quat2.w + Quat1.x * Quat2.x + Quat1.y * Quat2.y + Quat1.z * Quat2.z;
	 };

	 static CQuaternion InverseQuaternion(const CQuaternion& Quat) {
		 float LengthSq = Quat.w * Quat.w + Quat.x * Quat.x + Quat.y * Quat.y + Quat.z * Quat.z;
		 if (LengthSq != 0) {
			 float InvLengthSq = 1.0f / LengthSq;
			 return CQuaternion(Quat.w * InvLengthSq, -Quat.x * InvLengthSq, -Quat.y * InvLengthSq, -Quat.z * InvLengthSq);
		 }
		 else {
			 // 若四元数长度为0，则返回自身
			 return Quat;
		 }
	 };

	 static std::pair<float, Cvector::CVector> QuaternionToAxisAngle(const CQuaternion& Quat) {
		 float Angle = 2.0f * acosf(Quat.w);
		 float S = sqrtf(1.0f - Quat.w * Quat.w);
		 Cvector::CVector Axis;
		 if (S < 0.001f) {
			 // 若S接近于0，则旋转轴是任意的，这里选择(1, 0, 0)
			 Axis = { 1.0f, 0.0f, 0.0f };
		 }
		 else {
			 Axis = { Quat.x / S, Quat.y / S, Quat.z / S };
		 }
		 return std::make_pair(Angle, Axis);
	 };


	 static CQuaternion Slerp(const CQuaternion& Quat1, const CQuaternion& Quat2, float t) {
		 // 如果输入四元数未归一化，先归一化
		 CQuaternion Q1 = NormalizeQuaternion(Quat1);
		 CQuaternion Q2 = NormalizeQuaternion(Quat2);

		 // 计算两个四元数的点乘
		 float Dot = DotProductQuaternions(Q1, Q2);

		 // 如果点乘小于0，则反转其中一个四元数
		 if (Dot < 0.0f) {
			 Q2 = Q2 * -1.0;
			 Dot = -Dot;
		 }

		 // 如果两个四元数非常接近，则线性插值
		 if (Dot > 0.9995f) {
			 return NormalizeQuaternion(Q1 + (Q2 - Q1)*t);
		 }

		 // 使用球面线性插值
		 float Theta_0 = acosf(Dot);
		 float Theta = Theta_0 * t;
		 float Sin_Theta = sinf(Theta);
		 float Sin_Theta_0 = sinf(Theta_0);

		 float s0 = cosf(Theta) - Dot * Sin_Theta / Sin_Theta_0;
		 float s1 = Sin_Theta / Sin_Theta_0;

		 return NormalizeQuaternion( Q1* s0  +   Q2* s1);
	 }
}



