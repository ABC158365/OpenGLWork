#pragma once
#include <vector>
#include <string>
#include "Material.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CRotator.h"

#include "CMatrix.h"
#include "Cmath.h"

namespace {
    void getVerteciesInfo(std::vector<float>& vertices, std::string file_path);
}

class Mesh;

struct HitResult
{
    Mesh* mesh;
    Cvector::CVector hitPos;
    float distance = std::numeric_limits<float>::max();
    bool isHit = false;
};

struct Triangles
{
    Cvector::CVector a;
    Cvector::CVector b;
    Cvector::CVector c;
    Mesh* parent;

    // Function to check intersection with a line
    bool intersectsLine(const Cvector::CVector& start, const Cvector::CVector& end, Cvector::CVector& intersectionPt) const {
        Cvector::CVector dir = end - start;

        // Compute normal of the triangle
        Cvector::CVector normal = (b - a).crossMul(c - a);
        normal.Normalize();

        // Check if line and plane of the triangle are parallel
        float denom = normal.dotMul(dir);

        if (fabs(denom) < 0.000001f) // Parallel or nearly parallel
            return false;

        // Compute intersection point with the plane
        float t = normal.dotMul(a - start) / denom;

        if (t < 0) // Intersection behind the line's start point
            return false;


        Cvector::CVector intersection = start + dir * t;
        intersectionPt = intersection;

        // Check if the intersection point is inside the triangle
        float u, v;
        Cvector::CVector edge1 = b - a;
        Cvector::CVector edge2 = c - a;
        Cvector::CVector pvec = dir.crossMul(edge2);
        float det = edge1.dotMul(pvec);

        if (det > -0.000001f && det < 0.000001f) // Triangle is degenerate, no intersection
            return false;

        float invDet = 1.0f / det;
        Cvector::CVector tvec = start - a;
        u = tvec.dotMul(pvec) * invDet;

        if (u < 0.0f || u > 1.0f)
            return false;

        Cvector::CVector qvec = tvec.crossMul(edge1);
        v = dir.dotMul(qvec) * invDet;

   
        if (v < 0.0f || u + v > 1.0f)
            return false;
        return true; // Intersection within the triangle
    }
};

class Mesh
{

public:
   
    void setVertices(std::vector<float>& fv) {
        vertices.assign(fv.begin(), fv.end());
    }

    std::string parent;
	virtual void getVertices(bool ForceUpdateCollider, std::vector<float>& fv) {
        fv.clear();
        for (int i = 0; i < vertices.size() / 3; i+=3) {
            Cmatrix::Mat4x4 cmat;
            //glm::mat4x4 mat(1.0f);
            cmat = Cmatrix::scale(cmat,scale);
            cmat = CRotator::getRotationMatrix(rotation.Pitch, rotation.Yaw, rotation.Roll) * cmat;
            Cmatrix::Mat4x4 transs = Cmatrix::translate(Cmatrix::Mat4x4(), translate);
            cmat = transs * cmat;

            Cvector::CVector4 vert(vertices[i], vertices[i  + 1], vertices[i + 2], 1);
            vert = cmat * vert;
            fv.push_back(vert.x);
            fv.push_back(vert.y);
            fv.push_back(vert.z);

        }

        if (ForceUpdateCollider) {
            triangleVerts.clear();
            if (vertices.size() % 9 != 0)std::exception("Error!");
            for (int i = 0; i < vertices.size() / 9; i += 9) {
                Triangles tri;
                tri.a = Cvector::CVector(fv[i], fv[i + 1], fv[i + 2]);
                tri.b = Cvector::CVector(fv[i + 3], fv[i + 4], fv[i + 5]);
                tri.c = Cvector::CVector(fv[i + 6], fv[i + 7], fv[i + 8]);
                triangleVerts.push_back(tri);
            }
        }
	}

	uint32_t getID() {
		return matID;
	}

    bool selected = false;

	virtual void draw(Camera* cam) {};

