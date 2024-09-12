#version 330 core
out vec4 FragColor;
in vec3 ourColor;

uniform float greenValue;

void main() {
	FragColor = vec4(ourColor.x, greenValue, ourColor.z, 1.0f);
}