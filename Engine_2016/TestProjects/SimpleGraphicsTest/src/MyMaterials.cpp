#include "MyMaterials.h"

GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* sharedShaderValues) : ShaderUniforms(shader), m_sharedShaderValues(sharedShaderValues)
{
	
}

void GlobalShaderUniforms::getUniformLocations(graphics::Shader* shader)
{
	/*m_id = glGetUniformLocation(shader->getProgram(), "t");
	m_id2 = glGetUniformLocation(shader->getProgram(), "totalTime");
	m_id3 = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");*/

	// Get uniform locations
	m_ids[0] = glGetUniformLocation(shader->getProgram(), "g_matModel");
	m_ids[1] = glGetUniformLocation(shader->getProgram(), "g_matView");
	m_ids[2] = glGetUniformLocation(shader->getProgram(), "g_matProj");
	m_ids[3] = glGetUniformLocation(shader->getProgram(), "g_matModelView");
	m_ids[4] = glGetUniformLocation(shader->getProgram(), "g_matNormal");
	m_ids[5] = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
	m_ids[6] = glGetUniformLocation(shader->getProgram(), "g_lightPos");
	m_ids[7] = glGetUniformLocation(shader->getProgram(), "g_camPos");
}

void GlobalShaderUniforms::bind(graphics::Shader* shader)
{
	shader->bind();
	if (m_sharedShaderValues)
	{
		glUniformMatrix4fv(m_ids[0], 1, GL_FALSE, &m_sharedShaderValues->matModel[0][0]);
		glUniformMatrix4fv(m_ids[1], 1, GL_FALSE, &m_sharedShaderValues->matView[0][0]);
		glUniformMatrix4fv(m_ids[2], 1, GL_FALSE, &m_sharedShaderValues->matProj[0][0]);
		glUniformMatrix4fv(m_ids[3], 1, GL_FALSE, &m_sharedShaderValues->matModelView[0][0]);
		glUniformMatrix4fv(m_ids[4], 1, GL_FALSE, &m_sharedShaderValues->matNormal[0][0]);
		glUniformMatrix4fv(m_ids[5], 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);

		glUniform3f(m_ids[6], m_sharedShaderValues->lightPos.x, m_sharedShaderValues->lightPos.y, m_sharedShaderValues->lightPos.z);
		glUniform3f(m_ids[7], m_sharedShaderValues->camPos.x, m_sharedShaderValues->camPos.y, m_sharedShaderValues->camPos.z);

	}

	/*if (m_sharedShaderValues)
	{
		glUniform1f(m_id, m_sharedShaderValues->totalTime);
		glUniform1f(m_id2, m_sharedShaderValues->totalTime);
		if (m_id3 >= 0)
		{
			glUniformMatrix4fv(m_id3, 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);
		}
	}*/
}

GlobalShaderUniforms::~GlobalShaderUniforms()
{

}

SimpleMaterialUniforms::SimpleMaterialUniforms(graphics::Shader* shader, SharedShaderValues* sharedValues)
	: ShaderUniforms(shader), m_globalShaderUniforms(new GlobalShaderUniforms(shader, sharedValues))
{

}

void SimpleMaterialUniforms::getUniformLocations(graphics::Shader* shader)
{
	m_globalShaderUniforms->getUniformLocations(shader);
	m_materialAmbientLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vAmbient");
	m_materialDiffuseLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vDiffuse");
	m_materialSpecularLoc = glGetUniformLocation(shader->getProgram(), "g_Material.vSpecular");
}

void SimpleMaterialUniforms::bind(graphics::Shader* shader)
{
	shader->bind();
	m_globalShaderUniforms->bind(shader);
	glUniform4fv(m_materialAmbientLoc, 1, &vAmbient.x);
	glUniform4fv(m_materialDiffuseLoc, 1, &vDiffuse.x);
	glUniform4fv(m_materialSpecularLoc, 1, &vSpecular.x);
}

SimpleMaterialUniforms::~SimpleMaterialUniforms()
{

}