attribute vec3 a_posL;
attribute vec3 a_color;
varying vec3 v_color; /// interpolare a_color
uniform mat4 m_rotatie;
uniform mat4 m_camera;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = m_camera * posL;
	v_color = a_color;
}
   