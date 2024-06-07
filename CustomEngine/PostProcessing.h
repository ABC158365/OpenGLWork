#pragma once
#include <iostream>



class PostProcessing
{
private:
    uint32_t VAO;
    uint32_t VBO;
    uint32_t program;
public:
    PostProcessing();

    void render(uint32_t colorAttachment, uint32_t colorAttachment2, uint32_t stencil);

};

