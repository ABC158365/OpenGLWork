#include "StageInfo.h"
#include "Cmath.h"

void RegionA::SpawnMember() {

	float angle = Cmath::pi<float>() / 2;
	float r = 29 / angle;
	float perangle = angle / 28;
	for (int i = 0; i < 29; i++) {
		float x = cos(Cmath::pi<float>() / 4 + perangle * i) * r;
		float y = sin(Cmath::pi<float>() / 4 + perangle * i) * r;

		
		StaticMesh* renderCube = new StaticMesh(Cvector::CVector(1., 12., 1.), Cvector::CVector(x, 6, y), Cmath::pi<float>() / 4 - perangle * i ,mat);
		renderCube->setCubeUV(0, (i+1) * 1.0 / 29, 1, (i) * 1.0 / 29, 1, (i) * 1.0 / 29, 0, (i+1) * 1.0 / 29, 0);
		renderCube->type = -1;
		meshes.push_back(renderCube);
	}
}

void RegionB1::SpawnMember() {

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 10; j++) {

			/*Cmatrix::Mat4x4 cmat = Cmatrix::scale(Cmatrix::Mat4x4(),);
			Cmatrix::Mat4x4 trans = Cmatrix::translate(Cmatrix::Mat4x4(),);*/
			//mat = glm::scale(mat, glm::vec3(0.9, 5., 0.99));
			//glm::mat4x4 trans = glm::translate(glm::mat4x4(1.0f), glm::vec3(i - 4.5, 2.5, j));
			//cmat = trans * cmat;
			//lm::mat4x4 mat = getMatfromCmat(cmat);
			StaticMesh* renderCube = new StaticMesh(Cvector::CVector(0.9, 5.0, 0.99), Cvector::CVector(i - 4.5, 2.5, j), 0.0,  mat);
			renderCube->CtexIndex = 8-i;
			renderCube->texIndex = j;
			int jt = 10 - j;
			renderCube->setCubeUV(3, 1.0*(jt-1)/10, 0, 1.0*(jt)/10, 0, 1.0 * (jt) / 10, 1, 1.0 * (jt-1) / 10, 1);
			jt = 10 - jt +1;
			renderCube->setCubeUV(2, 1.0 * (jt) / 10, 0, 1.0 * (jt - 1) / 10, 0, 1.0 * (jt - 1) / 10, 1, 1.0 * (jt) / 10, 1);
			int it = 9 - i;


			renderCube->setCubeUV(0, 
				
				1.0 * (it) / 9, 1, 
				1.0 * (it - 1) / 9, 1,
				1.0 * (it - 1) / 9, 0,
				1.0 * (it) / 9, 0
				
				);

			it = 8 - i;
			jt = 9 - j;
			renderCube->setCubeUV(5,	 
										(it + 1.0) / 9, (jt +1.0)/10,
										it * 1.0 / 9,(jt + 1.0) / 10,
										it * 1.0 / 9, jt * 1.0 / 10,
										(it + 1.0) / 9, jt * 1.0 / 10
				);
			renderCube->type = 0;
			meshes.push_back(renderCube);
		}


	}

}

void RegionB2::SpawnMember(){

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 4; j++) {
			//Cmatrix::Mat4x4 cmat = Cmatrix::scale(Cmatrix::Mat4x4(),);
			//Cmatrix::Mat4x4 trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(i - 7.5, 1.5, j) + center);
			//cmat = trans * cmat;
			//glm::mat4x4 mat = getMatfromCmat(cmat);
			StaticMesh* renderCube = new StaticMesh(Cvector::CVector(0.9, 3., 0.99), Cvector::CVector(i - 7.5, 1.5, j) + center, 0.0, mat);
			renderCube->type = 1;
			renderCube->CtexIndex = 14 - i;
			renderCube->texIndex = j;
			int jt = 4 - j;
			renderCube->setCubeUV(3, 1.0 * (jt - 1) / 4, 0, 1.0 * (jt) / 4, 0, 1.0 * (jt) / 4, 1, 1.0 * (jt - 1) / 4, 1);
			jt = 4 - jt + 1;
			renderCube->setCubeUV(2, 1.0 * (jt) / 4, 0, 1.0 * (jt - 1) / 4, 0, 1.0 * (jt - 1) / 4, 1, 1.0 * (jt) / 4, 1);
			renderCube->setCubeUV(0, 1.0 * (i) / 15, 1, 1.0 * (i - 1) / 15, 1, 1.0 * (i - 1) / 15, 0, 1.0 * (i) / 15, 0);

			int it = 14 - i;
			jt = 3 - j;
			renderCube->setCubeUV(5,
				(it + 1.0) / 15, (jt + 1.0) / 4,
				it * 1.0 / 15, (jt + 1.0) / 4,
				it * 1.0 / 15, jt * 1.0 / 4,
				(it + 1.0) / 15, jt * 1.0 / 4
			);
			
			meshes.push_back(renderCube);
		}

	}
}

