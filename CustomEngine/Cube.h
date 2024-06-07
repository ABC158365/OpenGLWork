#pragma once
#include "Mesh.h"
#include <random>
#include <algorithm>
#include "StageInfo.h"
class InstancedCube:public InstancedMesh
{

public:
    InstancedCube(std::string v_path, std::string f_path) {
        float t_vertices[] = {
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        };

        vertex_path = v_path;
        frag_path = f_path;

        int arraySize = sizeof(t_vertices) / sizeof(float);

        
        vertices.assign(t_vertices, t_vertices+ arraySize);

        matID =  Material::registerMaterial(v_path.c_str(), f_path.c_str());
        bindBuffers();

       
        regionA = new RegionA();
        regionB1 = new RegionB1();
        regionB2 = new RegionB2();
        regionC = new RegionC();
        //updateBuffers();
    }

    ~InstancedCube() {
        delete regionA;
        delete regionB1;
        delete regionB2;
        delete regionC;
    }
    int hueshift=0;

    void draw(Camera* cam);
    void updateBuffers(Region* region);
    void getInstanceBoundFromTransform(Cvector::CVector& trans);


private:
    std::vector<Cvector::CVector> cubePositions;
    std::vector<Cvector::CVector> colors;
    std::vector<Cmatrix::Mat4x4> transforms;
    RegionA* regionA;
    RegionB1* regionB1;
    RegionB2* regionB2;
    RegionC* regionC;

    void bindBuffers();
};


class GridPlane :public Mesh {
public:
    GridPlane(std::string v_path, std::string f_path) {
        float t_vertices[] = {
            -0.5f, -0.5f, -0.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
           -0.5f,  0.5f, -0.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
           -0.5f, -0.5f, -0.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
        };

        vertex_path = v_path;
        frag_path = f_path;

        int arraySize = sizeof(t_vertices) / sizeof(float);


        vertices.assign(t_vertices, t_vertices + arraySize);

        matID = Material::registerMaterial(v_path.c_str(), f_path.c_str());

        bindBuffers();
        
    }

    void draw(Camera* cam);

   
    void bindBuffers();

};
