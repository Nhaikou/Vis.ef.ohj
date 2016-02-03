#pragma once

#include "graphics\Shader.h"
#include "core\Object.h"

struct SharedShaderValues : public core::Object
{
	float totalTime = 0.0f;
	// Model view projection matrix.
	slmath::mat4 matModel;				// Model matrix (object world matrix)
	slmath::mat4 matView;				// View matrix (inverse of camera world matrix)
	slmath::mat4 matProj;				// Projision matrix of the camera
	slmath::mat4 matModelView;			// Model view matrix. used to transform position vertices to camera space
	slmath::mat4 matNormal;				// Model view matrix. Used to transform normal/binormal/tangent vertices to camera space.
	slmath::mat4 matModelViewProj;		// Model view projection matrix. Used to tranform position vertices to clip space.
	slmath::vec3 lightPos;				// World position of point light.
	slmath::vec3 camPos;				// World position of camera.

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
	GLint m_ids[8];
};

class SimpleMaterialUniforms : public graphics::ShaderUniforms
{
public:
	slmath::vec4 vAmbient;		// Ambient color of the material (rgba)
	slmath::vec4 vDiffuse;		// Ambient color of the material (rgba)
	slmath::vec4 vSpecular;		// Specular color of the material (rgb). Specular exponent (a)

public:
	SimpleMaterialUniforms(graphics::Shader* shader, SharedShaderValues* sharedValues = 0);
	virtual~SimpleMaterialUniforms();

	virtual void getUniformLocations(graphics::Shader* shader);
	virtual void bind(graphics::Shader* shader);

private:
	core::Ref<GlobalShaderUniforms> m_globalShaderUniforms;
	GLint m_materialAmbientLoc;
	GLint m_materialDiffuseLoc;
	GLint m_materialSpecularLoc;
};