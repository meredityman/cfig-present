#version 150

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform float  u_fade;
uniform float  u_segFade;
uniform vec2   u_resolution;

in vec2 texCoordVarying;

out vec4 outputColor;

float rand(float n){return fract(sin(n) * 43758.5453123);}

float noise(float p){
	float fl = floor(p);
    float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}
	

void main() {

    vec2 uv0 = 2.0 * (texCoordVarying - 0.5);
    vec2 uv = uv0;

    vec2 resolution = u_resolution * (pow(u_fade, 2));

    vec2 uv_res = vec2(
        floor(((resolution.x + 2.0 * (1.0 - u_fade) * noise(20 * (uv0.x + 0.23123) * resolution.x * u_fade)) * uv.x )),
        ceil(((resolution.y + 2.0 * (1.0 - u_fade) * noise(20 * (uv0.y + 0.13123) * resolution.y * u_fade)) * uv.y ))
    );
    vec2 uv_m = uv_res / resolution;

    uv_m = 0.5 * (uv_m + vec2(1.0, 1.0));


    vec4 color1 = vec4(
        texture(tex1, uv_m + 0.1 * smoothstep(0.0, 1.0, 1.0 - u_fade) * vec2(0.0, 0.1 * (1.0 - u_fade))).r,
        texture(tex1, uv_m + 0.1 * smoothstep(0.0, 1.0, 1.0 - u_fade) * vec2(0.0, 0.2 * (1.0 - u_fade))).g,
        texture(tex1, uv_m + 0.1 * smoothstep(0.0, 1.0, 1.0 - u_fade) * vec2(0.0, 0.3 * (1.0 - u_fade))).b,
        1.0
        
    );
  vec4 color2 = vec4(
        texture(tex2, uv_m + 0.1 * smoothstep(0.0, 1.0, 1.0 - u_fade) * vec2(0.0, 0.1 * (1.0 - u_fade))).r,
        texture(tex2, uv_m + 0.1 * smoothstep(0.0, 1.0, 1.0 - u_fade) * vec2(0.0, 0.2 * (1.0 - u_fade))).g,
        texture(tex2, uv_m + 0.1 * smoothstep(0.0, 1.0, 1.0 - u_fade) * vec2(0.0, 0.3 * (1.0 - u_fade))).b,
        1.0
        
    );
    float mask  = texture(tex3, uv_m).r;

    outputColor = mix(color1, color2, (pow(u_fade, 0.5)));

    outputColor.rgb *= mix(1.0 - u_segFade, 1.0, mask);
    //outputColor = vec4(texCoordVarying.x, texCoordVarying.y, mask, u_fade);
}
