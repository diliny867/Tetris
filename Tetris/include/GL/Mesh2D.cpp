#include "Mesh2D.h"

Mesh2D::Mesh2D(std::vector<Vertex2D> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures) {
	this->vertices = std::move(vertices);
	this->indices  = std::move(indices);
	this->textures = std::move(textures);
	setupMesh();
}

void Mesh2D::setupMesh() { 
	VAO::generate(vao);
	VBO::generate(vbo);
	EBO::generate(ebo);
	
	VAO::bind(vao);
	VBO::bind(vbo);
	VBO::setData(vbo, vertices.size() * sizeof(Vertex2D), (vertices).data(), GL_STATIC_DRAW);
	
	EBO::bind(ebo);
	EBO::setData(ebo, indices.size() * sizeof(unsigned int), (indices).data(), GL_STATIC_DRAW);
	
	VAO::addAttrib(vao, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
	VAO::addAttrib(vao, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));
}

void Mesh2D::Draw(const Shader& shader) const{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		std::string name;
		switch (textures[i].type) {
		case TextureType::diffuse:
			name = "texture_diffuse" + std::to_string(diffuseNr++);
			break;
		case TextureType::specular:
			name = "texture_specular" + std::to_string(specularNr++);
			break;
		case TextureType::tNormal:
			name = "texture_normal" + std::to_string(normalNr++);
			break;
		case TextureType::height:
			name = "texture_height" + std::to_string(heightNr++);
			break;
		}
		shader.setInt(name, i);
		textures[i].activate(i);
	}
	VAO::bind(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	VAO::unbind();
	Texture2D::deactivate();
}
