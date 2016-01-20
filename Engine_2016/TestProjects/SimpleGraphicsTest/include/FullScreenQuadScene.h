#include "scene.h"
#include <core/log.h>

class FullScreenQuadScene : public Scene
{
public:
	FullScreenQuadScene()
	{

		LOG("FullScreenQuadScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_flow = 0.0f;
		m_hShaderProgram = 0;

		// Vertex shader
		const char* g_strVSProgram =
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			"attribute vec2 g_vTexCoord;										 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVTexCoord;										 \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"    g_vVTexCoord = g_vTexCoord;                                     \n"
			"}																	 \n";

		// Fragment shader
		const char* g_strFSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVTexCoord;										 \n"
			"uniform float t;												     \n"
			"void main()														 \n"
			"{																	 \n"
			"	//gl_FragColor = g_vVSColor;								     \n"
			"float x = g_vVTexCoord.x * 256;									 \n"
			"float y = g_vVTexCoord.y * 256;									 \n"
			"float vv = 0.3 * -tan((0.5 + (0.5 * sin(x / 7.0 + t))) +(0.5 + (0.5 * sin(y / 5.0 - t))) + (0.5 + (0.5 * sin((x + y) / 6.0 - t))) +(0.5 + (0.5 * sin(sqrt(float(x*x + y*y)) / 4.0 - t))));\n"
			"	gl_FragColor = g_vVSColor * vv;									 \n"
			"}																	 \n";
		
		// Pixel shader
		const char* g_strPSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVTexCoord;										 \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_FragColor = vec4((g_vVTexCoord.x + g_vVTexCoord.y)/2);       \n"
			"}																	 \n";

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 },
			{ "g_vTexCoord", 2 }
		};

		FrmCompileShaderProgram(g_strVSProgram, g_strFSProgram, &m_hShaderProgram, attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		checkOpenGL();
	}


	virtual ~FullScreenQuadScene()
	{
		glDeleteProgram(m_hShaderProgram);
		LOG("FullScreenQuadScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		//m_count += deltaTime;
		m_flow += deltaTime;

		if (m_count > 1.0f)
			m_count = 0.0f;
		
		if (m_flow > 20.0f)
			m_flow = 0.0f;
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
		{   1.0f, 0.0f, 0.0f, 1.0f,
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
		glUseProgram(m_hShaderProgram);
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
		
		glUniform1f(glGetUniformLocation(m_hShaderProgram, "t"), m_flow);

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

	GLuint       m_hShaderProgram;

};