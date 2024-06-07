#include "Agents.h"
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

    uint32_t compileShaders(const char* computePath) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::ifstream vShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // open files
            vShaderFile.open(computePath);

            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();

        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();

        // 2. compile shaders
        unsigned int compute;
        // compute shader
        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &vShaderCode, NULL);
        glCompileShader(compute);
        checkCompileErrors(compute, "COMPUTE");

        // shader Program
        uint32_t ID = glCreateProgram();
        glAttachShader(ID, compute);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        return ID;
    }

}

void Agents::compileComputeShaders() {
    computeCellProgram = compileShaders("computeCell.glsl");
    binoticSort = compileShaders("binoticSort.glsl");
}