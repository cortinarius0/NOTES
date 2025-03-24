#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "shader.h"
#include "texture.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float* a, glm::vec3* cameraPos, glm::vec3* targetPos);
void printMat4(const glm::mat4& matrix);
std::string readShaderFile(const std::string& filePath);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float alphaRatio = 0.5;
	int screenWidth = 300;
	int screenHeight = 225;

	

	GLFWwindow* window = glfwCreateWindow( screenWidth, screenHeight, "opengl!YES!", NULL, NULL);


	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << nrAttributes << std::endl;


	float vertices[] = {
		// 位置              // 纹理坐标
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // 0
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 1
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // 2
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // 3
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // 4
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // 5
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // 6
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f   // 7
	};

	float vertices2[] = {
	1.5f,  0.5f,  0.0f,   // 
	0.5f, -0.5f,  0.0f,   // 
	0.5f,  0.5f,  0.0f    // 
	};

	unsigned int indices[] = {
		// 前面 (z = -0.5)
		0, 1, 2,
		2, 3, 0,

		// 后面 (z = 0.5)
		4, 5, 6,
		6, 7, 4,

		// 左面 (x = -0.5)
		7, 3, 0,
		0, 4, 7,

		// 右面 (x = 0.5)
		5, 1, 2,
		2, 6, 5,

		// 底面 (y = -0.5)
		0, 1, 5,
		5, 4, 0,

		// 顶面 (y = 0.5)
		3, 2, 6,
		6, 7, 3
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);


#pragma region Shader_Part

	Shader ourShader("vertex_shader.glsl", "fragment_shader.glsl");

#pragma endregion

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glViewport(0, 0, 300, 225);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

#pragma region Texture

	Texture newTexture("resource/img/sky.png");

	Texture newTexture2("resource/img/arrow_tex.png");

#pragma endregion

#pragma region Transformation

	//设置一个基础变换，然后对其进行MVP变换
	glm::mat4 vbasic = glm::mat4(1.0f);

	glm::mat4 modelMatrix = glm::rotate( vbasic, glm::radians(-55.0f), glm::vec3(1, 0, 0));

	glm::mat4 viewMatrix = glm::translate(vbasic, glm::vec3(0, 0, -3));

	//摄像机以及目标坐标
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);

	//摄像机位移
	glm::mat4 cameraTranslate = glm::mat4(
		1, 0, 0, -cameraPos.x,
		0, 1, 0, -cameraPos.y,
		0, 0, 1, -cameraPos.z,
		0, 0, 0, 1
	);

	cameraTranslate = glm::transpose(cameraTranslate);

	//构建摄像机的三个坐标轴
	glm::vec3 cameraBack = glm::normalize(cameraPos - targetPos);
	glm::vec3 upDir = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(upDir, cameraBack));
	glm::vec3 cameraTop =glm::normalize( glm::cross(cameraBack, cameraRight));
	
	glm::mat4 lookAt = glm::mat4(
		cameraRight.x, cameraRight.y, cameraRight.z, 0,
		cameraTop.x, cameraTop.y, cameraTop.z, 0,
		cameraBack.x, cameraBack.y, cameraBack.z, 0,
		0, 0, 0, 1
	);
	lookAt = glm::transpose(lookAt);

	lookAt = lookAt * cameraTranslate;

	float fovy = glm::radians(45.0f);
	float aspect = float(screenWidth) / float(screenHeight);
	float zNear = 0.1f;
	float zFar = 1000.0f;

	float t = zNear * std::tan(fovy / 2.0f);
	float b = -t;
	float r = aspect * t;
	float l = -r;
	float n = zNear;
	float f = zFar;

	glm::mat4 projectionMatrix = glm::mat4(
		n / r, 0, 0, 0,
		0, n / t, 0, 0,
		0, 0, -(f + n) / (f - n), -2 * f * n / (f - n),
		0, 0, -1, 0
	);

	projectionMatrix = glm::transpose(projectionMatrix);
	//glm::mat4 projectionMatrix2 = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

	//printMat4(projectionMatrix);
	//printMat4(projectionMatrix2);

#pragma endregion

#pragma region TEST
	glEnable(GL_DEPTH_TEST);
	
#pragma endregion

	ourShader.use();
	ourShader.setInt("tex1", 0);
	ourShader.setInt("tex2", 1);

	while (!glfwWindowShouldClose(window)) {

		processInput(window, &alphaRatio, &cameraPos, &targetPos);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, newTexture.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, newTexture2.ID);

		ourShader.use();
		ourShader.setFloat("alpha", alphaRatio);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "Trans");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		modelMatrix = glm::mat4(1.0f); 
		//float angle = (float)glfwGetTime() * glm::radians(55.0f);
		float angle = glm::radians(55.0f);
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.5f, 1.0f, 0.0f));

		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "Model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		cameraTop = glm::normalize(glm::cross(
			glm::normalize(cameraPos - targetPos),
			glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(cameraPos - targetPos)))
		));
		lookAt = glm::lookAt(cameraPos, targetPos, cameraTop);
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "View");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(lookAt));

		unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "Projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glBindVertexArray(VAO);
		
		glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window, float* a, glm::vec3* cameraPos, glm::vec3* targetPos ) {
	float cameraSpeed = 7.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		if (*a <= 0.95f) {
			*a += 0.001f;
		}
		std::cout << *a << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (*a >= 0.05f) {
			*a -= 0.001f;
		}
		std::cout << *a << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos->z -= cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos->z += cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos->x -= cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos->x += cameraSpeed;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void printMat4(const glm::mat4& matrix) {
	for (int i = 0; i < 4; ++i) { // 遍历行
		for (int j = 0; j < 4; ++j) { // 遍历列
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl; // 每行结束后换行
	}
}

std::string readShaderFile(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "cannot open file" << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}





