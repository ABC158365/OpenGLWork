#include "Material.h"
#include<glad/glad.h>
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif 

namespace {

    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

	uint32_t compileShaders(const char* vertexPath, const char* fragmentPath) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        uint32_t ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return ID;
	}



}

std::map<std::string, uint32_t>Material::mats;

int Material::hasMaterial(const char* vertex, const char* frag) {
	std::string a(vertex), b(frag);
	a = a + b;
	auto res = mats.find(a);
	if (res != mats.end()) {
        return res->second;
	}
	else {
		return -1;
	}
}

uint32_t Material::registerMaterial(const char* vertex, const char* frag) {
	std::string a(vertex), b(frag);
	a = a + b;
    int res = hasMaterial(vertex, frag);
    if (res == -1) {
        mats[a] = compileShaders(vertex, frag);
        return mats[a];
    }
    else {
        return res;
    }
    
}



void Material::setBool(uint32_t ID, const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Material::setInt(uint32_t ID, const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Material::setFloat(uint32_t ID, const std::string& name, float value) 
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void  Material::readAfterBindTex(const char* file, bool isRGB, bool flip) {
    int width, height, nrchannels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(file, &width, &height, &nrchannels, 0);
    if (isRGB) {
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed tp load texure." << std::endl;
        }
    }
    else {
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed tp load texure." << std::endl;
        }
    }
    stbi_image_free(data);
}

void TextureManager::upLoadTexture(std::string objectRef, std::vector<std::string>& paths) {
    Manager[objectRef].textures.resize(paths.size());
    glGenTextures(Manager[objectRef].textures.size(), Manager[objectRef].textures.data());
    int i = 0;
    for (auto path : paths) {
        int width, height, nrchannels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrchannels, 0);
        if(data){
            if (Manager.find(objectRef) == Manager.end()) {
                TextureStorage a;
                a.datas.push_back(data);
                Manager[objectRef] = a;
            }
            else {
                Manager[objectRef].datas.push_back(data);
            }
            glBindTexture(GL_TEXTURE_2D, Manager[objectRef].textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            i++;
            stbi_image_free(data);
        }
        else {
            printf("Error: %s\n", stbi_failure_reason());
        }
    }
  
}


void TextureManager::destroyTexture(std::string objectRef) {
    if (Manager.find(objectRef) != Manager.end()) {
        for (auto data : Manager[objectRef].datas) {
            stbi_image_free(data);
        }
    }
}

void TextureManager::BindTextures(std::string objectRef) {
    if (Manager.find(objectRef) != Manager.end()) {
        for (uint32_t i = 0; i < Manager[objectRef].textures.size(); i++) {
            GLenum ref = GL_TEXTURE0 + i;
            glActiveTexture(ref);
            glBindTexture(GL_TEXTURE_2D, Manager[objectRef].textures[i]);
            //glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
}