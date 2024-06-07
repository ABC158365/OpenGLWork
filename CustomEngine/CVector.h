#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace Cvector {

	struct  Vec3
	{
		float x;
		float y;
		float z;
	};

	class CVector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		CVector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}

		// жиди
		float& operator[](int index);

		CVector4 operator+(const CVector4& other) const {
			return CVector4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		CVector4 operator-(const CVector4& other) const {
			return CVector4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		CVector4 operator*(const CVector4& other) const {
			return CVector4(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		CVector4 operator*(const float& other) const {
			return CVector4(x * other, y * other, z * other, w * other);
		}

		CVector4& operator-=(CVector4 subtrahend) {
			x -= subtrahend.x;
			y -= subtrahend.y;
			z -= subtrahend.z;
			w -= subtrahend.w;
			return *this;
		}
		friend CVector4 operator*(float multiplier, const CVector4& c) {
			return CVector4(c.x * multiplier, c.y * multiplier, c.z * multiplier, c.w * multiplier);
		};

		CVector4 operator/(const float& other) const {
			return CVector4(x / other, y / other, z / other, w / other);
		}
	};

	class CVector
	{
	public:
		float x;
        float y;
        float z;

        CVector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        // жиди
        float& operator[](int index);
		float operator[](int index)const;

        CVector operator+(const CVector& other) const {
            return CVector(x + other.x, y + other.y, z + other.z);
        }

        CVector operator-(const CVector& other) const {
            return CVector(x - other.x, y - other.y, z - other.z);
        }

		CVector operator*(const CVector& other) const {
			return CVector(x * other.x, y * other.y, z * other.z);
		}

		CVector operator*(const float& other) const {
			return CVector(x * other, y * other, z * other);
		}

		float dotMul(const CVector& n) {
			return x * n.x + y * n.y + z * n.z;
		}

		CVector crossMul(const CVector& n) {
			float newX = y * n.z - z * n.y;
			float newY = z * n.x - x * n.z;
			float newZ = x * n.y - y * n.x;
			return CVector(newX, newY, newZ);

		}


		void Normalize() {
			float mag = len();
			if (mag != 0) {
				x = x / mag;
				y = y / mag;
				z = z / mag;
			}	
		}


		float len()const {
			return sqrt(x * x + y * y + z * z);
		}
		
		float* value_ptr() {
			return &x;
		}

		CVector project(const CVector& n) {
			float res = dotMul(n);
			float l = n.len();
			if (l != 0) {
				res /= l;
			}
			else {
				return CVector(0, 0, 0);
			}
			CVector nVector(n.x, n.y, n.z);
			nVector.Normalize();
			return CVector(nVector.x * res, nVector.y * res, nVector.z * res);
		}

		static void parse2Vector(const std::string& line ,std::vector<Vec3>& vecs);
		static void parse1Vector(const std::string& line, std::vector<Vec3>& vec);

		static CVector normalize(const CVector& v) {
			float mag = v.len();
			if (mag != 0) {
				return CVector(v.x / mag, v.y / mag, v.z / mag);
			}
			else {
				return CVector(0, 0, 0);
			}
		}
		
	};

	CVector rotateAroundAxis(float degree, CVector axis, CVector vec);
	float DotProduct(const CVector4& Vec1, const CVector4& Vec2);
	CVector4 CrossProduct(const CVector4& Vec1, const CVector4& Vec2);
	CVector4 CrossProduct(const CVector4& Vec1, const CVector4& Vec2, const CVector4& Vec3);
	CVector4 Normalize(const CVector4& Vec);

};

