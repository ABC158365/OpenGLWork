#pragma once





#include <glad/glad.h>
#include <map>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct Character {
    GLuint     TextureID;  
    glm::ivec2 Size;       
    glm::ivec2 Bearing;    
    GLuint     Advance;    
};




class Font

{
public:
    
    Font(int screenWidth, int screenHeight);
    int w, h;
    std::map<GLchar, Character> Characters;
    uint32_t program;
    uint32_t VAO;
    uint32_t VBO;
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

