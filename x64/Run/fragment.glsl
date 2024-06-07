#version 410 core
out vec4 FragColor;
uniform vec4 ourColor;
in vec2 Tcoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float par;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int type;

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
    FragColor = mix(texture(texture1, Tcoord), texture(texture2, Tcoord), par);
    FragColor = vec4(FragColor.xyz*res,  1);
    vec3 color = (type==1)?1-debugColor:debugColor;
    FragColor = vec4(color*(res+0.1),  1);
  
} 