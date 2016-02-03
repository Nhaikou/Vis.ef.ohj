#include "scene.h"
#include <graphics/Mesh.h>
#include <graphics/Shader.h>
#include "MyMaterials.h"
#include "teapot.h"

class BlinnPhongScene : public Scene
{
public:
	BlinnPhongScene()
	{
		FRM_SHADER_ATTRIBUTE attributes[2] =
		{
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

		//Load shader
		m_shader = new graphics::Shader("assets/Blinn-phong.vertexShader",
			"assets/Blinn-phong.fragmentShader", attributes, numAttributes);

		SimpleMaterialUniforms* simpleMaterialUniforms = new SimpleMaterialUniforms(m_shader, &m_sharedValues);
		
		// Material values for mesh
		simpleMaterialUniforms->vAmbient = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vDiffuse = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		simpleMaterialUniforms->vSpecular = slmath::vec4(1.0f, 1.0f, 1.0f, 5.0f);

		m_material = simpleMaterialUniforms;

		checkOpenGL();

		//Create mesh
		m_mesh = createTeapotMesh();
	}

	virtual ~BlinnPhongScene()
	{
	}

	graphics::Mesh* createTeapotMesh()
	{
		//Creating index buffer
		graphics::IndexBuffer* indexBuf = new graphics::IndexBuffer
			(TeapotData::indices, TeapotData::numIndices);

		//Creating vertex arrays
		graphics::VertexArray* vertexArr[] =
		{
			new graphics::VertexArrayImpl<slmath::vec3>
			(graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions,
			TeapotData::numVertices),

			new graphics::VertexArrayImpl<slmath::vec3>
			(graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals,
			TeapotData::numVertices)
		};

		//Creating vertex buffer from arrays
		graphics::VertexBuffer* vertexBuf =
			new graphics::VertexBuffer(&vertexArr[0],
			sizeof(vertexArr) / sizeof(vertexArr[0]));

		//Create mesh from indices and vertices
		return new graphics::Mesh(indexBuf, vertexBuf);
	}

	/////////////////////////////////////////////////////////////////
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

		// Calculate needed stuff for m_sharedValues
		m_sharedValues.matModel = m_matModel;
		m_sharedValues.matView = m_matView;
		m_sharedValues.matProj = m_matProjection;

		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;
		slmath::mat4 matNormal = slmath::transpose(slmath::inverse(matModelView));

		m_sharedValues.matModelView = matModelView;
		m_sharedValues.matNormal = matNormal;
		m_sharedValues.matModelViewProj = matModelViewProj;

		m_sharedValues.lightPos = slmath::vec3(0.0, 70.0f, 70.0f);
		m_sharedValues.camPos = slmath::vec3(0.0, 70.0f, 70.0f);
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
		
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		checkOpenGL();
	}

private:
	core::Ref<graphics::Mesh> m_mesh;
	core::Ref<graphics::Shader> m_shader;
	SharedShaderValues m_sharedValues;
	core::Ref<graphics::ShaderUniforms> m_material;
	float m_totalTime;
	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;
};