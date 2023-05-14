#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <utility>

#include "Shader.h"
#include "Texture2D.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

struct Vertex2D{
	Vertex2D(const glm::vec2 position_,const glm::vec2 texCoords_):position(position_),texCoords(texCoords_){};
	glm::vec2 position;
	glm::vec2 texCoords;
};

class Mesh2D {
public:
	// mesh data
	std::vector<Vertex2D> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2D> textures;

	Mesh2D(){};
	Mesh2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices,std::vector<Texture2D> textures);

	void Draw(const Shader& shader) const;

	// render data
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
private:
	void setupMesh();
};