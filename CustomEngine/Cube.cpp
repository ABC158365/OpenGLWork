#include "Cube.h"
void InstancedCube::draw(Camera* cam) {
    glBindVertexArray(VAO);
    glUseProgram(matID);
    int a = glGetUniformLocation(matID, "ourColor");
    float green = sin(glfwGetTime());
    Material::setFloat(matID, "offset", green / 4);
    glUniform4f(a, 0.0f, green, 0.0f, 1.0f);
    Material::setFloat(matID, "par", (green + 1) / 2.0);

    Material::setInt(matID, "texture1", 0);
    Material::setInt(matID, "texture2", 1);
    Material::setInt(matID, "type", hueshift);

    Cvector::CVector lightpos(5.0f, 10.0f, 10.0f);

    //glm::vec3 lightpos = glm::vec3(5.0f, 10.0f, 10.0f);
    glUniform3fv((glGetUniformLocation(matID, "lightPos")), 1, lightpos.value_ptr());
    glUniform3fv((glGetUniformLocation(matID, "viewPos")), 1,cam->GetPos().value_ptr());

    Cmatrix::Mat4x4 vp = cam->GetViewMatrix(true);
    vp = cam->getProjectionMatrix() * vp;
    glUniformMatrix4fv(glGetUniformLocation(matID, "mvp"), 1, GL_FALSE, vp.value_ptr());

    auto displayColorLocation = glGetUniformLocation(matID, "color");

    updateBuffers(regionA);
    glUniform3fv(displayColorLocation, 1, (regionA->getColorInfo()).value_ptr());
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, regionA->getTransforms().size());
    
    updateBuffers(regionB1);
    glUniform3fv(displayColorLocation, 1, (regionB1->getColorInfo()).value_ptr());
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, regionB1->getTransforms().size());
    
    
    updateBuffers(regionB2);
    glUniform3fv(displayColorLocation, 1, (regionB2->getColorInfo()).value_ptr());
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, regionB2->getTransforms().size());
    
    
    updateBuffers(regionC);
    glUniform3fv(displayColorLocation, 1, (regionC->getColorInfo()).value_ptr());
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, regionC->getTransforms().size());

}

void InstancedCube::bindBuffers(){

    VBOsize = 2;

    glGenVertexArrays(1, &VAO);
    VBO.resize(VBOsize);
    glGenBuffers(VBOsize, VBO.data());
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
 
    textures.resize(2);
    glGenTextures(2, textures.data());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Material::readAfterBindTex("container.jpg", true, false);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Material::readAfterBindTex("awesomeface.png", false, false);

}

void InstancedCube::updateBuffers(Region* region) {
    glBindVertexArray(VAO);

    transforms.clear();
    transforms.insert(transforms.end(), region->getTransforms().begin(), region->getTransforms().end());

    std::size_t vec4Size = sizeof(Cvector::CVector4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(Cmatrix::Mat4x4), transforms.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(0));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));


}

void GridPlane::bindBuffers(){
    VBOsize = 1;
    glGenVertexArrays(1, &VAO);
    VBO.resize(VBOsize);
    glGenBuffers(VBOsize, VBO.data());
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GridPlane::draw(Camera* cam) {
    glBindVertexArray(VAO);
    glUseProgram(matID);
    Cmatrix::Mat4x4 rot;
    rot = Cmatrix::rotate(rot, (float)Cmath::radians(90.0), Cvector::CVector(1.0f, 0.0f, 0.0f));
    rot = Cmatrix::scale(rot, Cvector::CVector(50, 50, 50));
   
    Cmatrix::Mat4x4 vp = cam->GetViewMatrix(true);
    vp = cam->getProjectionMatrix() * vp * rot;
    glUniformMatrix4fv(glGetUniformLocation(matID, "mvp"), 1, GL_FALSE, vp.value_ptr());
    glUniformMatrix4fv(glGetUniformLocation(matID, "model"), 1, GL_FALSE, rot.value_ptr());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}