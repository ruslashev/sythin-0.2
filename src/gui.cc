#include "gui.hh"
#include "constants.hh"

static void ImGuiRenderDrawLists(ImDrawData *draw_data)
{
	Gui *gui = (Gui*)ImGui::GetIO().UserData;
	GLint last_program, last_texture;
	glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Setup orthographic projection matrix
	const float width = ImGui::GetIO().DisplaySize.x;
	const float height = ImGui::GetIO().DisplaySize.y;
	const float ortho_projection[4][4] =
	{
		{  2.0f/width, 0.0f,          0.0f,     0.0f },
		{  0.0f,       2.0f/-height,  0.0f,     0.0f },
		{  0.0f,       0.0f,         -1.0f,     0.0f },
		{ -1.0f,       1.0f,          0.0f,     1.0f },
	};
	glUseProgram(gui->shaderHandle);
	glUniform1i(gui->attribLocationTex, 0);
	glUniformMatrix4fv(gui->attribLocationProjMtx, 1, GL_FALSE,
			&ortho_projection[0][0]);
	glBindVertexArray(gui->vaoHandle);

	for (int n = 0; n < draw_data->CmdListsCount; n++) {
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = nullptr;

		glBindBuffer(GL_ARRAY_BUFFER, gui->vboHandle);
		glBufferData(GL_ARRAY_BUFFER,
				(GLsizeiptr)cmd_list->VtxBuffer.size()*sizeof(ImDrawVert),
				(GLvoid*)&cmd_list->VtxBuffer.front(),
				GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui->elementsHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				(GLsizeiptr)cmd_list->IdxBuffer.size()*sizeof(ImDrawIdx),
				(GLvoid*)&cmd_list->IdxBuffer.front(),
				GL_STREAM_DRAW);

			for (const ImDrawCmd *pcmd = cmd_list->CmdBuffer.begin();
					pcmd != cmd_list->CmdBuffer.end();
					pcmd++)
			{
				if (pcmd->UserCallback) {
					pcmd->UserCallback(cmd_list, pcmd);
				} else {
					glBindTexture(GL_TEXTURE_2D,
							(GLuint)(intptr_t)pcmd->TextureId);
					glScissor((int)pcmd->ClipRect.x,
							(int)(height - pcmd->ClipRect.w),
							(int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
							(int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
							GL_UNSIGNED_SHORT, idx_buffer_offset);
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
	}

	// Restore modified state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(last_program);
	glDisable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, last_texture);
}

Gui::Gui()
{
	int err = glewInit();
	if (err != GLEW_OK) {
		printf("Failed to initialize GLEW: %s\n", glewGetErrorString(err));
		throw;
	}

	fontTexture = 0;
	shaderHandle = 0, vertHandle = 0, fragHandle = 0;
	attribLocationTex = 0, attribLocationProjMtx = 0;
	attribLocationPosition = 0, attribLocationUV = 0, attribLocationColor = 0;
	vboHandle = 0, vaoHandle = 0, elementsHandle = 0;

	settingsOpen = true;
	waveOpen = false;

	mousePosX = 0;
	mousePosY = 0;
	mousePressed[0] = false;
	mousePressed[1] = false;

	ImGuiIO& io = ImGui::GetIO();
	io.RenderDrawListsFn = ImGuiRenderDrawLists;
	io.DisplaySize = ImVec2(Globals.windowWidth, Globals.windowHeight);
	io.IniFilename = NULL;
	io.UserData = this;
	io.KeyRepeatDelay = 5.5;
	io.KeyRepeatRate = 1.1;

	io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
	io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
	io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
	io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
	io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
	io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
	io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
	io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
	io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
	io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
	io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
	io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
	io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

	const GLchar *vertex_shader =
		"#version 120\n"
		"uniform mat4 ProjMtx;\n"
		"attribute vec2 Position;\n"
		"attribute vec2 UV;\n"
		"attribute vec4 Color;\n"
		"varying vec2 Frag_UV;\n"
		"varying vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
		"#version 120\n"
		"uniform sampler2D Texture;\n"
		"varying vec2 Frag_UV;\n"
		"varying vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
		"}\n";

	shaderHandle = glCreateProgram();
	vertHandle = glCreateShader(GL_VERTEX_SHADER);
	fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertHandle, 1, &vertex_shader, 0);
	glShaderSource(fragHandle, 1, &fragment_shader, 0);
	glCompileShader(vertHandle);
	glCompileShader(fragHandle);
	glAttachShader(shaderHandle, vertHandle);
	glAttachShader(shaderHandle, fragHandle);
	glLinkProgram(shaderHandle);

	checkShaderCompileSuccess(vertHandle);
	checkShaderCompileSuccess(fragHandle);
	checkProgramLinkSuccess(shaderHandle);

	attribLocationTex = glGetUniformLocation(shaderHandle, "Texture");
	attribLocationProjMtx = glGetUniformLocation(shaderHandle, "ProjMtx");
	attribLocationPosition = glGetAttribLocation(shaderHandle, "Position");
	attribLocationUV = glGetAttribLocation(shaderHandle, "UV");
	attribLocationColor = glGetAttribLocation(shaderHandle, "Color");

	glGenBuffers(1, &vboHandle);
	glGenBuffers(1, &elementsHandle);

	glGenVertexArrays(1, &vaoHandle);
}

void Gui::checkShaderCompileSuccess(int shader)
{
	GLint status, type;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(shader, GL_SHADER_TYPE, &type);
	if (status == GL_FALSE) {
		char buffer[1024];
		glGetShaderInfoLog(shader, 1024, NULL, buffer);
		printf("Failed to compile %s shader:\n%s",
				type == GL_VERTEX_SHADER ? "vertex" : "fragment",
				buffer);
		throw;
	}
}

void Gui::checkProgramLinkSuccess(int program)
{
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		char buffer[1024];
		glGetProgramInfoLog(vertHandle, 1024, NULL, buffer);
		printf("Failed to link shaders:\n%s", buffer);
		throw;
	}
}

void Gui::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Sythin2")) {
			if (ImGui::MenuItem("Quit", ""))
				Globals.quit = true;
			ImGui::EndMenu();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
				ImVec2(Constants.gui.menuBar.modeSpacing, 3));

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,
				Constants.gui.menuBar.modeLiveActive);
		if (Globals.mode == GlobalsHolder::Mode_Live) {
			ImGui::PushStyleColor(ImGuiCol_Button,
					Constants.gui.menuBar.modeLiveActive);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					Constants.gui.menuBar.modeLiveActive);
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button,
					Constants.gui.menuBar.modeLiveIdle);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					Constants.gui.menuBar.modeLiveHovered);
		}
		if (ImGui::Button("LIVE"))
			Globals.mode = GlobalsHolder::Mode_Live;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,
				Constants.gui.menuBar.modeWriteActive);
		if (Globals.mode == GlobalsHolder::Mode_Write) {
			ImGui::PushStyleColor(ImGuiCol_Button,
					Constants.gui.menuBar.modeWriteActive);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					Constants.gui.menuBar.modeWriteActive);
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button,
					Constants.gui.menuBar.modeWriteIdle);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					Constants.gui.menuBar.modeWriteHovered);
		}
		if (ImGui::Button("WRITE"))
			Globals.mode = GlobalsHolder::Mode_Write;

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,
				Constants.gui.menuBar.modePlaybackActive);
		if (Globals.mode == GlobalsHolder::Mode_Playback) {
			ImGui::PushStyleColor(ImGuiCol_Button,
					Constants.gui.menuBar.modePlaybackActive);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					Constants.gui.menuBar.modePlaybackActive);
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button,
					Constants.gui.menuBar.modePlaybackIdle);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
					Constants.gui.menuBar.modePlaybackHovered);
		}
		if (ImGui::Button("PLAYBACK"))
			Globals.mode = GlobalsHolder::Mode_Playback;

		ImGui::PopStyleColor(3*3);

		ImGui::PopStyleVar(2);

		ImGui::EndMainMenuBar();
	}
}

