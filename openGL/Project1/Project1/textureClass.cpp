#include "texture.h"

unsigned int Texture::instanceCount = 0;

Texture::Texture(const char* texPath) {

	A = instanceCount;
	instanceCount++;

	ID = 0;

	glActiveTexture(instanceCount - 1);
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::string errorMessage;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texPath, &width, &height, &nrChannels, 0);

	if (data == nullptr) {
		errorMessage = "Failed to load image: Reason" + (std::string)stbi_failure_reason();
	}

	if (endsWith(texPath, ".jpg")) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//deliver data to GPU
		
	}
	else if (endsWith(texPath, ".png")) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//deliver data to GPU
		
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}



unsigned int Texture::GetTextureID(){
	return 0;
}

bool Texture::endsWith(const char* str, const char* suffix) {
	if (!str || !suffix) return false; // 如果输入为空指针，直接返回 false
	size_t strLen = strlen(str);       // 获取主字符串的长度
	size_t suffixLen = strlen(suffix); // 获取后缀的长度

	// 如果后缀长度大于主字符串长度，不可能匹配
	if (suffixLen > strLen) return false;

	// 比较主字符串的末尾部分与后缀
	return strncmp(str + strLen - suffixLen, suffix, suffixLen) == 0;
}