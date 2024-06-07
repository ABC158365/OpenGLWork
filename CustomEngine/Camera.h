#pragma once
#include <glad/glad.h>

#include <vector>
#include <iostream>
#include "CMatrix.h"
#include "Cmath.h"

//namespace {
//	glm::mat4x4 getMatfromCmat(const Cmatrix::Mat4x4& cmat) {
//		return glm::mat4x4(cmat[0][0], cmat[0][1], cmat[0][2], cmat[0][3],
//			cmat[1][0], cmat[1][1], cmat[1][2], cmat[1][3],
//			cmat[2][0], cmat[2][1], cmat[2][2], cmat[2][3],
//			cmat[3][0], cmat[3][1], cmat[3][2], cmat[3][3]
//			);
//	}
//}

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	TSPEEDUP,
	TSPEEDDOWN,
	RSPEEDUP,
	RSPEEDDOWN,
};

class Camera
{
public:
	float xpos = 0, ypos = 0;
	Cvector::CVector Position, targetPos, tempPosition;
	Cvector::CVector WorldFront;
	Cvector::CVector Up;
	Cvector::CVector WorldRight;
	Cvector::CVector WorldUp;
	Cvector::CVector Cfront, Cright;

	float ts=0.1, rs = 0.1;
	float yaw=180, pitch=0., roll=0.;
	float Threshold;
	float totalFront=0, totalRight=0, currentFront=0, currentRight=0;
	bool disableCameraMovement = false;
	bool CameraState = true;

	void moveAroundCenter(float degree, Cvector::CVector center, Cvector::CVector axis);

	Camera(float threhold, Cmatrix::Mat4x4 projMat, Cvector::CVector position = Cvector::CVector(0.0f, 5.0f, -1.0f), Cvector::CVector up = Cvector::CVector(0.0f, 1.0f, 0.0f));

	void update(float deltatime);

	void updateViewMatrix() {
		viewMat = Cmatrix::lookAt(Position, Position + Cfront, Up);
	}

	Cmatrix::Mat4x4 GetViewMatrix(bool ForceUpdate)

	{	
		if (ForceUpdate && !disableCameraMovement)updateViewMatrix();
		return viewMat;
	}

	Cvector::CVector GetPos() {
		return Position;
	}

	void ProcessKeyboard(Camera_Movement direction) {
		if (direction == FORWARD) {
			
			targetPos = targetPos + Cfront * ts;
		}
		if (direction == BACKWARD) {
			targetPos = targetPos - Cfront * ts;
		}
		if (direction == RIGHT) {
			targetPos = targetPos + Cright * ts;
		}
		if (direction == LEFT) {
			targetPos = targetPos - Cright *ts;
		}
		if (direction == UP) {
			targetPos.y += ts;
		}
		if (direction == DOWN) {
			targetPos.y -= ts;
		}
		if (direction == TSPEEDUP) {
			ts += 0.1;
			ts = std::min<float>(ts, 1);
		}
		if (direction == TSPEEDDOWN) {
			ts -= 0.1;
			ts = std::max<float>(ts, 0.1);
		}
		if (direction == RSPEEDUP) {
			rs += 0.1;
			rs = std::min<float>(rs, 1);
		}
		if (direction == RSPEEDDOWN) {
			rs -= 0.1;
			rs = std::max<float>(rs, 0.1);
		}

	}

	void ProcessAxisMoveMent(float yaw_, float pitch_, float row_) {
		yaw += yaw_;
		pitch += pitch_;
		roll += row_;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

	}

	void ProcessMouseMovement(float xPos, float yPos, GLboolean constrainPitch = true)
	{
		float xoffset = xpos - xPos;
		float yoffset = ypos - yPos;
		
		xpos = xPos;
		ypos = yPos;


		xoffset *= 0.1;
		yoffset *= 0.1;

		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

	}

	Cmatrix::Mat4x4 getProjectionMatrix() {
		return projectionMat;
	};

	void updateProjMaxtrix(Cmatrix::Mat4x4 mat) {
		projectionMat = mat;
	}

	void GetCamStartAndDir(Cvector::CVector& start, Cvector::CVector& dir, float screePosx, float screenPosy);

private:
	Cmatrix::Mat4x4 projectionMat;
	Cmatrix::Mat4x4 viewMat;

	float interpolate(float interpSpeed, float threshold, float deltatime, float current, float total) {
		float dist = total-current;
		if (abs(dist) < threshold)current = total;
		return  current + dist * interpSpeed * deltatime;
	}
};

