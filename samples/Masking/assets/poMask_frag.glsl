//
//  poMask_frag.glsl
//  poMask Fragment Shader
//
//  Created by Stephen Varga on 5/13/14.
//
//

uniform sampler2D tex;
uniform sampler2D mask;

uniform vec2 contentScale;
uniform vec2 maskPosition;

void main(void)
{
    vec2 c0 = gl_TexCoord[0].st;
    vec2 c1 = gl_TexCoord[0].st * contentScale;
    
    vec4 rgbValue = texture2D( tex, c0);
    vec4 alphaValue = texture2D( mask, c1 );
    gl_FragColor.rgb = rgbValue.rgb;
    gl_FragColor.a = alphaValue.r;
}