void RegionC::SpawnMember() {
	transform.resize(8);
	//glm::mat4x4 (1.0f);
	Cmatrix::Mat4x4 cmat = Cmatrix::scale(Cmatrix::Mat4x4(), Cvector::CVector(3, 11., 0.5));
	Cmatrix::Mat4x4 trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(-13.0, 0., 12.0) + center);
	StaticMesh* renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(-13.0, 0., 12.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 1, 0, 0.5, 1, 0.5, 1, 1);
	renderCube->setCubeUV(2, 1, 1, 1, 0.875, 0, 0.875, 0, 1);
	renderCube->setCubeUV(3, 1, 0.75, 1, 0.875, 0, 0.875, 0, 0.75);
	renderCube->texIndex = 2;
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(-14.5, 0., 11.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(-14.5, 0., 11.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 0.5, 0, 0, 1, 0, 1, 0.5);
	renderCube->setCubeUV(2, 1, 0.75, 1, 0.625, 0, 0.625, 0, 0.75);
	renderCube->setCubeUV(3, 1, 0.5, 1, 0.625, 0, 0.625, 0, 0.5);
	renderCube->texIndex = 2;
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(-16.0, 0., 11.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(-16.0, 0., 10.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 0.5, 0, 0, 1, 0, 1, 0.5);
	renderCube->setCubeUV(1, 0, 0.5, 0, 1, 1, 1, 1, 0.5);
	renderCube->setCubeUV(2, 1, 0.5, 1, 0.375, 0, 0.375, 0, 0.5);
	renderCube->setCubeUV(3, 1, 0.25, 1, 0.375, 0, 0.375, 0, 0.25);
	renderCube->texIndex = 1;
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(-17.5, 0., 9.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(-17.5, 0., 9.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 0.5, 0, 0, 1, 0, 1, 0.5);
	renderCube->setCubeUV(1, 0, 0.5, 0, 1, 1, 1, 1, 0.5);
	renderCube->setCubeUV(2, 1, 0.25, 1, 0.125, 0, 0.125, 0, 0.25);
	renderCube->setCubeUV(3, 1, 0, 1, 0.125, 0, 0.125, 0, 0);
	renderCube->texIndex = 0;
	meshes.push_back(renderCube);


	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(13.0, 0., 12.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(13.0, 0., 12.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 1, 0, 0.5, 1, 0.5, 1, 1);
	renderCube->texIndex = 2;
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(14.5, 0., 11.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(14.5, 0., 11.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 0.5, 0, 0, 1, 0, 1, 0.5);
	renderCube->texIndex = 2;
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(16.0, 0., 10.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(16.0, 0., 10.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 0.5, 0, 0, 1, 0, 1, 0.5);
	renderCube->setCubeUV(1, 0, 0.5, 0, 1, 1, 1, 1, 0.5);
	renderCube->texIndex = 1;
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(17.5, 0., 9.0) + center);

	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(17.5, 0., 9.0) + center, 0.0, mat);
	renderCube->setCubeUV(0, 0, 0.5, 0, 0, 1, 0, 1, 0.5);
	renderCube->setCubeUV(1, 0, 0.5, 0, 1, 1, 1, 1, 0.5);
	renderCube->texIndex = 0;
	meshes.push_back(renderCube);

}