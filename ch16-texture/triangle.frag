#version 430 core

out vec4 fragColor;

in VS_OUT {
  vec2 tc;
  vec4 color;
}fs_in;

uniform sampler2D tex; // texture data

void main(void)
 {
      fragColor = texture2D(tex, fs_in.tc);
 }