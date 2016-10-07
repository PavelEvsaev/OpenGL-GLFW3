#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Program.h"

enum TextureType
{
	Diffuse,
	Specular
};

struct Texture
{
	GLuint id;
	TextureType type;
	aiString path;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coords;
};

class Mesh
{
private:
	GLuint m_vao, m_vbo, m_ebo;
	void setup();

public:
	std::vector<Vertex> m_vertices;
	std::vector<Texture> m_textures;
	std::vector<GLuint> m_indices;

	Mesh(std::vector<Vertex> vertices,
		std::vector<Texture> textures,
		std::vector<GLuint>  indices);
	~Mesh();

	void draw(std::shared_ptr<Program> program);
};