#version 130


varying vec4 position_3d_original;
varying vec4 position_3d_modelview;
varying vec3 normal;
flat in vec4 color;

uniform sampler2D texture [4];

uniform vec3 light=vec3(0.5,0.3,5.0);


void main (void)
{
    vec3 n=normalize(normal);

    vec3 p=position_3d_modelview.xyz;
    vec3 vertex_to_light=normalize(light-p);
    vec3 reflected_light=reflect(-vertex_to_light,n);
    vec3 user_to_vertex=normalize(-p);

    float diffuse_term=0.8*clamp(abs(dot(n,vertex_to_light)),0.0,1.0);
    float specular_term=0.2*pow(clamp(dot(reflected_light,user_to_vertex),0.0,1.0),128.0);
    float ambiant_term=0.4;

    vec4 white=vec4(1.0,1.0,1.0,0.0);
    vec2 tex_coord=gl_TexCoord[0].xy;

    vec4 color_final=vec4(1,0,0,0);

    vec4 ColorTexture0;
    vec4 ColorTexture1;
    vec4 ColorTexture2;
    vec4 ColorTexture3;


    ColorTexture0 = texture2D(texture[0],tex_coord);
    ColorTexture1 = texture2D(texture[1],tex_coord);
    ColorTexture2 = texture2D(texture[2],tex_coord);
    ColorTexture3 = texture2D(texture[3],tex_coord);


    if(color.r == 0)
    {
      color_final = ColorTexture1;
    }
    else if (color.r == 0.25)
    {
        color_final = ColorTexture3;
    }
    else if (color.r == 0.50)
    {
        color_final = ColorTexture0;
    }
    else if (color.r == 0.75)
    {
        color_final = ColorTexture2;
    }





    gl_FragColor =(ambiant_term+diffuse_term)*color_final+specular_term*white;

}
