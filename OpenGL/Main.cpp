#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

int main() 
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
	//		Coordinates				Color
		 0.5f,  0.5f, 0.0f,		0.8f, 0.3f, 0.02f,  // top right
		 0.5f, -0.5f, 0.0f,		0.8f, 0.3f, 0.02f, // bottom right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.6f, 0.32f,  // bottom left
		-0.5f,  0.5f, 0.0f,		0.9f, 0.45f, 0.17f   // top left 
	};

	GLuint indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	VAO vao;
	vao.bind();

	VBO vbo(vertices, sizeof(vertices));
	EBO ebo(indices, sizeof(indices));

	vao.linkAttributes(vbo, 0, 3, GL_FLOAT, 6 * (sizeof(float)), (void*)0);
	vao.linkAttributes(vbo, 1, 3, GL_FLOAT, 6 * (sizeof(float)), (void*)(3 * sizeof(float)));
	vao.unbind();
	vbo.unbind();
	ebo.unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.activate();
		glUniform1f(uniID, 0.25f);
		vao.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	shaderProgram.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}