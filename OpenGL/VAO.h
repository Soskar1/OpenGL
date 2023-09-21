#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
	GLuint ID;
	VAO();

	void linkVBO(VBO vbo, GLuint layout);
	void bind();
	void unbind();
	void destroy();
};

#endif