void Gui::TabBar()
{
	bool opened = true;

	ImVec2 windowSize(Constants.gui.width,
			40); // hardcode for now
	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		// ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("TabBar", &opened, windowSize, Constants.gui.alpha,
			windowFlags);

	ImVec2 windowPos(
			Globals.windowWidth - Constants.gui.width - Constants.padding,
			Constants.padding + Constants.gui.menuBarGuiOffset);
	ImGui::SetWindowPos("TabBar", windowPos, ImGuiSetCond_Always);

	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants.gui.tabs.active);
	if (Globals.tab == GlobalsHolder::Tab_Wave) {
		ImGui::PushStyleColor(ImGuiCol_Button, Constants.gui.tabs.active);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants.gui.tabs.active);
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, Constants.gui.tabs.idle);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants.gui.tabs.hovered);
	}
	if (ImGui::Button("Wave")) {
		Globals.tab = GlobalsHolder::Tab_Wave;
		settingsOpen = false;
		waveOpen = true;
	}
	ImGui::SameLine();
	if (Globals.tab == GlobalsHolder::Tab_Settings) {
		ImGui::PushStyleColor(ImGuiCol_Button, Constants.gui.tabs.active);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants.gui.tabs.active);
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, Constants.gui.tabs.idle);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants.gui.tabs.hovered);
	}
	if (ImGui::Button("Settings")) {
		Globals.tab = GlobalsHolder::Tab_Settings;
		settingsOpen = true;
		waveOpen = false;
	}
	ImGui::PopStyleColor(2*2+1);

	ImGui::End();
}

