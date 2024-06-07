#pragma once
#include <vector>
#include <iostream>
#include "CMatrix.h"
#include "Mesh.h"
#include "Camera.h"

class Region
{


protected:
	uint32_t size;
	std::vector<Cmatrix::Mat4x4> transform;
	Cvector::CVector center;
	Cvector::CVector color;



public:
	uint32_t mat;
	std::vector<StaticMesh*> meshes;
	Region(){
	}

	std::vector<Cmatrix::Mat4x4>& getTransforms() {
		return transform;
	}

	Cvector::CVector& getColorInfo() {
		return color;
	}
	virtual void SpawnMember()=0;

	void drawmeshes(Camera* cam) {
		for (auto m : meshes) {
			m->draw(cam, color);
		}
	}
};

class RegionA :public Region {
public:
	RegionA() {
		center = Cvector::CVector(0.0, 0.0, 0.0);
		color = Cvector::CVector(0.6, 0.5, 0.2);

	}

	void SpawnMember();
	
};

class RegionB1:public Region {
public:
	RegionB1() {
		center = Cvector::CVector(0.0, 0.0, 0.0);
		color = Cvector::CVector(0.8, 0.1, 0.3);

	}
	void SpawnMember();
};

class RegionB2 :public Region {
public:
	RegionB2() {
		center = Cvector::CVector(0., 0.0, 10.0);
		color = Cvector::CVector(0.4, 0.8, 0.9);

	}

	void SpawnMember();
};

class RegionC :public Region {
public:
	RegionC() {
		center = Cvector::CVector(0., 5.5, 0.0);
		color = Cvector::CVector(0.2, 0.6, 0.2);

	}

	void SpawnMember();
};

class StageInfo
{


};

