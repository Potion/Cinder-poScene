//
//  poMask_frag.glsl
//  poMask Fragment Shader
//
//  Created by Stephen Varga on 5/13/14.
//
//

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

