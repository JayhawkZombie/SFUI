#version 140

void main()
{
  vec4 color = gl_Color;
  gl_FragColor = vec4(0, 0, 0, color.w);
}