#version 410 core
layout (location = 0)out vec3 FragColor;
layout (location = 1)out vec4 buff1;

uniform vec4 ourColor;
in vec2 Tcoord;

uniform float par;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int type;
uniform bool selected;

in vec3 norm;
in vec3 FragPos;
in vec3 debugColor;
void main()
{   
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffuse = max(dot(lightDir, norm), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float res = spec + diffuse;
    vec3 color = (type==1)?1-debugColor:debugColor;
    FragColor = color*(res+0.1);
    if(selected)buff1 = vec4(1.0, 0.0, 0.0, 1.0);
    else buff1 = vec4(0.0, 0.0, 0.0, 0.0);
  
} 