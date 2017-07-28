// From https://www.shadertoy.com/view/Mtl3Rj

#version 140

uniform sampler2D image;
uniform vec2 imagesize;

float SCurve(float x) {

  x = x * 2.0 - 1.0;
  return ( -x * abs(x) * 0.5 + x + 0.5 );
}

vec4 BlurV (sampler2D source, vec2 size, vec2 uv, float radius) {

	if (radius >= 1.0)
	{
		vec4 A = vec4(0.0); 
		vec4 C = vec4(0.0); 

		float height = 1.0 / size.y;

		float divisor = 0.0; 
        float weight = 0.0;
        
        float radiusMultiplier = 1.0 / radius;

        for (float y = -30.0; y <= 30.0; y++)
		{
			A = texture(source, uv + vec2(0.0, y * height));
            	
            	weight = SCurve(1.0 - (abs(y) * radiusMultiplier)); 
            
            	C += A * weight; 
            
			divisor += weight; 
		}

		return vec4(C.r / divisor, C.g / divisor, C.b / divisor, C.a / divisor);
	}

	return texture(source, uv);
}

void main()
{
  vec2 uv = gl_FragCoord.xy / imagesize.xy;
  gl_FragColor = BlurV(image, imagesize.xy, uv, 30.0);
}