#pragma once

#include "Mesh.h"
#include <memory>

class Model
{
private:
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<Texture> m_loaded_textures;
	std::string m_directory;

   	void load_model(std::string path);
   	void process_node(aiNode* node, const aiScene* scene);
   	Mesh* process_mesh(aiMesh* mesh, const aiScene* scene);
   	std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type);

public:
	Model(std::string path);

	void draw(std::shared_ptr<Program> program);
};