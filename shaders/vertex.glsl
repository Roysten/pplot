#version 130

attribute highp vec4 posAttr;
attribute lowp vec3 normAttr;
uniform highp mat4 matrix;

out vec3 normals;

void main() {
	gl_Position = matrix * posAttr;
	normals = normAttr;
}
