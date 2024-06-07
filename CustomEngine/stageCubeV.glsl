#version 430 

layout (location = 0) in vec3 aPos;
layout (location =1) in vec2 coord;
layout (location =2) in vec3 normal;


uniform float offset;
uniform mat4 mvp;
uniform vec3 color;
//uniform mat4 model;

out vec2 Tcoord;
out vec3 norm;
out vec3 FragPos;
out vec3 debugColor;
void main()
{   
    //vec3 i = vec3(float(gl_InstanceID), 0, 0);
    gl_Position = mvp  * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    Tcoord = coord;
    norm = normal;
    FragPos = (vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;
    debugColor = color;
}

