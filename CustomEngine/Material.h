#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

class Material
{
private:
	static std::map<std::string, uint32_t> mats;

protected:
	std::vector<uint32_t> VAO;
	std::vector<uint32_t> VBO;
	std::vector<uint32_t> textures;
	uint32_t VAOsize;
	uint32_t VBOsize;
	uint32_t textureSize;
	

public:

	static int hasMaterial(const char* vertex, const char* frag);
	static uint32_t registerMaterial(const char* vertex, const char* frag);
	static void setBool(uint32_t ID, const std::string& name, bool value);
	static void setInt(uint32_t ID, const std::string& name, int value);
	static void setFloat(uint32_t ID, const std::string& name, float value);
	static void readAfterBindTex(const char* file, bool isRGB, bool flip);
};

struct TextureStorage {
	std::vector<unsigned char*> datas;
	std::vector<uint32_t> textures;
};

class TextureManager {
public:
	std::map<std::string, TextureStorage> Manager;
	void upLoadTexture(std::string objectRef, std::vector<std::string>& paths);
	void BindTextures(std::string objectRef);
	void destroyTexture(std::string objectRef);
};

