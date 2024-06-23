#version 410 core
layout (location = 0)out vec4 FragColor;
layout (location = 1)out vec4 buff1;

uniform vec4 ourColor;
in vec2 Tcoord;

uniform float par;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int type;
uniform int texIndex;
uniform int CtexIndex;
uniform bool selected;
uniform int Selector;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;
uniform sampler2D texture8;
uniform sampler2D texture9;
uniform sampler2D texture10;
uniform sampler2D texture11;
uniform sampler2D texture12;
uniform sampler2D texture13;
uniform sampler2D texture14;
uniform sampler2D texture15;
uniform sampler2D texture16;
uniform sampler2D texture17;
uniform sampler2D texture18;
uniform sampler2D texture19;
uniform sampler2D texture20;
uniform sampler2D texture21;
uniform sampler2D texture22;
uniform sampler2D texture23;
uniform sampler2D texture24;
uniform sampler2D texture25;
uniform sampler2D texture26;
uniform sampler2D texture27;
uniform sampler2D texture28;
uniform sampler2D texture29;
uniform sampler2D texture30;
uniform sampler2D texture31;

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
    vec3 color;


    if(Selector == -1){
      if(dot(vec3(1, 0, 0), norm) > 0.999 || dot(vec3(1, 0, 0), norm) < -0.999){
            color  = texture(texture3, Tcoord).rgb;
        }else{
         switch(texIndex){
            case 0:{
                color = texture(texture0, Tcoord).rgb;
                break;
            }
            case 1:{
                color = texture(texture1, Tcoord).rgb;
                break;
            }    
            case 2:{
                color = texture(texture2, Tcoord).rgb;
                break;
            }   
            case 3:{
                color = texture(texture3, Tcoord).rgb;
                break;
            }   
            default:{
                color = vec3(0);
             }
        };
    
        }
    
    }
    else if(Selector == 0){
        if(dot(vec3(1, 0, 0), norm) > 0.999){
            switch(CtexIndex){
                case 0:{
                    color = texture(texture1, Tcoord).rgb;
                    break;
                }
                case 1:{
                    color = texture(texture2, Tcoord).rgb;
                    break;
                }    
                case 2:{
                    color = texture(texture3, Tcoord).rgb;
                    break;
                }   
                case 3:{
                    color = texture(texture4, Tcoord).rgb;
                    break;
                }   
                case 4:{
                    color = texture(texture5, Tcoord).rgb;
                    break;
                }
                case 5:{
                    color = texture(texture6, Tcoord).rgb;
                    break;
                }
                case 6:{
                    color = texture(texture7, Tcoord).rgb;
                    break;
                }
                case 7:{
                    color = texture(texture8, Tcoord).rgb;
                    break;
                }
                case 8:{
                    color = texture(texture9, Tcoord).rgb;
                    break;
                }
                default:{
                    color = vec3(0);
                 }
            };
        
        }
        else if(dot(vec3(1, 0, 0), norm) < -0.999){
            switch(CtexIndex){
                case 0:{
                    color = texture(texture10, Tcoord).rgb;
                    break;
                }
                case 1:{
                    color = texture(texture11, Tcoord).rgb;
                    break;
                }    
                case 2:{
                    color = texture(texture12, Tcoord).rgb;
                    break;
                }   
                case 3:{
                    color = texture(texture13, Tcoord).rgb;
                    break;
                }   
                case 4:{
                    color = texture(texture14, Tcoord).rgb;
                    break;
                }
                case 5:{
                    color = texture(texture15, Tcoord).rgb;
                    break;
                }
                case 6:{
                    color = texture(texture16, Tcoord).rgb;
                    break;
                }
                case 7:{
                    color = texture(texture17, Tcoord).rgb;
                    break;
                }
                case 8:{
                    color = texture(texture18, Tcoord).rgb;
                    break;
                }
                default:{
                    color = vec3(0);
                 }
            };             
        }
        else if(dot(vec3(0, 0, 1), norm) < -0.999){
            switch(texIndex){
                case 0:{
                    color = texture(texture19, Tcoord).rgb;
                    break;
                }
                case 1:{
                    color = texture(texture20, Tcoord).rgb;
                    break;
                }    
                case 2:{
                    color = texture(texture21, Tcoord).rgb;
                    break;
                }   
                case 3:{
                    color = texture(texture22, Tcoord).rgb;
                    break;
                }   
                case 4:{
                    color = texture(texture23, Tcoord).rgb;
                    break;
                }
                case 5:{
                    color = texture(texture24, Tcoord).rgb;
                    break;
                }
                case 6:{
                    color = texture(texture25, Tcoord).rgb;
                    break;
                }
                case 7:{
                    color = texture(texture26, Tcoord).rgb;
                    break;
                }
                case 8:{
                    color = texture(texture27, Tcoord).rgb;
                    break;
                }
                 case 9:{
                    color = texture(texture28, Tcoord).rgb;
                    break;
                }
                default:{
                    color = vec3(0);
                 }
            };             
        }
        else if(dot(vec3(0, 1, 0), norm) > 0.9999){
            color = texture(texture0, Tcoord).rgb;
            //color = vec3(Tcoord.x, Tcoord.y, 0);
        }
        else{
            color = vec3(0, 0, 0);
        }

    }
    else if(Selector == 1){
        if(dot(vec3(1, 0, 0), norm) > 0.999){
            switch(CtexIndex){
                case 0:{
                    color = texture(texture1, Tcoord).rgb;
                    break;
                }
                case 1:{
                    color = texture(texture2, Tcoord).rgb;
                    break;
                }    
                case 2:{
                    color = texture(texture3, Tcoord).rgb;
                    break;
                }   
                case 3:{
                    color = texture(texture4, Tcoord).rgb;
                    break;
                }   
                case 4:{
                    color = texture(texture5, Tcoord).rgb;
                    break;
                }
                case 5:{
                    color = texture(texture6, Tcoord).rgb;
                    break;
                }
                case 6:{
                    color = texture(texture7, Tcoord).rgb;
                    break;
                }
                case 7:{
                    color = texture(texture8, Tcoord).rgb;
                    break;
                }
                case 8:{
                    color = texture(texture9, Tcoord).rgb;
                    break;
                }
                case 9:{
                    color = texture(texture10, Tcoord).rgb;
                    break;
                }
                case 10:{
                    color = texture(texture11, Tcoord).rgb;
                    break;
                }
                case 11:{
                    color = texture(texture12, Tcoord).rgb;
                    break;
                }
                case 12:{
                    color = texture(texture13, Tcoord).rgb;
                    break;
                }
                case 13:{
                    color = texture(texture14, Tcoord).rgb;
                    break;
                }
                case 14:{
                    color = texture(texture2, Tcoord).rgb;
                    break;
                }

                default:{
                    color = vec3(0);
                 }
            };
        
        }
        else if(dot(vec3(1, 0, 0), norm) < -0.999){
            switch(CtexIndex){
                case 0:{
                    color = texture(texture3, Tcoord).rgb;
                    break;
                }
                case 1:{
                    color = texture(texture17, Tcoord).rgb;
                    break;
                }    
                case 2:{
                    color = texture(texture18, Tcoord).rgb;
                    break;
                }   
                case 3:{
                    color = texture(texture19, Tcoord).rgb;
                    break;
                }   
                case 4:{
                    color = texture(texture20, Tcoord).rgb;
                    break;
                }
                case 5:{
                    color = texture(texture21, Tcoord).rgb;
                    break;
                }
                case 6:{
                    color = texture(texture22, Tcoord).rgb;
                    break;
                }
                case 7:{
                    color = texture(texture23, Tcoord).rgb;
                    break;
                }
                case 8:{
                    color = texture(texture24, Tcoord).rgb;
                    break;
                }
                case 9:{
                    color = texture(texture25, Tcoord).rgb;
                    break;
                }
                case 10:{
                    color = texture(texture26, Tcoord).rgb;
                    break;
                }
                case 11:{
                    color = texture(texture27, Tcoord).rgb;
                    break;
                }
                case 12:{
                    color = texture(texture28, Tcoord).rgb;
                    break;
                }
                case 13:{
                    color = texture(texture29, Tcoord).rgb;
                    break;
                }
                case 14:{
                    color = texture(texture4, Tcoord).rgb;
                    break;
                }
                default:{
                    color = vec3(0);
                 }
            };             
        }
        else if(dot(vec3(0, 0, 1), norm) < -0.999){
            switch(texIndex){
                case 0:{
                    color = texture(texture5, Tcoord).rgb;
                    break;
                }
                case 1:{
                    color = texture(texture6, Tcoord).rgb;
                    break;
                }    
                case 2:{
                    color = texture(texture7, Tcoord).rgb;
                    break;
                }   
                case 3:{
                    color = texture(texture8, Tcoord).rgb;
                    break;
                }   
              
                default:{
                    color = vec3(0);
                 }
            };             
        }
        else if(dot(vec3(0, 1, 0), norm) > 0.9999){
            color = texture(texture0, Tcoord).rgb;
            //color = vec3(Tcoord.x, Tcoord.y, 0);
        }
        else{
            color = vec3(0, 0, 0);
        }
    }
    else if(Selector == -2){
        color = texture(texture0, Tcoord).rgb;
    }
    else{
        color = vec3(0, 0, 0);
    }


   
    //color = texture(texture0, Tcoord).rgb;
    FragColor = vec4(color*(res+0.5), 1);

    if(selected)buff1 = vec4(1.0, 0.0, 0.0, 1.0);
    else buff1 = vec4(0.0, 0.0, 0.0, 0.0);
  
} 