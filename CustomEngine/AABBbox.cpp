#include "AABBbox.h"


AABBbox::AABBbox() {
    boundsMin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    boundsMax = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
}

AABBbox::AABBbox(Cvector::CVector min, Cvector::CVector max) {
    boundsMin = min;
    boundsMax = max;
}

void AABBbox::extend(const Cvector::CVector& point) {
     boundsMin.x = std::min<float>(boundsMin.x, point.x);
     boundsMin.y = std::min<float>(boundsMin.y, point.y);
     boundsMin.z = std::min<float>(boundsMin.z, point.z);

     boundsMax.x = std::max<float>(boundsMax.x, point.x);
     boundsMax.y = std::max<float>(boundsMax.y, point.y);
     boundsMax.z = std::max<float>(boundsMax.z, point.z);
}

bool AABBbox::intersects(const AABBbox& other) const {
    return (boundsMax.x >= other.boundsMin.x && boundsMin.x <= other.boundsMax.x &&
        boundsMax.y >= other.boundsMin.y && boundsMin.y <= other.boundsMax.y &&
        boundsMax.z >= other.boundsMin.z && boundsMin.z <= other.boundsMax.z);
}

bool AABBbox::intersects(const Cvector::CVector& OboundsMin, const Cvector::CVector& OboundsMax) {
    return (boundsMax.x >= OboundsMin.x && boundsMin.x <= OboundsMax.x &&
        boundsMax.y >= OboundsMin.y && boundsMin.y <= OboundsMax.y &&
        boundsMax.z >= OboundsMin.z && boundsMin.z <= OboundsMax.z);
}

bool AABBbox::contains(const Cvector::CVector& point) const {
     return (point.x >= boundsMin.x && point.x <= boundsMax.x &&
         point.y >= boundsMin.y && point.y <= boundsMax.y &&
         point.z >= boundsMin.z && point.z <= boundsMax.z);
 }


void OctNode::insert(const MeshBBox& box) {
    // 如果节点有子节点，则将盒子插入适当的子节点
    if (children.empty()) { //叶节点
        meshes.push_back(box);
        if (meshes.size() > MAX_OBJECTS_PER_NODE) {
            subdivide(); //分离
        }
    }
    else {
        for (std::vector<OctNode*>::iterator child = children.begin(); child != children.end(); child++) {
            if ((*child)->bounds.intersects(box.boundsMin, box.boundsMax)) {
                (*child)->insert(box);
            }
        }
    }
}

void  OctNode::subdivide() {
    // 分割AABB
    Cvector::CVector center((bounds.boundsMin.x + bounds.boundsMax.x) / 2.0f,
                   (bounds.boundsMin.y + bounds.boundsMax.y) / 2.0f,
                   (bounds.boundsMin.z + bounds.boundsMax.z) / 2.0f );

    children.push_back(new OctNode(AABBbox{{bounds.boundsMin.x, bounds.boundsMin.y, bounds.boundsMin.z}, center}));
    children.push_back(new OctNode(AABBbox{ {center.x, bounds.boundsMin.y, bounds.boundsMin.z}, {bounds.boundsMax.x, center.y, center.z} }));
    children.push_back(new OctNode(AABBbox{ {bounds.boundsMin.x, center.y, bounds.boundsMin.z}, {center.x, bounds.boundsMax.y, center.z} }));
    children.push_back(new OctNode(AABBbox{ {bounds.boundsMin.x, bounds.boundsMin.y, center.z}, {center.x, center.y, bounds.boundsMax.z} }));
    children.push_back(new OctNode(AABBbox{ {center.x, bounds.boundsMin.y, center.z}, {bounds.boundsMax.x, center.y, bounds.boundsMax.z} }));
    children.push_back(new OctNode(AABBbox{ {bounds.boundsMin.x, center.y, center.z}, {center.x, bounds.boundsMax.y, bounds.boundsMax.z} }));
    children.push_back(new OctNode(AABBbox{ {center.x, center.y, bounds.boundsMin.z}, {bounds.boundsMax.x, bounds.boundsMax.y, center.z} }));
    children.push_back(new OctNode(AABBbox{ {center.x, center.y, center.z}, bounds.boundsMax }));

    // 将已经存在的物体重新插入到子节点中
    for (int i = 0; i < meshes.size(); i++) {
        for (std::vector<OctNode*>::iterator child = children.begin(); child != children.end(); child++) {
            if ((*child)->bounds.intersects(meshes[i].boundsMin, meshes[i].boundsMax)) {
                (*child)->insert(meshes[i]);
                break;
            }
        }
    }
    meshes.clear();

    }

