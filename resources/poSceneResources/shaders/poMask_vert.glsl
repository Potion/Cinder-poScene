//
//  poMask_frag.glsl
//  poMask Fragment Shader
//
//  Created by Stephen Varga on 5/13/14.
//
//
uniform vec2 maskPosition;

void main()
{
	gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;// * vec4(1,-1,1,1);
	gl_TexCoord[0]  = gl_MultiTexCoord0;
    gl_TexCoord[1]  = gl_MultiTexCoord1;
}

