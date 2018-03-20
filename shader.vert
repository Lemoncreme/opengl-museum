#version 150
in vec4 vPosition;
in vec4 vColor;
out vec4 color;
uniform vec3 theta;
uniform vec2 scale;
varying vec4 vWorld;
void main()
{
	//Compute the sines and cosines of theta for each of
	//the three axes in one computation.
	vec3 angles = radians(theta);
	vec3 c = cos(angles);
	vec3 s = sin(angles);
	//Remember: these matrices are column major.
	mat4 rx = mat4(1.0, 0.0, 0.0, 0.0,
		  0.0, c.x, s.x, 0.0,
		  0.0, -s.x, c.x, 0.0,
		  0.0, 0.0, 0.0, 1.0);
	mat4 ry = mat4(c.y, 0.0, -s.y, 0.0,
		  0.0, 1.0, 0.0, 0.0,
		  s.y, 0.0, c.y, 0.0,
		  0.0, 0.0, 0.0, 1.0);
	mat4 rz = mat4(c.z, -s.z, 0.0, 0.0,
		  s.z, c.z, 0.0, 0.0,
		  0.0, 0.0, 1.0, 0.0,
		  0.0, 0.0, 0.0, 1.0);
    mat4 scalemat = mat4(mat2(scale[0], 0, 0, scale[1]));
	color = vColor;
	gl_Position = scalemat * rz * ry * rx * vPosition;
	vWorld = gl_Position;
}