MeshBBox OctNode::getIntersectedObjects(const Ray& ray) const {
    MeshBBox result;
    getIntersectedObjectsHelper(ray, result);
    return result;
}

void OctNode::getIntersectedObjectsHelper(const Ray& ray, MeshBBox& result) const {
    if (!bounds.intersects(ray)) return;

    // 从当前节点中找到与射线相交的第一个物体
    for (const auto& obj : meshes) {
        if (obj.intersects(ray)) {
            result = obj;
            return;
        }
    }

    // 递归地在子节点中查找
    for (const auto& child : children) {
        child->getIntersectedObjectsHelper(ray, result);
    }
}

bool AABBbox::intersects(const Ray& ray) const {
    // Ray-AABB intersection test
    float tmin = (boundsMin.x - ray.origin.x) / ray.direction.x;
    float tmax = (boundsMax.x - ray.origin.x) / ray.direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (boundsMin.y - ray.origin.y) / ray.direction.y;
    float tymax = (boundsMax.y - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (boundsMin.z - ray.origin.z) / ray.direction.z;
    float tzmax = (boundsMax.z - ray.origin.z) / ray.direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    return true;
}

void OctNode::updateMeshBBox(OctNode* root, const MeshBBox& oldMesh, const MeshBBox& newMesh) {
    removeMeshBBox(root, oldMesh);
    root->insert(newMesh);
}



bool OctNode::removeMeshBBoxHelper(OctNode* node, const MeshBBox& meshBBox) {

    bool updated = false;
    if (!node->bounds.contains(meshBBox.boundsMin, meshBBox.boundsMax)) {
        return false;
    }

    auto it = std::find(node->meshes.begin(), node->meshes.end(), meshBBox);
    if (it != node->meshes.end()) {
        node->meshes.erase(it);
        updated = true;
    }

    if (!node->bounds.intersects(meshBBox.boundsMin, meshBBox.boundsMax)) {
        for (auto child : node->children) {
            updated |= removeMeshBBoxHelper(child, meshBBox);
        }
        return updated;
    }
}
void OctTree::getNodeBBoxTransforms(std::vector<Cmatrix::Mat4x4>& trans, std::vector<Cmatrix::Mat4x4>& objt) {

    getNodeBBoxTransformsHelper(root, trans, objt);
    
}
void OctTree::getNodeBBoxTransformsHelper(OctNode* node, std::vector<Cmatrix::Mat4x4>& trans, std::vector<Cmatrix::Mat4x4>& objt) {
    Cvector::CVector center = node->bounds.boundsMax * 0.5 + node->bounds.boundsMin * 0.5;
    Cvector::CVector scale = Cvector::CVector(node->bounds.boundsMax.x - node->bounds.boundsMin.x,
        node->bounds.boundsMax.y - node->bounds.boundsMin.y,
        node->bounds.boundsMax.z - node->bounds.boundsMin.z
    );
    Cmatrix::Mat4x4 mat;
    mat = Cmatrix::scale(mat, scale);
    Cmatrix::Mat4x4 translate = Cmatrix::translate(Cmatrix::Mat4x4(), center);
    mat = translate * mat;
    trans.push_back(mat);

    for (int i = 0; i<node->meshes.size(); i++) {
        Cvector::CVector center = node->meshes[i].boundsMax * 0.5 + node->meshes[i].boundsMin * 0.5;
        Cvector::CVector scale = Cvector::CVector(node->meshes[i].boundsMax.x - node->meshes[i].boundsMin.x,
            node->meshes[i].boundsMax.y - node->meshes[i].boundsMin.y,
            node->meshes[i].boundsMax.z - node->meshes[i].boundsMin.z
        );
        Cmatrix::Mat4x4 mat;
        mat = Cmatrix::scale(mat, scale);
        Cmatrix::Mat4x4 translate = Cmatrix::translate(Cmatrix::Mat4x4(), center);
        mat = translate * mat;
        objt.push_back(mat);
    
    }
    if (!node->children.empty()){
        for (auto child : node->children) {
            getNodeBBoxTransformsHelper(child, trans, objt);
        }
        
    }
}