#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace {
    void getVerteciesInfo(std::vector<float>& vertices, std::string file_path) {
        vertices.clear();

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_path.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
        }
    }


}

DebugFrameCube::DebugFrameCube() {
    float tvertices[] = {
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
     -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    };

    vertices.assign(tvertices, tvertices+int(sizeof(tvertices) / sizeof(float)));

    // Indices to draw the rectangle as a wireframe
    uint32_t tindices[] = {
           0, 1, // bottom line
           1, 2, // right line
           2, 3, // top line
           3, 0,  // left lin
           4, 5,
           5, 6,
           6, 7,
           7, 4,
           0, 4,
           3, 7,
           1, 5,
           2, 6
    };

    indices.assign(tindices, tindices + int(sizeof(tindices) / sizeof(unsigned int)));

    matID = Material::registerMaterial("./frameCubeV.glsl", "./frameCubeF.glsl");
    
    bindBuffers();
}

void DebugFrameCube::bindBuffers() {
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBO.resize(2);
    glGenBuffers(2, VBO.data());
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
}

void DebugFrameCube::updateBuffers(std::vector<Cmatrix::Mat4x4>& trans) {

    transforms.clear();
    transforms.insert(transforms.end(), trans.begin(), trans.end());
    
    glBindVertexArray(VAO);
    std::size_t vec4Size = sizeof(Cvector::CVector4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(Cmatrix::Mat4x4), transforms.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(0));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
    glBindVertexArray(0);
}

void DebugFrameCube::draw(Camera* cam, Cvector::CVector color) {
    glUseProgram(matID);

    Cmatrix::Mat4x4 vp = cam->GetViewMatrix(true);
    vp = cam->getProjectionMatrix() * vp;
    glUniformMatrix4fv(glGetUniformLocation(matID, "mvp"), 1, GL_FALSE, vp.value_ptr());
    glUniform3fv(glGetUniformLocation(matID, "color"), 1, color.value_ptr());

    // Set polygon mode to draw only the outlines
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Draw rectangle
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, transforms.size());
    glBindVertexArray(0);

    // Reset polygon mode to draw filled faces
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

StaticMesh::StaticMesh(Cvector::CVector scale_, Cvector::CVector translate_, float yaw_, uint32_t matID_) {
    matID = matID_;
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

   /* for (int i = 0; i < 36; i++) {
        Cvector::CVector4 vert(t_vertices[i * 8], t_vertices[i * 8 + 1], t_vertices[i * 8 + 2], 1);
        vert = transform * vert;
        t_vertices[i * 8] = vert.x;
        t_vertices[i * 8 + 1] = vert.y;
        t_vertices[i * 8 + 2] = vert.z;
    }*/

    //for (int i = 0; i < 12; i++) {
    //    Triangles tri;
    //    tri.a = Cvector::CVector(t_vertices[i * 24], t_vertices[i * 24 + 1], t_vertices[i * 24 + 2]);
    //    tri.b = Cvector::CVector(t_vertices[i * 24 + 8], t_vertices[i * 24 + 9], t_vertices[i * 24 + 10]);
    //    tri.c = Cvector::CVector(t_vertices[i * 24 + 16], t_vertices[i * 24 + 17], t_vertices[i * 24 + 18]);
    //    triangleVerts.push_back(tri);
    //}

    scale = scale_;
    rotation = CRotator::CRotator(0, yaw_, 0);
    
    translate = translate_;
    abstranslate = translate_;

    int arraySize = sizeof(t_vertices) / sizeof(float);
    vertices.assign(t_vertices, t_vertices + arraySize);

    
    bindBuffers();

}

StaticMesh::StaticMesh(std::string file_path, uint32_t matID_) {
    matID = matID_;
    getVerteciesInfo(vertices, file_path);
    bindBuffers();

}

