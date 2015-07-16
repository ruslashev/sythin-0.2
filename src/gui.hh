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

	int mousePosX;
	int mousePosY;
	bool mousePressed[2];

	Gui();
	~Gui();

	void Update(int dt);
	void Draw();
};

#endif

