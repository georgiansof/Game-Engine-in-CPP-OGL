attribute vec3 a_posL;
uniform mat4 m_rotatie;
uniform mat4 m_camera;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = posL;
}
   