#include "MyMaterials.h"

GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* sharedShaderValues) : ShaderUniforms(shader), m_sharedShaderValues(sharedShaderValues)
{
	
}

void GlobalShaderUniforms::getUniformLocations(graphics::Shader* shader)
{
	m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
	m_id2 = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
}

void GlobalShaderUniforms::bind(graphics::Shader* shader)
{
	shader->bind();
	if (m_sharedShaderValues)
	{
		if (m_id2 >= 0)
		{
			glUniformMatrix4fv(m_id2, 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);
		}
	}
}

GlobalShaderUniforms::~GlobalShaderUniforms()
{

}