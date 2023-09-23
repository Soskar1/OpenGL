#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] =
{
	//		Coordinates				Color			  TexCoordinates
		 -0.5f, 0.0f, 0.5f,		0.83f, 0.7f, 0.44f,		0.0f, 0.0f,
		 -0.5f, 0.0f, -0.5f,	0.83f, 0.7f, 0.44f,		5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,		0.83f, 0.7f, 0.44f,		0.0f, 0.0f,
		 0.5f, 0.0f, 0.5f,		0.83f, 0.7f, 0.44f,		5.0f, 0.0f,
		 0.0f, 0.8f, 0.0f,		0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main() 
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	VAO vao;
	vao.bind();

	VBO vbo(vertices, sizeof(vertices));
	EBO ebo(indices, sizeof(indices));

	vao.linkAttributes(vbo, 0, 3, GL_FLOAT, 8 * (sizeof(float)), (void*)0);
	vao.linkAttributes(vbo, 1, 3, GL_FLOAT, 8 * (sizeof(float)), (void*)(3 * sizeof(float)));
	vao.linkAttributes(vbo, 2, 2, GL_FLOAT, 8 * (sizeof(float)), (void*)(6 * sizeof(float)));
	vao.unbind();
	vbo.unbind();
	ebo.unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	Texture brick("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brick.texUnit(shaderProgram, "tex0", 0);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.activate();

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform1f(uniID, 0.5f);
		brick.bind();
		vao.bind();

		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	shaderProgram.destroy();
	brick.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}