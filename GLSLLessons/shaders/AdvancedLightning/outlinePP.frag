#version 330 core

uniform sampler2D mainTexture;
uniform sampler2D selectionObjTexture;


vec2 offsets[8]=
{
vec2(1,1),
vec2(-1,-1),
vec2(1,-1),
vec2(-1,1),

vec2(1,0),
vec2(0,1),
vec2(-1,0),
vec2(0,-1)
};
void main()
{
vec4 color=texture(selectionObjTexture,gl_FragCoord.xy/vec2(1920,1280));
vec4 colorMain=texture(mainTexture,gl_FragCoord.xy/vec2(1920,1280));
vec4 outlineM=vec4(0);
for(int i=0; i<offsets.length(); i++)
{


    outlineM=max(outlineM,  texture(selectionObjTexture, (gl_FragCoord.xy+offsets[i]*15)/vec2(1920,1280)));
   
}
gl_FragColor=colorMain+vec4(outlineM.rgb-color.rgb,1);

}