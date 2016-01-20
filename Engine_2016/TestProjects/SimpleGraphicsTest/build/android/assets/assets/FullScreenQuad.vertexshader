//Vertex shader

attribute vec4 g_vVertex;
attribute vec4 g_vColor;
attribute vec2 g_vTexCoord;
varying   vec4 g_vVSColor;
varying   vec2 g_vVTexCoord;

void main()
{
    gl_Position  = vec4( g_vVertex.x, g_vVertex.y, g_vVertex.z, g_vVertex.w );
    g_vVSColor = g_vColor;
    g_vVTexCoord = g_vTexCoord;
};