void StaticMesh::getVertices(bool ForceUpdateCollider, std::vector<float>& fv) {
    fv.clear();
    Cvector::CVector tboundsmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Cvector::CVector tboundsmax(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    for (int i = 0; i < vertices.size(); i += 8) {
        Cmatrix::Mat4x4 cmat;
        //glm::mat4x4 mat(1.0f);

        
        cmat = Cmatrix::scale(cmat, scale);
        cmat = CRotator::getRotationMatrix(rotation.Pitch, rotation.Yaw, rotation.Roll)*cmat;
        Cmatrix::Mat4x4 transs = Cmatrix::translate(Cmatrix::Mat4x4(), translate);
        cmat = transs * cmat;

        Cvector::CVector4 vert(vertices[i], vertices[i + 1], vertices[i + 2], 1);
        vert = cmat * vert;
        fv.push_back(vert.x);
        fv.push_back(vert.y);
        fv.push_back(vert.z);
        Cvector::CVector4 norm(vertices[i+3], vertices[i + 4], vertices[i + 5], 1);
       // norm = rot * norm; todo::正交变换
        fv.push_back(norm.x);
        fv.push_back(norm.y);
        fv.push_back(norm.z);
        fv.push_back(vertices[i + 6]);
        fv.push_back(vertices[i + 7]);

        tboundsmin.x = std::min<float>(tboundsmin.x, vert.x);
        tboundsmin.y = std::min<float>(tboundsmin.y, vert.y);
        tboundsmin.z = std::min<float>(tboundsmin.z, vert.z);
        tboundsmax.x = std::max<float>(tboundsmax.x, vert.x);
        tboundsmax.y = std::max<float>(tboundsmax.y, vert.y);
        tboundsmax.z = std::max<float>(tboundsmax.z, vert.z);
        
    }

    boundsMin = tboundsmin;
    boundsMax = tboundsmax;

    if (ForceUpdateCollider) {
        triangleVerts.clear();
        if (vertices.size() % 24 != 0)std::exception("Error!");
        for (int i = 0; i < vertices.size(); i += 24) {
            Triangles tri;
            tri.a = Cvector::CVector(fv[i], fv[i + 1], fv[i + 2]);
            tri.b = Cvector::CVector(fv[i + 8] ,fv[i + 9], fv[i + 10]);
            tri.c = Cvector::CVector(fv[i + 16], fv[i + 17], fv[i + 18]);
            triangleVerts.push_back(tri);

        }
        
    }
}


void StaticMesh::updateBuffers(bool forceUpdateCollider) {
    std::vector<float> res;
    getVertices(forceUpdateCollider, res);
    VBOsize = 1;
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, res.size() * 4, res.data());
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StaticMesh::bindBuffers() {
    std::vector<float> res;
    getVertices(true, res);
    VBOsize = 1;
    glGenVertexArrays(1, &VAO);
    VBO.resize(VBOsize);
    glGenBuffers(VBOsize, VBO.data());
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, res.size() * 4, res.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StaticMesh::draw(Camera* cam, Cvector::CVector color) {
    
    
    
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    

    glBindVertexArray(VAO);
    glUseProgram(matID);
    int a = glGetUniformLocation(matID, "ourColor");
    float green = sin(glfwGetTime());
    Material::setFloat(matID, "offset", green / 4);
    glUniform4f(a, 0.0f, green, 0.0f, 1.0f);
    Material::setFloat(matID, "par", (green + 1) / 2.0);
    if (selected == true)
    {
        Material::setBool(matID, "selected", true);
        glStencilFunc(GL_ALWAYS, 255, 255);
        glStencilMask(0xFF);

    }
    else {
        Material::setBool(matID, "selected", false);
        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilMask(0x00);
    }

    Material::setInt(matID, "type", 1);
    


    Cmatrix::Mat4x4 vp = cam->GetViewMatrix(true);
    vp = cam->getProjectionMatrix() * vp;
    glUniformMatrix4fv(glGetUniformLocation(matID, "mvp"), 1, GL_FALSE, vp.value_ptr());
    glUniform3fv(glGetUniformLocation(matID, "color"), 1, color.value_ptr());

    Cvector::CVector lightpos(5.0f, 10.0f, 10.0f);

    //glm::vec3 lightpos = glm::vec3(5.0f, 10.0f, 10.0f);
    glUniform3fv((glGetUniformLocation(matID, "lightPos")), 1, lightpos.value_ptr());
    glUniform3fv((glGetUniformLocation(matID, "viewPos")), 1, cam->GetPos().value_ptr());
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/8);
    glBindVertexArray(0);


      

}

void Actor::appendMeshManager(std::vector<Mesh*>& manager) {
    head->parent = "Actor";
    manager.push_back(head);
    body->parent = "Actor";
    manager.push_back(body);
    leftArm->parent = "Actor";
    manager.push_back(leftArm);
    rightArm->parent = "Actor";
    manager.push_back(rightArm);
    leftLeg->parent = "Actor";
    manager.push_back(leftLeg);
    rightLeg->parent = "Actor";
    manager.push_back(rightLeg);
}


void Actor::draw(Camera* cam) {
    head->draw(cam, Cvector::CVector(1.0f, 0.5f, 0.5f));
    leftArm->draw(cam, Cvector::CVector(1.0f, 1.0f, 1.0f));
    rightArm->draw(cam, Cvector::CVector(1.0f, 1.0f, 1.0f));
    leftLeg->draw(cam, Cvector::CVector(1.0f, 1.0f, 1.0f));
    rightLeg->draw(cam, Cvector::CVector(1.0f, 1.0f, 1.0f));
    body->draw(cam, Cvector::CVector(1.0f, 1.0f, 1.0f));
}

void Actor::setRotation(CRotator::CRotator rot) {
    rootR = rot;
}

void Actor::setTranslate(Cvector::CVector t) {
    rootT = t;
}


namespace {
    void updateSkeletalMesh(SkeletalMesh* mesh, Cvector::CVector rootT, CRotator::CRotator rootR) {

        Cvector::CVector4 res = Cvector::CVector4(mesh->abstranslate.x, mesh->abstranslate.y, mesh->abstranslate.z, 1);
        res = CRotator::getRotationMatrix(rootR) * res;
        mesh->translate = rootT + Cvector::CVector(res.x, res.y, res.z) ;
        mesh->skeletonRot = CRotator::getRotationMatrix(rootR)* CRotator::getRotationMatrix(mesh->rotation);
        mesh->updateBuffers(true);
    }
}

void Actor::updateBuffer() {
    updateSkeletalMesh(head, rootT, rootR);
    updateSkeletalMesh(body, rootT, rootR);
    updateSkeletalMesh(leftArm, rootT, rootR);
    updateSkeletalMesh(rightArm, rootT, rootR);
    updateSkeletalMesh(leftLeg, rootT, rootR);
    updateSkeletalMesh(rightLeg, rootT, rootR);
}

Actor::Actor(uint32_t matID){

    head = new SkeletalMesh("./obj/sphere.obj", matID, this);
    head->abstranslate = Cvector::CVector(0.0, 3.2, 0.0);
    head->updateBuffers(true);
    body = new SkeletalMesh("./obj/body.obj", matID, this);
    body->abstranslate = Cvector::CVector(0.0, 2, 0.0);
    body->updateBuffers(true);
    leftArm = new SkeletalMesh("./obj/arm.obj", matID, this);
    leftArm->abstranslate = Cvector::CVector(0, 2.6, -0.6);
    leftArm->updateBuffers(true);
    rightArm = new SkeletalMesh("./obj/arm.obj", matID, this);
    rightArm->abstranslate = Cvector::CVector(0, 2.6, 0.6);
    rightArm->updateBuffers(true);
    leftLeg = new SkeletalMesh("./obj/leg.obj", matID, this);
    leftLeg->abstranslate = Cvector::CVector(0.0, 1.3, -0.25);
    leftLeg->updateBuffers(true);
    rightLeg = new SkeletalMesh("./obj/leg.obj", matID, this);
    rightLeg->abstranslate = Cvector::CVector(0.0, 1.3, 0.25);
    rightLeg->updateBuffers(true);

    rootR = CRotator::CRotator();
    rootT = Cvector::CVector(0., 0., 0.);

}


void Actor::disSelect() { 
    head->selected = false;
    body->selected = false;
    leftArm->selected = false;
    rightArm->selected = false;
    leftLeg->selected = false;
    rightLeg->selected = false;
}

void Actor::markSelect() {
    head->selected = true;
    body->selected = true;
    leftArm->selected = true;
    rightArm->selected = true;
    leftLeg->selected = true;
    rightLeg->selected = true;
}


void SkeletalMesh::intersectionEvent(Mesh* other) {
    if (boxIntersection(other)) {
        std::cout << "碰撞信息：" << other->parent << "--" << root->name << "\n";
    }
}

void SkeletalMesh::getVertices(bool ForceUpdateCollider, std::vector<float>& fv) {
    fv.clear();
    Cvector::CVector tboundsmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Cvector::CVector tboundsmax(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    for (int i = 0; i < vertices.size(); i += 8) {
        Cmatrix::Mat4x4 cmat;
        //glm::mat4x4 mat(1.0f);


        cmat = Cmatrix::scale(cmat, scale);
        cmat = skeletonRot * cmat;
        Cmatrix::Mat4x4 transs = Cmatrix::translate(Cmatrix::Mat4x4(), translate);
        cmat = transs * cmat;

        Cvector::CVector4 vert(vertices[i], vertices[i + 1], vertices[i + 2], 1);
        vert = cmat * vert;
        fv.push_back(vert.x);
        fv.push_back(vert.y);
        fv.push_back(vert.z);
        Cvector::CVector4 norm(vertices[i + 3], vertices[i + 4], vertices[i + 5], 1);
        // norm = rot * norm; todo::正交变换
        fv.push_back(norm.x);
        fv.push_back(norm.y);
        fv.push_back(norm.z);
        fv.push_back(vertices[i + 6]);
        fv.push_back(vertices[i + 7]);

        tboundsmin.x = std::min<float>(tboundsmin.x, vert.x);
        tboundsmin.y = std::min<float>(tboundsmin.y, vert.y);
        tboundsmin.z = std::min<float>(tboundsmin.z, vert.z);
        tboundsmax.x = std::max<float>(tboundsmax.x, vert.x);
        tboundsmax.y = std::max<float>(tboundsmax.y, vert.y);
        tboundsmax.z = std::max<float>(tboundsmax.z, vert.z);

    }

    boundsMin = tboundsmin;
    boundsMax = tboundsmax;

    if (ForceUpdateCollider) {
        triangleVerts.clear();
        if (vertices.size() % 24 != 0)std::exception("Error!");
        for (int i = 0; i < vertices.size(); i += 24) {
            Triangles tri;
            tri.a = Cvector::CVector(fv[i], fv[i + 1], fv[i + 2]);
            tri.b = Cvector::CVector(fv[i + 8], fv[i + 9], fv[i + 10]);
            tri.c = Cvector::CVector(fv[i + 16], fv[i + 17], fv[i + 18]);
            triangleVerts.push_back(tri);

        }

    }
}

HitResult Mesh::rawIntersection(Cvector::CVector start, Cvector::CVector end) {
    {
        float tMin = 0;    // tMin设为0，可以有效过滤掉内部情况
        float tMax = std::numeric_limits<float>::max();

        Cvector::CVector dir = end - start;
        dir.Normalize();

        HitResult res;
        res.isHit = false;

        for (int axis = 0; axis < 3; axis++) {
            if (std::abs(dir[axis]) < std::numeric_limits<float>::epsilon()) {  // 射线方向与某个轴向平行
                if (start[axis] > boundsMax[axis] || start[axis] < boundsMin[axis]) {  // 起点同时在Bound于该轴的两侧，则不相交
                    return res;
                }
                continue;
            }
            double invD = 1 / dir[axis];
            double tNear = invD * (boundsMin[axis] - start[axis]);  // 计算射线起点到平面的斜向距离
            double tFar = invD * (boundsMax[axis] - start[axis]);
            if (tNear > tFar) {
                std::swap(tNear, tFar);
            }
            if (tNear > tMin) {
                tMin = tNear;
            }
            if (tFar < tMax) {
                tMax = tFar;
            }
            if (tMin > tMax) {
                return res;
            }
        }
        res.distance = tMin;
        res.hitPos = dir * tMin;
        res.mesh = this;
        res.isHit = true;

        return res;
    }
}

HitResult Mesh::checkIntersection(Cvector::CVector start, Cvector::CVector end, bool rawQuery) {
    Cvector::CVector tres;
    HitResult res;
    float dist = std::numeric_limits<float>::max();
    res.isHit = false;

    HitResult rawHit = rawIntersection(start, end);
    if (!rawHit.isHit)return res;
    if (rawQuery)return res;

    for (auto tri : triangleVerts) {
        if (tri.intersectsLine(start, end, tres)) {
            res.isHit |= true;
            float tdist = (tres - start).len();
            if (tdist < dist) {
                res.hitPos = tres;
                res.distance = tdist;
            }
            res.mesh = this;
        };
    }
    return res;
}