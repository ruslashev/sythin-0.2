#ifndef GUI_HH
#define GUI_HH

#include <GL/glew.h>

class Gui
{
	void createFontsTexture();
	void checkShaderCompileSuccess(int shader);
	void checkProgramLinkSuccess(int program);
public:
	GLuint fontTexture;
	int shaderHandle, vertHandle, fragHandle;
	int attribLocationTex, attribLocationProjMtx;
	int attribLocationPosition, attribLocationUV, attribLocationColor;
	int vboSize;
	unsigned int vboHandle, vaoHandle;

	Gui();
	~Gui();

	void Draw();
};

#endif

