#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices,
		   std::vector<Texture> textures,
		   std::vector<GLuint> indices)
{
	this->m_vertices = vertices;
	this->m_textures = textures;
	this->m_indices  = indices;

	this->setup();
}

Mesh::~Mesh()
{	
	glDeleteVertexArrays(1, &this->m_vao);
    glDeleteBuffers(1, &this->m_vbo);
    glDeleteBuffers(1, &this->m_ebo);
}

void Mesh::draw(std::shared_ptr<Program> program)
{
	//activate textures
	GLuint diffuse_number  = 1;
    GLuint specular_number = 1;

    std::string diffuse_name  = "diffuse";
    std::string specular_name = "specular";
    std::string material_prefix = "material.";

	for (GLuint i = 0; i < this->m_textures.size(); i++)
	{
		std::string texture_uniform_name(material_prefix);
	    TextureType texture_type = this->m_textures[i].type;

	    if(texture_type == Diffuse)
	    {
	        texture_uniform_name += diffuse_name + std::to_string(diffuse_number);
	        diffuse_number++;
	    }
	    else if(texture_type == Specular)
	    {
			texture_uniform_name += specular_name + std::to_string(specular_number);
	        specular_number++;
		}

		glActiveTexture(GL_TEXTURE0 + i);
    	glUniform1f(glGetUniformLocation(program->get_handle(), texture_uniform_name.c_str()), i);
    	glBindTexture(GL_TEXTURE_2D, this->m_textures[i].id);
	}

	glUniform1f(glGetUniformLocation(program->get_handle(), (material_prefix + "shininess").c_str()), 32);

	glBindVertexArray(this->m_vao);
	glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
    	glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::setup()
{
	glGenVertexArrays(1, &this->m_vao);
    glGenBuffers(1, &this->m_vbo);
    glGenBuffers(1, &this->m_ebo);
  
    glBindVertexArray(this->m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(Vertex), &this->m_vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(GLuint), &this->m_indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_coords));

    glBindVertexArray(0);  
}