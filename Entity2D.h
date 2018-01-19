#ifndef ENTITY_2D
#define ENTITY_2D

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx\transform.hpp>
#include <GL\glew.h>
#include "Shader.h"

#define BUFFER_OFFSET(a) ((void*)(a))

struct Properties2D
{
	glm::vec2 position;
	glm::vec2 rotation;
	glm::vec2 size;
	glm::vec3 color;
};

struct VertexData
{
	const GLfloat* vPositions;
	const GLsizei vpSize;
	const GLfloat* vColors;
	const GLsizei vcSize;
	const GLushort* eIndices;
	const GLsizei viSize;
	
	const GLenum mode;
	const GLuint indices;
};

class Entity2D
{
private:
	Properties2D m_Properties;

	GLint m_Program;
	GLuint m_Vao;
	GLenum m_Mode;
	GLuint m_Indices;
public:
	Entity2D(Properties2D properties);
	Entity2D(ShaderInfo* shaders, VertexData vert, Properties2D prop);
	
	GLenum getMode() const;
	GLuint getIndicies() const;
	
	void setUniform2f(const GLchar* name, glm::vec2 vector);
	void setUniformMat4(const GLchar* name, glm::mat4 matrix);

	void bind() const;
	void unbind() const;
private:
	bool init(ShaderInfo* shaders, const VertexData& vert);
};
#endif