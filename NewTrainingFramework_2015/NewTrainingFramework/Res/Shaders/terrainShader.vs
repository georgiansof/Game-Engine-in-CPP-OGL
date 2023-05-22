varying vec2 v_uv;
varying vec2 v_uv_bmap;
attribute vec3 a_posL;
uniform mat4 m_camera;
attribute vec2 a_uv;
attribute vec2 a_uv2;
varying vec3 position;
uniform vec3 u_height;
uniform sampler2D u_texture_0;

void main()
{
	vec4 c_blend = texture2D(u_texture_0,a_uv2);
	v_uv_bmap = a_uv2;
	v_uv = a_uv;
	vec4 posL = vec4(a_posL ,1.0);

	posL.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;

	gl_Position = m_camera * posL;
	position = posL.xyz;
}