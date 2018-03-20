#version 330 core
out vec4 FragColor;
varying vec4 vWorld;
void main()
{
   FragColor = vec4(1.0f, abs(vWorld.y / 1.5f), 0.2f, 1.0f);
}
