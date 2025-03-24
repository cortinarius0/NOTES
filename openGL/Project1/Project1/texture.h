#pragma once
#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"

class Texture {

private:
	static unsigned int instanceCount;
	unsigned int A;

public:
	static unsigned int textureCount;
	unsigned int ID;

	Texture(const char* texPath);
	static bool endsWith(const char* str, const char* suffix);

	unsigned int GetTextureID();
};