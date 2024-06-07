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
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(-14.5, 0., 11.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(-14.5, 0., 11.0) + center, 0.0, mat);
	meshes.push_back(renderCube);
	//trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(-16.0, 0., 11.0) + center);
	//renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(-16.0, 0., 10.0) + center, 0.0, mat);
	//meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(-17.5, 0., 9.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(-17.5, 0., 9.0) + center, 0.0, mat);
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(13.0, 0., 12.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(13.0, 0., 12.0) + center, 0.0, mat);
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(14.5, 0., 11.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(14.5, 0., 11.0) + center, 0.0, mat);
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(16.0, 0., 10.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(16.0, 0., 10.0) + center, 0.0, mat);
	meshes.push_back(renderCube);
	trans = Cmatrix::translate(Cmatrix::Mat4x4(), Cvector::CVector(17.5, 0., 9.0) + center);
	renderCube = new StaticMesh(Cvector::CVector(3, 11., 0.5), Cvector::CVector(17.5, 0., 9.0) + center, 0.0, mat);
	meshes.push_back(renderCube);

}