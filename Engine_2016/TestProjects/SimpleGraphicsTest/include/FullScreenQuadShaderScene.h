#include "scene.h"
#include "graphics\Shader.h"
#include <core/log.h>
#include "MyMaterials.h"

class FullScreenQuadShaderScene : public Scene
{
public:
	FullScreenQuadShaderScene()
	{

		LOG("FullScreenQuadShaderScene construct");
		checkOpenGL();

		//m_count = 0.0f;
		//m_flow = 0.0f;
		//m_hShaderProgram = 0;

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 },
			{ "g_vTexCoord", 2 }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

		m_shader = new graphics::Shader("assets/FullScreenQuad.vertexshader", "assets/PlasmaEffect.fragmentshader", attributes, numAttributes);
		//m_sharedValues = new SharedShaderValues();
		m_material = new GlobalShaderUniforms(m_shader, &m_sharedValues);
		checkOpenGL();
	}


	virtual ~FullScreenQuadShaderScene()
	{
		//glDeleteProgram(m_hShaderProgram);
		LOG("FullScreenQuadShaderScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		//m_count += deltaTime;
		//m_flow += deltaTime;
		m_sharedValues.totalTime += deltaTime;

		if (m_count > 1.0f)
			m_count = 0.0f;

		if (m_flow > 20.0f)
			m_flow = 0.0f;

		if (m_sharedValues.totalTime > 20.0f)
			m_sharedValues.totalTime = 0.0f;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);
		checkOpenGL();

		float aspectRatio = float(esContext->width) / float(esContext->height);
		float fSize = 1.0f;
		float VertexPositions[] =
		{
			-fSize, +fSize, 0.0f, 1.0f,
			+fSize, +fSize, 0.0f, 1.0f,
			-fSize, -fSize, 0.0f, 1.0f,
			+fSize, -fSize, 0.0f, 1.0f,
		};

		float VertexColors[] =
		{ 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		};

		float TexCoordinates[] =
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f
		};

		// Clear the backbuffer and depth-buffer
		glClearColor(0.0f, m_count, m_count, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		// Set the shader program and the texture
		//glUseProgram(m_hShaderProgram);
		m_material->bind(); 
		checkOpenGL();

		// Draw the colored triangle
		glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, VertexPositions);
		checkOpenGL();
		glEnableVertexAttribArray(0);
		checkOpenGL();

		glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray(1);
		checkOpenGL();

		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, TexCoordinates);
		checkOpenGL();
		glEnableVertexAttribArray(2);
		checkOpenGL();

		m_material->getUniformLocations(m_shader);
		//glUniform1f(glGetUniformLocation(m_shader->getProgram(), "t"), m_sharedValues.totalTime);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		checkOpenGL();

		glDisableVertexAttribArray(0);
		checkOpenGL();
		glDisableVertexAttribArray(1);
		checkOpenGL();
		glDisableVertexAttribArray(2);
		checkOpenGL();
	}
private:
	float m_count;
	float m_flow;
	core::Ref<graphics::Shader>m_shader;
	
	SharedShaderValues m_sharedValues;
	core::Ref<graphics::ShaderUniforms>m_material;

	GLuint       m_hShaderProgram;

};