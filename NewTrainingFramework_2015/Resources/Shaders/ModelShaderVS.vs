varying vec2 v_uv;
attribute vec3 a_posL;
uniform mat4 m_camera;
attribute vec2 a_uv;

void main()
{
	v_uv = a_uv;
	vec4 posL = vec4(a_posL,1.0);
	gl_Position = m_camera * posL;
}