    HitResult rawIntersection(Cvector::CVector start, Cvector::CVector end);
    HitResult checkIntersection(Cvector::CVector start, Cvector::CVector end, bool rawQuery=false);
    virtual  bool boxIntersection(Mesh* other) {
        Cvector::CVector delta = (boundsMax + boundsMin) * 0.5f - (other->boundsMax + other->boundsMin) * 0.5;
        Cvector::CVector size = (boundsMax-boundsMin)*0.5 + (other->boundsMax - other->boundsMin) * 0.5;
        
        if (abs(delta.x) < size.x &&
            abs(delta.y) < size.y &&
            abs(delta.z) < size.z
            ) {
            std::cout << "(" << boundsMin.x << "," << boundsMin.y << "," << boundsMin.z << ")" << "(" << boundsMax.x << "," << boundsMax.y << "," << boundsMax.z << ")";
            std::cout << "(" << other->boundsMin.x << "," << other->boundsMin.y << "," << other->boundsMin.z << ")" << "(" << other->boundsMax.x << "," << other->boundsMax.y << "," << other->boundsMax.z << ")";

            return true;
        }
        return false;

    }


    Cvector::CVector translate = Cvector::CVector(0., 0., 0.);
    Cvector::CVector abstranslate = Cvector::CVector(0., 0., 0.);
    Cvector::CVector scale = Cvector::CVector(1., 1., 1.);
    CRotator::CRotator rotation = CRotator::CRotator(0., 0., 0.);

 



protected:
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	std::string vertex_path;
	std::string frag_path;
	uint32_t matID;
	uint32_t VAO;
	std::vector<uint32_t>VBO;
	uint32_t EBO;
	uint32_t VBOsize;
	std::vector<uint32_t> textures;
    std::vector<Triangles>triangleVerts;
    
    Cvector::CVector boundsMin;
    Cvector::CVector boundsMax;

private:
	virtual void bindBuffers() {};
    virtual void updateBounds() {};
};



class StaticMesh :public Mesh {
public:
    int texIndex;
    int CtexIndex;

    StaticMesh(Cvector::CVector scale, Cvector::CVector translate, float yaw, uint32_t matID);
    StaticMesh(std::string file_path, uint32_t matID_);
    
    virtual void getVertices(bool ForceUpdateCollider, std::vector<float>& fv);

    void setCubeUV(int face, float lbu, float lbv, float rbu, float rbv, float rtu, float rtv,
        float ltu, float ltv
    );
    void updateBuffers(bool forceUpdateCollider);
	void bindBuffers();
	void draw(Camera* cam, Cvector::CVector color);
	void SetMatID(uint32_t id) {
		matID = id;
	};

};


class Actor;

class SkeletalMesh :public StaticMesh {
public:
    Actor* root;
    Cmatrix::Mat4x4 skeletonRot;
    SkeletalMesh(Cvector::CVector scale, Cvector::CVector translate, float yaw, uint32_t matI, Actor* root_) :StaticMesh(scale, translate, yaw, matI) { root = root_; skeletonRot = CRotator::getRotationMatrix(0.0, yaw, 0.0); };
    SkeletalMesh(std::string file_path, uint32_t matID_, Actor* root_) :StaticMesh(file_path, matID_) { root = root_; skeletonRot = Cmatrix::Mat4x4(); };
    
    void intersectionEvent(Mesh* other);

    virtual void getVertices(bool ForceUpdateCollider, std::vector<float>& fv);
    void updateBuffers(bool forceUpdateCollider) { StaticMesh::updateBuffers(forceUpdateCollider); };
    void bindBuffers() { StaticMesh::bindBuffers(); };
    void draw(Camera* cam, Cvector::CVector color) { StaticMesh::draw(cam, color); };
    void SetMatID(uint32_t id) {
        StaticMesh::SetMatID(id);
    };
};


class Actor{
public:
    std::string name;


    SkeletalMesh* body;
    SkeletalMesh* head;
    SkeletalMesh* leftArm;
    SkeletalMesh* rightArm;
    SkeletalMesh* leftLeg;
    SkeletalMesh* rightLeg;

    Actor(uint32_t matID);

    void appendMeshManager(std::vector<Mesh*>& manager);

    void draw(Camera* cam);
    CRotator::CRotator getRotator() {
        return rootR;
    }
    Cvector::CVector getTranslate() {
        return rootT;
    }
    void markSelect();
    void disSelect();
    void setRotation(CRotator::CRotator rot);
    void setTranslate(Cvector::CVector t);
    void updateBuffer();

private:
    Cvector::CVector rootT;
    CRotator::CRotator rootR;
    
};


class InstancedMesh :public Mesh
{

};

class DebugFrameCube :public Mesh {
public:
	DebugFrameCube();
	void updateBuffers(std::vector<Cmatrix::Mat4x4>& trans);
	void bindBuffers();
	void draw(Camera* cam, Cvector::CVector color);
	std::vector<Cmatrix::Mat4x4> transforms;
};