#version 430 core
out vec4 FragColor;
in vec2 Tcoord;

in vec3 norm;
in vec3 FragPos;

void main()
{   
    
    float x = (abs(FragPos.x)<0.001)?FragPos.y:FragPos.x;
    float y = (abs(FragPos.y)<0.001)?FragPos.z:FragPos.y;
    x = floor(x);
    y = floor(y);
    int x_ = int(x)%2;
    int y_ = int(y)%2;
    vec3 color = (x_==y_)?vec3(0.5, 0.5, 0.5):vec3(.8, 0.8, 0.8);
    FragColor = vec4(color, 1.0);
    
} 
   
