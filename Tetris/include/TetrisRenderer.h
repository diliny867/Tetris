#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../include/GL/Time.h"
#include "../include/GL/VBO.h"
#include "../include/GL/VAO.h"
#include "../include/GL/EBO.h"
#include "../include/GL/Shader.h"
#include "../include/GL/Texture2D.h"
#include "../include/GL/Mesh2D.h"

#include "Tetris.h"

class TetrisRenderer {
private:
	int BlockSize = 40;
	glm::vec2 FieldOffset = {50,50};
	Texture2D BlockTexture;
	Mesh2D BlockMesh;
	inline static std::vector<Vertex2D> squareVertices = {Vertex2D({0,0},{0,0}),Vertex2D({1,0},{1,0}),Vertex2D({0,1},{0,1}),Vertex2D({1,1},{1,1})};
	inline static std::vector<unsigned int> squareIndices = {0,1,3,1,2,3};
public:
	TetrisRenderer() = default;
	void Init(){
		BlockTexture = Texture2D("resources/empty_block.jpg",TextureType::diffuse);
		BlockMesh = Mesh2D(squareVertices,squareIndices, {BlockTexture});
	}
	void Draw(const Shader& shader) const {
		shader.use();
		BlockMesh.Draw(shader);
	}
};