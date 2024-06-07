#version 440
layout(location = 0)in vec3 pos;
layout(location = 1)in mat4 instancedMat;

uniform mat4 mvp;

void main(){
	gl_Position = mvp * instancedMat * vec4(pos, 1);
}