#ifndef GUI_HH
#define GUI_HH

#include <GL/glew.h>
#include "../imgui/imgui.h"
#include <memory>

class Gui
{
	ImFont *font;

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

	void CreateFontTexture(ImFont *imFont);
	void Update(int dt);
	void Draw();
};

#endif

