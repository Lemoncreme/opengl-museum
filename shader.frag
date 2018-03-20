
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"varying vec4 vWorld;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, abs(vWorld.y / 1.5f), 0.2f, 1.0f);\n"
	"}\n\0";