bool Gui::BeginSettingsWindow()
{
	if (!settingsOpen)
		return false;
	ImVec2 windowSize(Constants.gui.width,
			Globals.windowHeight - Constants.padding*2 -
			Constants.gui.menuBarGuiOffset - 40 - Constants.padding);
	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Settings", &settingsOpen,
			windowSize, Constants.gui.alpha, windowFlags);

	ImVec2 windowPos(
			Globals.windowWidth - Constants.gui.width - Constants.padding,
			Constants.padding + Constants.gui.menuBarGuiOffset +
			40 + Constants.padding);
	ImGui::SetWindowPos("Settings", windowPos, ImGuiSetCond_Always);
	return true;
}

void Gui::WaveWindow()
{
	if (!waveOpen)
		return;
	ImVec2 windowSize(Constants.gui.width,
			Globals.windowHeight - Constants.padding*2 -
			Constants.gui.menuBarGuiOffset - 40 - Constants.padding);
	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse;

	ImVec2 windowPos(
			Globals.windowWidth - Constants.gui.width - Constants.padding,
			Constants.padding + Constants.gui.menuBarGuiOffset +
			40 + Constants.padding);
	ImGui::SetWindowPos("Wave", windowPos, ImGuiSetCond_Always);

	ImGui::Begin("Wave", &waveOpen,
			windowSize, Constants.gui.alpha, windowFlags);

	static char text[1024*16] = "";
	// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
	// ImGui::PopStyleVar();
	ImGui::InputTextMultiline("##source", text, 1024*16,
			ImVec2(-1.0f, ImGui::GetTextLineHeight()*16),
			ImGuiInputTextFlags_AllowTabInput);

	ImGui::End();
}

void Gui::CreateFontTexture(ImFont *imFont)
{
	font = imFont;

	ImGuiIO& io = ImGui::GetIO();

	unsigned char *pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	glGenTextures(1, &fontTexture);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, pixels);

	io.Fonts->TexID = (void*)(intptr_t)fontTexture;
}

void Gui::Update(int dt)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = dt/1000.;
	io.MousePos = ImVec2(mousePosX, mousePosY);
	io.MouseDown[0] = mousePressed[0];
	io.MouseDown[1] = mousePressed[1];
}

void Gui::Draw()
{
	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glEnableVertexAttribArray(attribLocationPosition);
	glEnableVertexAttribArray(attribLocationUV);
	glEnableVertexAttribArray(attribLocationColor);
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(attribLocationPosition, 2, GL_FLOAT, GL_FALSE,
			sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(attribLocationUV, 2, GL_FLOAT, GL_FALSE,
			sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE,
			sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ImGui::Render();
}

Gui::~Gui()
{
	if (vaoHandle) glDeleteVertexArrays(1, &vaoHandle);
	if (vboHandle) glDeleteBuffers(1, &vboHandle);
	if (elementsHandle) glDeleteBuffers(1, &elementsHandle);
	vaoHandle = vboHandle = elementsHandle = 0;

	glDetachShader(shaderHandle, vertHandle);
	glDeleteShader(vertHandle);
	vertHandle = 0;

	glDetachShader(shaderHandle, fragHandle);
	glDeleteShader(fragHandle);
	fragHandle = 0;

	glDeleteProgram(shaderHandle);
	shaderHandle = 0;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();

	if (fontTexture) {
		glDeleteTextures(1, &fontTexture);
		ImGui::GetIO().Fonts->TexID = 0;
		fontTexture = 0;
	}
	ImGui::Shutdown();
}

