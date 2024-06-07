 #version 440 core
in vec2 TexCoords;
    out vec4 FragColor;
layout (binding=0) uniform sampler2D  albedo;
layout (binding=1) uniform sampler2D buff;
layout (binding=2) uniform usampler2D stencil;


    void main() {
        vec3 color = texture(albedo, TexCoords).rgb;
        FragColor = vec4(color, 1.0);

    
        uvec3 st = texture(stencil, TexCoords).rgb;

        vec3 color1 = texture(buff, TexCoords).rgb;
        vec4 FragColor1 = vec4(color1, 1.0);

        bool res = false;
        vec3 left = texture(buff, TexCoords +vec2(1.0, 0.0) / textureSize(buff, 0)).rgb;
        vec3 right = texture(buff, TexCoords+vec2(-1.0, 1.0) / textureSize(buff, 0)).rgb;
        vec3 up = texture(buff, TexCoords +vec2(0.0, 1.0) / textureSize(buff, 0)).rgb;
        vec3 down = texture(buff, TexCoords+vec2(0.0, -1.0) / textureSize(buff, 0)).rgb;
        if(left.r + right.r > 0.99 && left.r + right.r < 1.01)res = true;
        if(up.r + down.r > 0.99 && up.r + down.r < 1.01)res = true;
        

        FragColor = (st.r>0)?vec4(1.0, 0.0, 0.0, 1.0):FragColor;

        // Output the integer stencil value as grayscale
       // FragColor = vec4(vec3(float(packedValue)/255), 1.0);
    }