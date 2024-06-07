#pragma once
#include <iostream>
#include <GLFW/glfw3.h>




class Attachment {
public:
	uint32_t FBO;
	uint32_t tex;
	int windowHeight;
	int windowWidth;
};

class ColorAttachment :public Attachment {
public:
	uint32_t stencilTex;
	uint32_t stencilView;
	uint32_t customTex;
	ColorAttachment(GLFWwindow* window);
};


class StencilAttachment:Attachment {
public:

	StencilAttachment(GLFWwindow* window);


};

class BufferManager
{
private:
	ColorAttachment* albedo;
	StencilAttachment* stencil;
	GLFWwindow* window;

public:
	BufferManager(GLFWwindow* w) :window(w) {};
	ColorAttachment* getAlbedo();
	~BufferManager();
};
