#pragma once
#include "scene.h"
#include "graphics\Mesh.h"
#include "graphics\Shader.h"
#include "MyMaterials.h"
#include "teapot.h"

class SimpleMeshRenderingScene : public Scene
{
public:
	SimpleMeshRenderingScene()
	{
		FRM_SHADER_ATTRIBUTE attributes[2] =
		{
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

		//Load shader
		m_shader = new graphics::Shader("assets/Simple3d.vertexshader",
			"assets/Simple3d.fragmentshader", attributes, numAttributes);

		m_material = new GlobalShaderUniforms(m_shader, &m_sharedValues);
		checkOpenGL();

		//Create mesh
		m_mesh = createTeapotMesh();
	}
	virtual~SimpleMeshRenderingScene()
	{

	}

	graphics::Mesh* createTeapotMesh()
	{
		// Create Index buffer
		graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);

		// Create Vertex arrays
		graphics::VertexArray* va[] =
		{
			new graphics::VertexArrayImpl<slmath::vec3>(
			graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),
			new graphics::VertexArrayImpl<slmath::vec3>(
			graphics::ATTRIB_NORMAL, (slmath::vec3*) TeapotData::normals, TeapotData::numVertices)
		};

		// Create vertex buffer from vertex arrays
		graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));

		// Create mesh from ib and vb
		return new graphics::Mesh(ib, vb);
	}

	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_totalTime += deltaTime;

		m_sharedValues.totalTime += deltaTime;
		if (m_sharedValues.totalTime > 2.0f)
		{
			m_sharedValues.totalTime = 0.0;
		}
		//Camera perspective matrix = Field of view, aspect ratio 
		//near plane distance and far plane distance
		float fAspect = (float)esContext->width / (float)esContext->height;
		m_matProjection = slmath::perspectiveFovRH(
			slmath::radians(45.0f),
			fAspect,
			5.0f,
			1000.0f);

		//Look at view matrix
		m_matView = slmath::lookAtRH(
			slmath::vec3(0.0f, 70.0f, 70.0f),
			slmath::vec3(0.0f, 15.0f, 0.0f),
			slmath::vec3(0.0f, 1.0f, 0.0f));

		//Update teapot model matrix
		m_matModel = slmath::rotationX(-3.1415 * 0.5f); // 90 degrees around X-axis
		m_matModel = slmath::rotationY(m_totalTime) * m_matModel; //Rotate according to total time
		m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel; //Translate
	}

	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();
		//set viewport
		glViewport(0, 0, esContext->width, esContext->height);
		//clear back and depth buffers
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		//initialize GL state.
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		checkOpenGL();

		//values for updating each mesh that is rendered (different model matrix)
		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;

		//Set matrix to shared values
		m_sharedValues.matModelViewProj = matModelViewProj;

		m_material->bind();
		checkOpenGL();

		m_mesh->render();
		checkOpenGL();
	}

private:
	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;
	core::Ref<graphics::Mesh> m_mesh;
	core::Ref<graphics::Shader> m_shader;
	SharedShaderValues m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;
	float m_totalTime;
};