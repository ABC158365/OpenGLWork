#include "Camera.h"
#include <glm.hpp>
#include <gtc/matrix_inverse.hpp>
void Camera::moveAroundCenter(float degree, Cvector::CVector center, Cvector::CVector axis) {
	float length = Cvector::CVector(Position - center).len();
	Cvector::CVector eyeV = Cvector::CVector(Position - center);
	eyeV.Normalize();
	Cvector::CVector newEye = Cvector::rotateAroundAxis(degree, axis, eyeV);
	newEye = newEye * length + center;
	tempPosition = newEye;
	viewMat =  Cmatrix::lookAt(newEye, center, Cvector::CVector(0, 1, 0));

	//viewMat = Cmatrix::lookAt(Position, Position + Cfront, WorldUp);
}

Camera::Camera(float threhold, Cmatrix::Mat4x4 projMat, Cvector::CVector position, Cvector::CVector up ) {
	Position = position;
	targetPos = position;
	WorldUp = up;
	WorldFront = Cvector::CVector(0.0f, 0.0f, -1.0f);
	WorldRight = Cvector::CVector(1.0f, 0.0f, 0.0f);
	Cfront = Cvector::CVector(0.0f, 0.0f, -1.0f);
	Cright = Cvector::CVector(1.0f, 0.0f, 0.0f);
	Threshold = threhold;
	projectionMat = projMat;
}

void Camera::update(float deltatime) {
	if (abs(targetPos.z - Position.z) > 0.00001) {
		Position.z = interpolate(10, Threshold, deltatime, Position.z, targetPos.z); //更新当前z插值
		//原位置+插值向量
	}
	if (abs(targetPos.x - Position.x) > 0.001) {
		Position.x = interpolate(10, Threshold, deltatime, Position.x, targetPos.x); //更新当前x插值
		//原位置+插值向量
	}
	if (abs(targetPos.y - Position.y) > 0.001) {
		Position.y = interpolate(10, Threshold, deltatime, Position.y, targetPos.y); //更新当前x插值
		//原位置+插值向量
	}
	Cvector::CVector front;
	front.x = -sin(Cmath::radians(yaw)) * cos(Cmath::radians(pitch));
	front.z = -cos(Cmath::radians(yaw)) * cos(Cmath::radians(pitch));
	front.y = sin(Cmath::radians(pitch));
	Cfront = Cvector::CVector::normalize(front);

	Cmatrix::Mat4x4 rot = Cmatrix::rotate(Cmatrix::Mat4x4(), roll, Cfront);
	Cvector::CVector4 tmp = rot * Cvector::CVector4(0, 1, 0, 1);
	Up = Cvector::CVector(tmp.x, tmp.y, tmp.z);
	Cright = Cfront.crossMul(Up);

}

void Camera::GetCamStartAndDir(Cvector::CVector& start, Cvector::CVector& dir, float screePosx, float screenPosy) {
	// Convert screen coordinates to NDC (Normalized Device Coordinates)
	float ndcX = 2.0f * screePosx / 1280 - 1.0f;
	float ndcY = 1.0f - 2.0f * screenPosy / 720;


	// Create a homogeneous point in NDC
	Cvector::CVector4 rayClip(ndcX, ndcY, -1.0f, 1.0f);	
	glm::vec4 rayClip1(ndcX, ndcY, -1.0f, 1.0f);

	Cmatrix::Mat4x4 mat = projectionMat * GetViewMatrix(true);
	// Compute inverse view-projection matrix

	Cmatrix::Mat4x4 invViewProj = Cmatrix::inverse(mat);

	// Transform ray from NDC to world space
	Cvector::CVector4 rayWorld = invViewProj * rayClip;
	
	


	rayWorld = rayWorld / rayWorld.w; // Normalize homogeneous coordinates
	

	start = Cvector::CVector(rayWorld.x, rayWorld.y, rayWorld.z);
	if (disableCameraMovement) {
	dir = start - tempPosition;
	}else
	dir = start - Position;
	

}