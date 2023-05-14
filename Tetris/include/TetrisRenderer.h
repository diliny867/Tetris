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
	float blockSize = 25;
	glm::vec2 fieldOffset = {0,0};
	Texture2D emptyBlockTexture;
	Texture2D redBlockTexture;
	Texture2D blueBlockTexture;
	Texture2D yellowBlockTexture;
	Texture2D greenBlockTexture;
	Texture2D brownBlockTexture;
	Texture2D violetBlockTexture;
	Texture2D orangeBlockTexture;
	Mesh2D emptyBlockMesh;
	Mesh2D redBlockMesh;
	Mesh2D blueBlockMesh;
	Mesh2D yellowBlockMesh;
	Mesh2D greenBlockMesh;
	Mesh2D brownBlockMesh;
	Mesh2D violetBlockMesh;
	Mesh2D orangeBlockMesh;
	inline static std::vector<Vertex2D> squareVertices = {Vertex2D({0,0,0},{0,0}),Vertex2D({1,0,0},{1,0}),Vertex2D({0,1,0},{0,1}),Vertex2D({1,1,0},{1,1})};
	inline static std::vector<unsigned int> squareIndices = {0,1,2,1,2,3};
	std::vector<glm::vec3> fieldBlocksPositions;
public:
	Tetris tetris;
	TetrisRenderer() = default;
	void Init(){
		fieldOffset ={(800.0f-FIELD_WIDTH*blockSize)/2.0f,(600.0f-FIELD_HEIGHT*blockSize)/2.0f};
		emptyBlockTexture = Texture2D("resources/empty_block.png",TextureType::diffuse);
		redBlockTexture = Texture2D("resources/red_block.jpg",TextureType::diffuse);
		blueBlockTexture = Texture2D("resources/blue_block.jpg",TextureType::diffuse);;
		yellowBlockTexture = Texture2D("resources/yellow_block.jpg",TextureType::diffuse);
		greenBlockTexture = Texture2D("resources/green_block.jpg",TextureType::diffuse);
		brownBlockTexture = Texture2D("resources/brown_block.jpg",TextureType::diffuse);
		violetBlockTexture = Texture2D("resources/violet_block.jpg",TextureType::diffuse);
		orangeBlockTexture = Texture2D("resources/orange_block.jpg",TextureType::diffuse);
		emptyBlockMesh = Mesh2D(squareVertices,squareIndices, {emptyBlockTexture});
		redBlockMesh = Mesh2D(squareVertices,squareIndices, {redBlockTexture});
		blueBlockMesh = Mesh2D(squareVertices,squareIndices, {blueBlockTexture});
		yellowBlockMesh = Mesh2D(squareVertices,squareIndices, {yellowBlockTexture});
		greenBlockMesh = Mesh2D(squareVertices,squareIndices, {greenBlockTexture});
		brownBlockMesh = Mesh2D(squareVertices,squareIndices, {brownBlockTexture});
		violetBlockMesh = Mesh2D(squareVertices,squareIndices, {violetBlockTexture});
		orangeBlockMesh = Mesh2D(squareVertices,squareIndices, {orangeBlockTexture});
		for(int y=0;y<FIELD_HEIGHT;y++) {
			for(int x=0;x<FIELD_WIDTH;x++) {
				fieldBlocksPositions.push_back({(float)x*blockSize,(float)y*blockSize,0.0f});
			}
		}
	}
	void Draw(const Shader& shader) const {
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		for(int y=0;y<FIELD_HEIGHT;y++) {
			for( int x=0;x<FIELD_WIDTH;x++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model,glm::vec3(fieldOffset,0.0f)+fieldBlocksPositions[x+y*FIELD_WIDTH]);
				model = glm::scale(model,{blockSize,blockSize,0.0f});
				shader.setMat4("model",model);
				switch(tetris.GetBlock(x,y)) {
				case EMPTY_BLOCK:
					emptyBlockMesh.Draw(shader);
					break;
				case I_BLOCK:
					redBlockMesh.Draw(shader);
					break;
				case O_BLOCK:
					blueBlockMesh.Draw(shader);
					break;
				case L_BLOCK:
					yellowBlockMesh.Draw(shader);
					break;
				case J_BLOCK:
					greenBlockMesh.Draw(shader);
					break;
				case S_BLOCK:
					brownBlockMesh.Draw(shader);
					break;
				case Z_BLOCK:
					violetBlockMesh.Draw(shader);
					break;
				case T_BLOCK:
					orangeBlockMesh.Draw(shader);
					break;
				default:
					break;
				}
			}
		}
	}
	void UpdateResolution(const float width,const float height) {
		fieldOffset ={(width-FIELD_WIDTH*blockSize)/2.0f,(height-FIELD_HEIGHT*blockSize)/2.0f};
	}
};