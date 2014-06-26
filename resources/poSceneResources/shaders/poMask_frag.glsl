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
    vec2 c0 = vec2(gl_TexCoord[0].s, 1.0 - gl_TexCoord[0].t);
    vec2 c1 = (gl_TexCoord[0].st - maskPosition) * contentScale;
    
    vec4 rgbValue       = texture2D(tex, c0);
    vec4 alphaValue     = texture2D(mask, c1);
    gl_FragColor.rgb    = rgbValue.rgb;
    if(c1.x > 0.0 && c1.x < 1.0 && c1.y > 0.0 && c1.y < 1.0) {
        gl_FragColor.a      = alphaValue.r * rgbValue.a;
    } else {
        gl_FragColor.a = 0.0;
    }
    //gl_FragColor.a = 1.0;
}

