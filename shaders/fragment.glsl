#version 130

in vec3 normals;

void main() {
	gl_FragColor.rgb = normals + 0.6;
	gl_FragColor.a = 1.0;
}
