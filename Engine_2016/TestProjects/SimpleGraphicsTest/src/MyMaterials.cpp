#include "MyMaterials.h"

GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* sharedShaderValues) : ShaderUniforms(shader), m_sharedShaderValues(sharedShaderValues)
{
	
}

void GlobalShaderUniforms::getUniformLocations(graphics::Shader* shader)
{
	m_id = glGetUniformLocation(shader->getProgram(), "t");
	m_id2 = glGetUniformLocation(shader->getProgram(), "totalTime");
	m_id3 = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
}

void GlobalShaderUniforms::bind(graphics::Shader* shader)
{
	shader->bind();
	if (m_sharedShaderValues)
	{
		glUniform1f(m_id, m_sharedShaderValues->totalTime);
		glUniform1f(m_id2, m_sharedShaderValues->totalTime);
		if (m_id3 >= 0)
		{
			glUniformMatrix4fv(m_id3, 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);
		}
	}
}

GlobalShaderUniforms::~GlobalShaderUniforms()
{

}