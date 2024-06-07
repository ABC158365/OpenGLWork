#include "CVector.h"
#include "Cmath.h"
namespace Cvector {
	CVector rotateAroundAxis(float degree, CVector axis, CVector vec) {
		float cosTheta = cos(Cmath::radians(degree));
		float sinTheta = sin(Cmath::radians(degree));

		// 归一化轴向量
		float length = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
		axis.x /= length;
		axis.y /= length;
		axis.z /= length;

		// 计算旋转矩阵
		float rotationMatrix[3][3] = {
			{cosTheta + axis.x * axis.x * (1 - cosTheta), axis.x * axis.y * (1 - cosTheta) - axis.z * sinTheta, axis.x * axis.z * (1 - cosTheta) + axis.y * sinTheta},
			{axis.y * axis.x * (1 - cosTheta) + axis.z * sinTheta, cosTheta + axis.y * axis.y * (1 - cosTheta), axis.y * axis.z * (1 - cosTheta) - axis.x * sinTheta},
			{axis.z * axis.x * (1 - cosTheta) - axis.y * sinTheta, axis.z * axis.y * (1 - cosTheta) + axis.x * sinTheta, cosTheta + axis.z * axis.z * (1 - cosTheta)}
		};

		// 应用旋转矩阵到向量
		Cvector::CVector rotatedVector;
		rotatedVector.x = rotationMatrix[0][0] * vec.x + rotationMatrix[0][1] * vec.y + rotationMatrix[0][2] * vec.z;
		rotatedVector.y = rotationMatrix[1][0] * vec.x + rotationMatrix[1][1] * vec.y + rotationMatrix[1][2] * vec.z;
		rotatedVector.z = rotationMatrix[2][0] * vec.x + rotationMatrix[2][1] * vec.y + rotationMatrix[2][2] * vec.z;

		return rotatedVector;
	}


	void CVector::parse2Vector(const std::string& line, std::vector<Vec3>& vecs) {
		std::vector<float> numbers;

		std::stringstream ss(line);
		std::string token;


		while (std::getline(ss, token, '\t')) {
			if (!token.empty()) {
				std::stringstream ss1(token);
				std::string token1;
				while (getline(ss1, token1, ',')) {
					numbers.push_back(stof(token1));
				}
			}
		}
		if (numbers.size() < 6) {
			std::cout << "warning::解析到不足的向量组" << std::endl;
			numbers.resize(6);
		}
		vecs.resize(2);
		vecs[0].x = numbers[0];
		vecs[0].y = numbers[1];
		vecs[0].z = numbers[2];
		vecs[1].x = numbers[3];
		vecs[1].y = numbers[4];
		vecs[1].z = numbers[5];

		//std::cout << res[0].x << "," << res[0].y << "," << res[0].z<<std::endl;
		//std::cout << res[1].x << "," << res[1].y << "," << res[1].z << std::endl;

	}

	void CVector::parse1Vector(const std::string& line, std::vector<Vec3>& vec) {
		std::vector<float> numbers;
		std::stringstream ss(line);
		std::string token;


		while (std::getline(ss, token, ',')) {
			if (!token.empty()) {
				numbers.push_back(stof(token));
			}
		}
		if (numbers.size() < 3) {
			std::cout << "warning::解析到不足的向量" << std::endl;
			numbers.resize(3);
		}
		vec.resize(1);
		vec[0].x = numbers[0];
		vec[0].y = numbers[1];
		vec[0].z = numbers[2];


		//std::cout << res[0].x << "," << res[0].y << "," << res[0].z<<std::endl;
		//std::cout << res[1].x << "," << res[1].y << "," << res[1].z << std::endl;

	}


	float CVector::operator[](int index)const{
		if (index == 0) {
			return x;
		}
		else if (index == 1) {
			return y;
		}
		else if (index == 2) {
			return z;
		}
		else {
			throw std::out_of_range("Index out of range");
		}
	}

	float& CVector::operator[](int index){
		if (index == 0) {
			return x;
		}
		else if (index == 1) {
			return y;
		}
		else if (index == 2) {
			return z;
		}
		else {
			throw std::out_of_range("Index out of range");
		}
	}


	float& CVector4::operator[](int index) {
		if (index == 0) {
			return x;
		}
		else if (index == 1) {
			return y;
		}
		else if (index == 2) {
			return z;
		}
		else if (index == 3) {
			return w;
		}
		else {
			throw std::out_of_range("Index out of range");
		}
	}

	float DotProduct(const CVector4& Vec1, const CVector4& Vec2) {
		return Vec1.x * Vec2.x + Vec1.y * Vec2.y + Vec1.z * Vec2.z + Vec1.w * Vec2.w;
	}

	// 计算三维向量的叉乘
	CVector4 CrossProduct(const CVector4& Vec1, const CVector4& Vec2) {
		return CVector4(Vec1.y * Vec2.z - Vec1.z * Vec2.y,
			Vec1.z * Vec2.x - Vec1.x * Vec2.z,
			Vec1.x * Vec2.y - Vec1.y * Vec2.x, 0);
	}

	// 计算三个向量的混合乘积（用于正交化）
	CVector4 CrossProduct(const CVector4& Vec1, const CVector4& Vec2, const CVector4& Vec3) {
		return CVector4(DotProduct(Vec1, CrossProduct(Vec2, Vec3)),
			DotProduct(Vec2, CrossProduct(Vec3, Vec1)),
			DotProduct(Vec3, CrossProduct(Vec1, Vec2)), 0);
	}

	// 将向量归一化
	CVector4 Normalize(const CVector4& Vec) {
		float Length = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z + Vec.w * Vec.w);
		if (Length == 0) return CVector4(0, 0, 0, 0);
		return CVector4(Vec.x / Length, Vec.y / Length, Vec.z / Length, Vec.w / Length);
	}
}


