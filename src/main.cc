#include "constants.hh"
#include "conv.hh"
#include "font.hh"
#include "fontloader.hh"
#include "gui.hh"
#include "key.hh"
#include "note_atlas.hh"
#include "note.hh"
#include "script.hh"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include "../bzip2-1.0.6/bzlib.h"
#include "../imgui/imgui.h"
#include <memory>

class MainLoop
{
public:
	sf::RenderWindow window;
	sf::Event event;
	sf::Time simulatedTime;
	sf::Clock clock;
	MainLoop() {
		sf::ContextSettings settings;
		settings.antialiasingLevel = Constants.antialiasing;
		settings.majorVersion = 2;
		settings.minorVersion = 1;
		window.create(sf::VideoMode(Globals.windowWidth, Globals.windowHeight),
				"sythin2",
				sf::Style::Titlebar | sf::Style::Close,
				settings);
		window.setKeyRepeatEnabled(false);
	};
	bool Update() {
		window.clear(Constants.backgroundColor);
		return window.isOpen() && !Globals.quit;
	}
	void Display() {
		window.display();
	}
};

int main()
{
	Script ye;

	MainLoop ml;

	Gui gui;

	sf::Font sfFont;
	ImFont *imFont;
	std::unique_ptr<char> fontFileBuffer;
	if (!FontLoader::loadEmbeddedFont(sfFont, imFont,
				fontFileBuffer,
				_CommeLight_ttf.data, _CommeLight_ttf.size))
		return 1;

	gui.CreateFontTexture(imFont);

	sf::Texture noteNamesAtlas;
	if (!note_atlas::CreateNoteTexture(sfFont, &noteNamesAtlas))
		return 1;

	Key keys[3][12];
	int hue = 0;
	for (int r = 0; r < 3; r++)
		for (int i = 11; i >= 0; i--) {
			keys[r][i].SetHue((hue++/36.0)*360);
			int x = Constants.padding + i*(Constants.rectangle.size + Constants.padding);
			int y = Globals.windowHeight - Constants.padding - Constants.rectangle.size -
				(3-r-1)*(Constants.rectangle.size + Constants.padding);
			keys[r][i].SetPosition(x, y);
			keys[r][i].SetTexture(&noteNamesAtlas);
		}

	keys[0][ 0].key = sf::Keyboard::Num1;
	keys[0][ 1].key = sf::Keyboard::Num2;
	keys[0][ 2].key = sf::Keyboard::Num3;
	keys[0][ 3].key = sf::Keyboard::Num4;
	keys[0][ 4].key = sf::Keyboard::Num5;
	keys[0][ 5].key = sf::Keyboard::Num6;
	keys[0][ 6].key = sf::Keyboard::Num7;
	keys[0][ 7].key = sf::Keyboard::Num8;
	keys[0][ 8].key = sf::Keyboard::Num9;
	keys[0][ 9].key = sf::Keyboard::Num0;
	keys[0][10].key = sf::Keyboard::Dash;
	keys[0][11].key = sf::Keyboard::Equal;
	keys[0][ 0].SetNoteName(conv::C,  4);
	keys[0][ 1].SetNoteName(conv::Cs, 4);
	keys[0][ 2].SetNoteName(conv::D,  4);
	keys[0][ 3].SetNoteName(conv::Ds, 4);
	keys[0][ 4].SetNoteName(conv::E,  4);
	keys[0][ 5].SetNoteName(conv::F,  4);
	keys[0][ 6].SetNoteName(conv::Fs, 4);
	keys[0][ 7].SetNoteName(conv::G,  4);
	keys[0][ 8].SetNoteName(conv::Gs, 4);
	keys[0][ 9].SetNoteName(conv::A,  4);
	keys[0][10].SetNoteName(conv::As, 4);
	keys[0][11].SetNoteName(conv::B,  4);

	keys[1][ 0].key = sf::Keyboard::Q;
	keys[1][ 1].key = sf::Keyboard::W;
	keys[1][ 2].key = sf::Keyboard::E;
	keys[1][ 3].key = sf::Keyboard::R;
	keys[1][ 4].key = sf::Keyboard::T;
	keys[1][ 5].key = sf::Keyboard::Y;
	keys[1][ 6].key = sf::Keyboard::U;
	keys[1][ 7].key = sf::Keyboard::I;
	keys[1][ 8].key = sf::Keyboard::O;
	keys[1][ 9].key = sf::Keyboard::P;
	keys[1][10].key = sf::Keyboard::LBracket;
	keys[1][11].key = sf::Keyboard::RBracket;
	keys[1][ 0].SetNoteName(conv::C,  3);
	keys[1][ 1].SetNoteName(conv::Cs, 3);
	keys[1][ 2].SetNoteName(conv::D,  3);
	keys[1][ 3].SetNoteName(conv::Ds, 3);
	keys[1][ 4].SetNoteName(conv::E,  3);
	keys[1][ 5].SetNoteName(conv::F,  3);
	keys[1][ 6].SetNoteName(conv::Fs, 3);
	keys[1][ 7].SetNoteName(conv::G,  3);
	keys[1][ 8].SetNoteName(conv::Gs, 3);
	keys[1][ 9].SetNoteName(conv::A,  3);
	keys[1][10].SetNoteName(conv::As, 3);
	keys[1][11].SetNoteName(conv::B,  3);

	keys[2][ 0].key = sf::Keyboard::A;
	keys[2][ 1].key = sf::Keyboard::S;
	keys[2][ 2].key = sf::Keyboard::D;
	keys[2][ 3].key = sf::Keyboard::F;
	keys[2][ 4].key = sf::Keyboard::G;
	keys[2][ 5].key = sf::Keyboard::H;
	keys[2][ 6].key = sf::Keyboard::J;
	keys[2][ 7].key = sf::Keyboard::K;
	keys[2][ 8].key = sf::Keyboard::L;
	keys[2][ 9].key = sf::Keyboard::SemiColon;
	keys[2][10].key = sf::Keyboard::Quote;
	keys[2][11].key = sf::Keyboard::Return;
	keys[2][ 0].SetNoteName(conv::C,  2);
	keys[2][ 1].SetNoteName(conv::Cs, 2);
	keys[2][ 2].SetNoteName(conv::D,  2);
	keys[2][ 3].SetNoteName(conv::Ds, 2);
	keys[2][ 4].SetNoteName(conv::E,  2);
	keys[2][ 5].SetNoteName(conv::F,  2);
	keys[2][ 6].SetNoteName(conv::Fs, 2);
	keys[2][ 7].SetNoteName(conv::G,  2);
	keys[2][ 8].SetNoteName(conv::Gs, 2);
	keys[2][ 9].SetNoteName(conv::A,  2);
	keys[2][10].SetNoteName(conv::As, 2);
	keys[2][11].SetNoteName(conv::B,  2);

	while (ml.Update()) {
		sf::Time realTime = ml.clock.getElapsedTime();
		while (ml.simulatedTime < realTime) {
			while (ml.window.pollEvent(ml.event)) {
				switch (ml.event.type) {
					case sf::Event::Closed:
						ml.window.close();
						break;
					case sf::Event::KeyPressed: {
						ImGuiIO& io = ImGui::GetIO();
						io.KeysDown[ml.event.key.code] = true;
						io.KeyCtrl = ml.event.key.control;
						io.KeyShift = ml.event.key.shift;
						if (Globals.playingOnKeys)
							for (int r = 0; r < 3; r++)
								for (int i = 0; i < 12; i++)
									if (ml.event.key.code == keys[r][i].key) {
										keys[r][i].keyPressed = true;
										keys[r][i].KeyPressed();
										// 10 levels of indentation woo
									}
						break;
					}
					case sf::Event::KeyReleased: {
						ImGuiIO& io = ImGui::GetIO();
						io.KeysDown[ml.event.key.code] = false;
						io.KeyCtrl = ml.event.key.control;
						io.KeyShift = ml.event.key.shift;
						if (Globals.playingOnKeys)
							for (int r = 0; r < 3; r++)
								for (int i = 0; i < 12; i++)
									if (ml.event.key.code == keys[r][i].key) {
										keys[r][i].keyPressed = false;
										keys[r][i].KeyReleased();
									}
						break;
					}
					case sf::Event::TextEntered:
						if (ml.event.text.unicode > 0 &&
								ml.event.text.unicode < 0x10000)
							ImGui::GetIO().AddInputCharacter(ml.event.text.unicode);
						break;
					case sf::Event::MouseButtonPressed:
						gui.mousePressed[ml.event.mouseButton.button] = true;
						break;
					case sf::Event::MouseButtonReleased:
						gui.mousePressed[ml.event.mouseButton.button] = false;
						break;
					case sf::Event::MouseWheelMoved:
						ImGui::GetIO().MouseWheel += (float)ml.event.mouseWheel.delta;
						break;
					case sf::Event::MouseMoved:
						gui.mousePosX = ml.event.mouseMove.x;
						gui.mousePosY = ml.event.mouseMove.y;
						break;
					default:
						break;
				}
			}

			gui.Update(Constants.updateMilliseconds);

			ml.simulatedTime += sf::milliseconds(Constants.updateMilliseconds);
		}

		ImGui::NewFrame();

		gui.TabBar();

		if (gui.BeginSettingsWindow()) {
			if (ImGui::CollapsingHeader("Sampling options")) {
				if (ImGui::Button("Regenerate samples"))
					for (int r = 0; r < 3; r++)
						for (int i = 0; i < 12; i++)
							keys[r][i].GenerateSamples();

				ImGui::Text("Preview");
				static int samplesInPreview = 2000;
				float samplesInPreviewFloat = samplesInPreview;
				ImGui::SliderFloat("samples in preview", &samplesInPreviewFloat,
						Constants.gui.samplesInPreviewMin,
						Constants.gui.samplesInPreviewMax,
						"%.0f", Constants.gui.samplesInPreviewPower);
				samplesInPreview = samplesInPreviewFloat;

				ImGui::Spacing();

				struct {
					Key *key;
					const char *noteName;
					float samples[44100];
				} previewNotes[5] = {
					{ &keys[2][ 0], "C2",  {} },
					{ &keys[2][10], "A#2", {} },
					{ &keys[1][ 3], "D#3", {} },
					{ &keys[0][ 1], "C#4", {} },
					{ &keys[0][11], "B4",  {} }
				};
				for (int n = 0; n < 5; n++) {
					double x = 0;
					for (int i = 0; i < samplesInPreview; i++) {
						previewNotes[n].samples[i] = Globals.volume*
							sin(2*M_PI*previewNotes[n].key->baseFrequency*x);
						x += 1.0/Constants.samplesPerSecond;
					}
					ImGui::PlotLines(previewNotes[n].noteName, previewNotes[n].samples,
							samplesInPreview, 0, "", -32767, 32767,
							ImVec2(0, Constants.gui.graphHeight));
				}

				ImGui::Spacing();

				static float volumePercent = Constants.gui.volumePercent;
				ImGui::SliderFloat("volume", &volumePercent, 0.0f, 100.0f, "%.1f%%");
				Globals.volume = (32767.0*volumePercent)/100.0;

				ImGui::Spacing();

				if (ImGui::TreeNode("Volume/Frequency compensation\n"
							"(now disabled in code beacause it's shit)"))
					ImGui::TreePop();
			}
			ImGui::End();
		}

		gui.WaveWindow();

		gui.MainMenuBar();

		if (Globals.showDemo)
			ImGui::ShowTestWindow(&Globals.showDemo);

		gui.Draw();

		for (int r = 0; r < 3; r++)
			for (int i = 0; i < 12; i++)
				keys[r][i].Draw(&ml.window);

		ml.Display();
	}

	return 0;
}

