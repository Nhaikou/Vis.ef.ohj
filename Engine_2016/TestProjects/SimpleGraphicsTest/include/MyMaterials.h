#pragma once

#include "graphics\Shader.h"
#include "core\Object.h"

struct SharedShaderValues : public core::Object
{
	float totalTime = 0.0f;
	// Model view projection matrix.
	slmath::mat4 matModelViewProj;
};

class GlobalShaderUniforms : public graphics::ShaderUniforms
{
public:
	GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* sharedShaderValues = 0);
	virtual~GlobalShaderUniforms();

	virtual void getUniformLocations(graphics::Shader* shader);
	virtual void bind(graphics::Shader* shader);

private:
	const SharedShaderValues* m_sharedShaderValues;
	GLint m_id, m_id2;
};