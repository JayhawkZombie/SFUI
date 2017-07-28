#version 140

#define PI2 6.283184

//From shadertoy https://www.shadertoy.com/view/Xds3W8

#define CV 0.2
#define ST 0.02

uniform sampler2D image;
uniform vec2 imagesize;

vec4 colorat(vec2 uv) {
  return texture2D(image, uv);
}

vec4 blur(vec2 uv) {
  vec4 col = vec4(0.0);

  for (float r0 = 0.0; r0 < 1.0; r0 += ST) {
  
    float r = r0 * CV;

    for (float a0 = 0.0; a0 < 1.0; a0 += ST) {
    
      float a = a0 * PI2;
      col += colorat(uv + vec2(cos(a), sin(a)) * r);
    }
  }

  col *= ST;
  return col;
}

void main()
{
  vec2 uv = gl_FragCoord.xy / imagesize.xy;
  gl_FragColor = blur(uv);
}