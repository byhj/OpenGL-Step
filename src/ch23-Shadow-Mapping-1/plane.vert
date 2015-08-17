#version 430 core

 layout (location = 0) in vec4 Position;
 layout (location = 1) in vec2 TexCoord;

 out VS_OUT
 {
   vec2 tc;
 }vs_out;

 void main(void)
 {
   vs_out.tc = TexCoord;
   gl_Position = Position;
 }