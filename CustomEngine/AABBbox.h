#pragma once
#include "Mesh.h"

struct Ray {
    Cvector::CVector origin;
    Cvector::CVector direction;
};




class AABBbox
{
public:
    Cvector::CVector boundsMin;
    Cvector::CVector boundsMax;
    
    AABBbox();
    AABBbox(Cvector::CVector min, Cvector::CVector max);

    void extend(const Cvector::CVector& point);
    bool contains(const Cvector::CVector& point)const;
    bool contains(const AABBbox& other) const {
        return (other.boundsMin.x >= boundsMin.x && other.boundsMin.y >= boundsMin.y && other.boundsMin.z >= boundsMin.z &&
            other.boundsMax.x <= boundsMax.x && other.boundsMax.y <= boundsMax.y && other.boundsMax.z <= boundsMax.z);
    }
    bool contains(const Cvector::CVector& OboundsMin, const Cvector::CVector& OboundsMax) const {
        return (OboundsMin.x >= boundsMin.x && OboundsMin.y >= boundsMin.y && OboundsMin.z >= boundsMin.z &&
            OboundsMax.x <= boundsMax.x && OboundsMax.y <= boundsMax.y && OboundsMax.z <= boundsMax.z);
    }

    bool intersects(const Cvector::CVector& OboundsMin, const Cvector::CVector& OboundsMax);
    bool intersects(const AABBbox& other) const;
    bool intersects(const Ray& ray) const;

   

    
};


class MeshBBox : public AABBbox {
public:
    Mesh* mesh;
    int instancedID;
    MeshBBox(Cvector::CVector min, Cvector::CVector max, int id, Mesh* m) :AABBbox(min, max), instancedID(id), mesh(m) {};
    MeshBBox() :AABBbox() {
        instancedID = -2;
    };

    bool operator ==(const MeshBBox& other) {
        return mesh == other.mesh;
    }

};

class OctNode {
public:
    AABBbox bounds;
	std::vector<OctNode*> children;
    std::vector<MeshBBox> meshes;
    static const int MAX_OBJECTS_PER_NODE = 5;

    OctNode(const AABBbox& boundingBox) : bounds(boundingBox) {}


	~OctNode() {
		for (auto child : children) {
			delete child;
		}
	}


    void subdivide();
    void insert(const MeshBBox& box);
    void updateMeshBBox(OctNode* root, const MeshBBox& oldMesh, const MeshBBox& newMesh);
   
    void removeMeshBBox(OctNode* root, const MeshBBox& meshBBox) {
        removeMeshBBoxHelper(root, meshBBox);
    }

    MeshBBox getIntersectedObjects(const Ray& ray) const;

private:
    void getIntersectedObjectsHelper(const Ray& ray, MeshBBox& result) const;
    bool removeMeshBBoxHelper(OctNode* node, const MeshBBox& meshBBox);
};

class OctTree
{
public:
    OctNode* root;
    DebugFrameCube* debugRender;


    OctTree(const AABBbox& box) {
        root = new OctNode(box);
        debugRender = new DebugFrameCube();
    };
    ~OctTree() {
        delete root;
        delete debugRender;
    };

    OctNode* getRoot() {
        return root;
    }

    void renderFramework(Camera* cam) {
        std::vector<Cmatrix::Mat4x4>bbox;
        std::vector<Cmatrix::Mat4x4>obbox;
        getNodeBBoxTransforms(bbox, obbox);

        debugRender->updateBuffers(obbox);
        debugRender->draw(cam, Cvector::CVector(1.0, 1.0, 0.0));
        debugRender->updateBuffers(bbox);
        debugRender->draw(cam,Cvector::CVector(1.0, 0.0, 0.0));
    }
    void getNodeBBoxTransforms(std::vector<Cmatrix::Mat4x4>& trans, std::vector<Cmatrix::Mat4x4>& objt);
    void getNodeBBoxTransformsHelper(OctNode* node, std::vector<Cmatrix::Mat4x4>& trans, std::vector<Cmatrix::Mat4x4>& objt);



private:

};

