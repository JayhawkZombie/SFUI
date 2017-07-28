// From https://www.shadertoy.com/view/Mtl3Rj

#version 140

uniform sampler2D image;
uniform vec2 imagesize;

float Scurve(float x) {

  x = x * 2.0 - 1.0;
  return ( -x * abs(x) * 0.5 + x + 0.5 );
}

vec4 blurH(sampler2D source, vec2 size, vec2 uv, float radius) {

  if (radius >= 1.0) {
  
    vec4 a = vec4(0.0);
    vec4 c = vec4(0.0);

    float width = 1.0 / size.x;
    float divisor = 0.0;
    float weight = 0.0;
    float radiusMult = 1.0 / radius;

    for (float x = -30.0; x <= 30.0; x++) {
    
      a = texture2D(source, uv + vec2(x * width, 0.0));
      weight = Scurve(1.0 - (abs(x) * radiusMult));
      c += a * weight;

      divisor += weight;
    }
    
    return vec4(c.r / divisor, c.g / divisor, c.b / divisor, c.a / divisor);
  }

  return texture2D(source, uv);
}

void main()
{
  vec2 uv = gl_FragCoord.xy / imagesize.xy;
  gl_FragColor = blurH(image, imagesize.xy, uv, 30.0);
}