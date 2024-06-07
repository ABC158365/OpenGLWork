#pragma once
#include "CVector.h"
#include "CMatrix.h"
#include "Cmath.h"
namespace CRotator {
	class CRotator
	{

	public:
		float Pitch;
		float Yaw;
		float Roll;

		CRotator(float p, float y, float r) {
			Pitch = p;
			Yaw = y;
			Roll = r;
		}

		CRotator() {
			Pitch = 0;
			Yaw = 0;
			Roll = 0;
		}

	};
	static CRotator NormalizeRotator(const CRotator& Rotator) {
		CRotator Result;
		Result.Pitch = fmodf(Rotator.Pitch + 180.0f, 360.0f) - 180.0f;
		Result.Yaw = fmodf(Rotator.Yaw + 180.0f, 360.0f) + 180.0f;
		Result.Roll = fmodf(Rotator.Roll + 180.0f, 360.0f) - 180.0f;
		return Result;
	}

	inline Cmatrix::Mat4x4 getRotationMatrix(float pitch, float yaw, float roll) {
		yaw = -yaw;
		float cy = cos(pitch);
		float sy = sin(pitch);
		float cp = cos(yaw);
		float sp = sin(yaw);
		float cr = cos(roll);
		float sr = sin(roll);
		return Cmatrix::Mat4x4(cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr, 0.f,
			sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr, 0.f,
			-sp, cp * sr, cp * cr, 0.f,
			0.f, 0.f, 0.f, 1.f);

	}
		inline Cmatrix::Mat4x4 getRotationMatrix(CRotator r) {
			float yaw = r.Yaw;
			float pitch = r.Pitch;
			float roll = r.Roll;

			yaw = -yaw;
			float cy = cos(pitch);
			float sy = sin(pitch);
			float cp = cos(yaw);
			float sp = sin(yaw);
			float cr = cos(roll);
			float sr = sin(roll);
			return Cmatrix::Mat4x4(cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr, 0.f,
				sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr, 0.f,
				-sp, cp * sr, cp * cr, 0.f,
				0.f, 0.f, 0.f, 1.f);
	}


		inline Cvector::CVector getForwardVector(CRotator r) {
			Cvector::CVector4 res = Cvector::CVector4(1, 0, 0, 0);
			res = getRotationMatrix(r) * res;
			return Cvector::CVector(res.x, res.y, res.z);
			

		}


		static CRotator MatrixToRotator(const Cmatrix::Mat4x4& Matrix) {
			CRotator Rotator;
			Rotator.Pitch = asinf(-Matrix.value[2].x);
			float cosPitch = cosf(Rotator.Pitch);
			if (fabsf(cosPitch) > 0.0001f) {
				Rotator.Yaw = atan2f(Matrix.value[2].y, Matrix.value[2].z);
				Rotator.Roll = atan2f(Matrix.value[1].x, Matrix.value[0].x);
			}
			else {
				Rotator.Yaw = 0;
				Rotator.Roll = atan2f(-Matrix.value[0].y, Matrix.value[1].y);
			}
			// 将弧度转换为角度并确保在 -180 到 180 度之间
			Rotator.Pitch = Rotator.Pitch * (180.0f / Cmath::pi<float>());
			Rotator.Yaw = Rotator.Yaw * (180.0f / Cmath::pi<float>());
			Rotator.Roll = Rotator.Roll * (180.0f / Cmath::pi<float>());
			if (Rotator.Pitch < -180.0f) Rotator.Pitch += 360.0f;
			if (Rotator.Pitch > 180.0f) Rotator.Pitch -= 360.0f;
			if (Rotator.Yaw < -180.0f) Rotator.Yaw += 360.0f;
			if (Rotator.Yaw > 180.0f) Rotator.Yaw -= 360.0f;
			if (Rotator.Roll < -180.0f) Rotator.Roll += 360.0f;
			if (Rotator.Roll > 180.0f) Rotator.Roll -= 360.0f;


			return Rotator;
		}

		
}


