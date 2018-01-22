#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "Shader.h"
#include <vector>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx\transform.hpp>
#include <gl\glew.h>

#define BUFFER_OFFSET(a) ((void*)(a))

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
enum UniformType : unsigned char
{
	Vec2,
	Vec3,
	Mat4
};

struct Uniform
{
	UniformType type;
	const GLchar* name;
	glm::vec3 vec3;
	glm::vec2 vec2;
	glm::mat4 mat4;
};


class RenderObject
{
private:
	GLint m_Program;
	GLuint m_Vao;
	GLenum m_Mode;
	GLuint m_Indices;

	std::vector<Uniform> m_Uniforms;
public:

	GLenum getMode() const;
	GLuint getIndicies() const;

	void setUniform2f(const GLchar* name, glm::vec2 vector);
	void setUniform3f(const GLchar* name, glm::vec3 vector);
	void setUniformMat4(const GLchar* name, glm::mat4 matrix);

	virtual void updateUniforms() = 0;

	virtual glm::vec3 getPos3fv() const;

	virtual void rotate(float angle, const glm::vec3& axis);
	virtual void translate(const glm::vec3& vector);

	void prepare();
	void unbind();

protected:
	bool init(ShaderInfo* shaders, const VertexData& vert);
};

#endif // !RENDER_OBJECT_H

