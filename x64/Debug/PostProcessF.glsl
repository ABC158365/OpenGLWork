 #version 440 core
in vec2 TexCoords;
    out vec4 FragColor;
layout (binding=0) uniform sampler2D  albedo;
layout (binding=1) uniform sampler2D buff;
layout (binding=2) uniform usampler2D stencil;


    void main() {
        vec3 color = texture(albedo, TexCoords).rgb;
        FragColor = vec4(color, 1.0);

    
        uint st = texture(stencil, TexCoords).r;

        vec3 color1 = texture(buff, TexCoords).rgb;
        vec4 FragColor1 = vec4(color1, 1.0);

        bool res = false;
        uint left = texture(stencil, TexCoords +vec2(1.0, 0.0) / textureSize(stencil, 0)).r;
        uint right = texture(stencil, TexCoords+vec2(-1.0, 1.0) / textureSize(stencil, 0)).r;
        uint up = texture(stencil, TexCoords +vec2(0.0, 1.0) / textureSize(stencil, 0)).r;
        uint down = texture(stencil, TexCoords+vec2(0.0, -1.0) / textureSize(stencil, 0)).r;
        if(left + right == 255)res = true;
        if(up + down == 255)res = true;
        

        FragColor = (res)?vec4(1.0, 0.0, 0.0, 1.0):FragColor;

        // Output the integer stencil value as grayscale
       // FragColor = vec4(vec3(float(packedValue)/255), 1.0);
    }