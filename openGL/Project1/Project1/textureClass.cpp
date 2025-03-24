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
	if (!str || !suffix) return false; // �������Ϊ��ָ�룬ֱ�ӷ��� false
	size_t strLen = strlen(str);       // ��ȡ���ַ����ĳ���
	size_t suffixLen = strlen(suffix); // ��ȡ��׺�ĳ���

	// �����׺���ȴ������ַ������ȣ�������ƥ��
	if (suffixLen > strLen) return false;

	// �Ƚ����ַ�����ĩβ�������׺
	return strncmp(str + strLen - suffixLen, suffix, suffixLen) == 0;
}