#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
class Shaders
{
public:
    unsigned int ID;
    Shaders(const char* vertexPath, const char* fragmentPath);
    Shaders() {};
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    unsigned int  CompileShaders(const char* vertex_file, const char* fragment_file);
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

