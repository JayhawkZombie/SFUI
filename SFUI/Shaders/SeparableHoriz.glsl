#version 140

uniform sampler2D image;
uniform vec2 imagesize;

void main()
{

  vec2 p = ( -imagesize.xy + 2.0 * gl_FragCoord.xy ) / imagesize.y;
  vec4 blr = vec4(0.0);

    blr += 0.026109*texture2D( image, (gl_FragCoord.xy + vec2(-15.5,0.0))/imagesize.xy );
    blr += 0.034202*texture2D( image, (gl_FragCoord.xy + vec2(-13.5,0.0))/imagesize.xy );
    blr += 0.043219*texture2D( image, (gl_FragCoord.xy + vec2(-11.5,0.0))/imagesize.xy );
    blr += 0.052683*texture2D( image, (gl_FragCoord.xy + vec2( -9.5,0.0))/imagesize.xy );
    blr += 0.061948*texture2D( image, (gl_FragCoord.xy + vec2( -7.5,0.0))/imagesize.xy );
    blr += 0.070266*texture2D( image, (gl_FragCoord.xy + vec2( -5.5,0.0))/imagesize.xy );
    blr += 0.076883*texture2D( image, (gl_FragCoord.xy + vec2( -3.5,0.0))/imagesize.xy );
    blr += 0.081149*texture2D( image, (gl_FragCoord.xy + vec2( -1.5,0.0))/imagesize.xy );
    blr += 0.041312*texture2D( image, (gl_FragCoord.xy + vec2(  0.0,0.0))/imagesize.xy );
    blr += 0.081149*texture2D( image, (gl_FragCoord.xy + vec2(  1.5,0.0))/imagesize.xy );
    blr += 0.076883*texture2D( image, (gl_FragCoord.xy + vec2(  3.5,0.0))/imagesize.xy );
    blr += 0.070266*texture2D( image, (gl_FragCoord.xy + vec2(  5.5,0.0))/imagesize.xy );
    blr += 0.061948*texture2D( image, (gl_FragCoord.xy + vec2(  7.5,0.0))/imagesize.xy );
    blr += 0.052683*texture2D( image, (gl_FragCoord.xy + vec2(  9.5,0.0))/imagesize.xy );
    blr += 0.043219*texture2D( image, (gl_FragCoord.xy + vec2( 11.5,0.0))/imagesize.xy );
    blr += 0.034202*texture2D( image, (gl_FragCoord.xy + vec2( 13.5,0.0))/imagesize.xy );
    blr += 0.026109*texture2D( image, (gl_FragCoord.xy + vec2( 15.5,0.0))/imagesize.xy );

  blr /= 0.93423;
  gl_FragColor = blr;
}