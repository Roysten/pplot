#version 130

in vec3 normals;

void main() {
	gl_FragColor.rgb = abs(normals);
	gl_FragColor.a = 1.0;
}
