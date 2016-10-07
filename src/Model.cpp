#include "Model.h"
#include <SOIL/SOIL.h>

GLint TextureFromFile(const char* path, std::string directory)
{
    std::string filename(path);
    filename = directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width,height, channels;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGB);

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);	

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureID;
}

Model::Model(std::string path)
{
	this->load_model(path);
} 

void Model::load_model(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
	   std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	   return;
	}

	this->m_directory = path.substr(0, path.find_last_of("/"));
	this->process_node(scene->mRootNode, scene);
}
   	
void Model::process_node(aiNode* node, const aiScene* scene)
{
	for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::shared_ptr<Mesh> mesh_ptr(this->process_mesh(mesh, scene));
        this->m_meshes.push_back(mesh_ptr);			
    }

    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->process_node(node->mChildren[i], scene);
    }
}

Mesh* Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<GLuint> indices;

	//process verticies
	for(GLuint i = 0; i < mesh->mNumVertices; i++)
	{
	    Vertex vertex;
	    // Positions
	   	glm::vec3 vector; 
	    vector.x = mesh->mVertices[i].x;
	    vector.y = mesh->mVertices[i].y;
	    vector.z = mesh->mVertices[i].z;
	    vertex.position = vector;
	    // Normals
	    vector.x = mesh->mNormals[i].x;
	    vector.y = mesh->mNormals[i].y;
	    vector.z = mesh->mNormals[i].z;
	    vertex.normal = vector;

	    // Texture Coordinates
	    vertex.texture_coords = glm::vec2(0.0f, 0.0f);
	    if(mesh->mTextureCoords[0])
	    {
	        vertex.texture_coords.x = mesh->mTextureCoords[0][i].x; 
	        vertex.texture_coords.y = mesh->mTextureCoords[0][i].y;
	    }

	    vertices.push_back(vertex);
	}

	//process indicies
	for(GLuint i = 0; i < mesh->mNumFaces; i++)
	{
	    aiFace face = mesh->mFaces[i];

	    for(GLuint j = 0; j < face.mNumIndices; j++)
	    {
	        indices.push_back(face.mIndices[j]);
	    }
	}

	// Process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // 1. Diffuse maps
    std::vector<Texture> diffuse_aps = this->load_material_textures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuse_aps.begin(), diffuse_aps.end());
    // 2. Specular maps
    std::vector<Texture> specular_aps = this->load_material_textures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specular_aps.begin(), specular_aps.end());

	return new Mesh(vertices, textures, indices); 
}

std::vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;

    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        GLboolean skip = false;
        for(GLuint j = 0; j < this->m_loaded_textures.size(); j++)
        {
        	Texture texture = this->m_loaded_textures[j];
            if(texture.path == str)
            {
                textures.push_back(texture);
                skip = true;
                break;
            }
        }

        if(!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->m_directory);
            texture.type = type == aiTextureType_DIFFUSE ? Diffuse : Specular;
            texture.path = str;
            textures.push_back(texture);
            this->m_loaded_textures.push_back(texture);
        }
    }

    return textures;
}

void Model::draw(std::shared_ptr<Program> program)
{
	for(GLuint i = 0; i < this->m_meshes.size(); i++)
	{
        this->m_meshes[i]->draw(program);
	}
}