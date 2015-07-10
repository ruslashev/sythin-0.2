#include "constants.hh"
#include "conv.hh"
#include "note.hh"
#include "textures.hh"
#include "font.hh"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "../bzip2-1.0.6/bzlib.h"

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
		window.create(
				sf::VideoMode(Globals.windowWidth, Globals.windowHeight),
				"sythin2",
				sf::Style::Titlebar | sf::Style::Close,
				settings);
		window.setKeyRepeatEnabled(false);
	};
	bool Update() {
		window.clear(sf::Color::Black);
		return window.isOpen();
	}
	void Display() {
		window.display();
	}
};

int main()
{
	MainLoop ml;

	/*
	unsigned int uncompSize = 5 * 1024 * 1024;
	char *fontUncomp = new char [uncompSize];
	int bzerror = BZ2_bzBuffToBuffDecompress(fontUncomp,
			&uncompSize,
			(char*)_MesloLGMRegular_ttf.data,
			_MesloLGMRegular_ttf.size,
			0,
			0);
	if (bzerror != BZ_OK)
		return bzerror;
		*/

	std::unique_ptr<char> fontDataBuffer(new char [_MesloLGMRegular_ttf.size]);
	int fontDataBufferCounter = 0;
	for (size_t i = 0; i < _MesloLGMRegular_ttf.size*2; i += 2) {
		std::string byte = {
			_MesloLGMRegular_ttf.data[i],
			_MesloLGMRegular_ttf.data[i+1],
			'\0' };
		fontDataBuffer.get()[fontDataBufferCounter++] = std::stoi(byte, NULL, 16);
	}

	sf::Font font;
	if (!font.loadFromMemory(fontDataBuffer.get(), _MesloLGMRegular_ttf.size))
		return 1;
	std::unique_ptr<sf::Texture> noteNamesAtlas = textures::CreateNoteTexture(font);

	Note notes[12];
	for (int i = 0; i < 12; i++) {
		notes[i].SetHue((i/12.0)*360);
		int x = Constants.padding + i*(Constants.rectangle.size + Constants.padding);
		int y = Globals.windowHeight - Constants.padding - Constants.rectangle.size;
		notes[i].SetPosition(x, y);
		notes[i].SetTexture(noteNamesAtlas.get());
	}

	notes[ 0].key = sf::Keyboard::Num1;
	notes[ 1].key = sf::Keyboard::Num2;
	notes[ 2].key = sf::Keyboard::Num3;
	notes[ 3].key = sf::Keyboard::Num4;
	notes[ 4].key = sf::Keyboard::Num5;
	notes[ 5].key = sf::Keyboard::Num6;
	notes[ 6].key = sf::Keyboard::Num7;
	notes[ 7].key = sf::Keyboard::Num8;
	notes[ 8].key = sf::Keyboard::Num9;
	notes[ 9].key = sf::Keyboard::Num0;
	notes[10].key = sf::Keyboard::Dash;
	notes[11].key = sf::Keyboard::Equal;

	notes[ 0].SetNoteName(conv::A,  3);
	notes[ 1].SetNoteName(conv::As, 3);
	notes[ 2].SetNoteName(conv::B,  3);
	notes[ 3].SetNoteName(conv::C,  3);
	notes[ 4].SetNoteName(conv::Cs, 3);
	notes[ 5].SetNoteName(conv::D,  3);
	notes[ 6].SetNoteName(conv::Ds, 3);
	notes[ 7].SetNoteName(conv::E,  3);
	notes[ 8].SetNoteName(conv::F,  3);
	notes[ 9].SetNoteName(conv::Fs, 3);
	notes[10].SetNoteName(conv::G,  3);
	notes[11].SetNoteName(conv::Gs, 3);

	while (ml.Update()) {
		sf::Time realTime = ml.clock.getElapsedTime();
		while (ml.simulatedTime < realTime) {
			while (ml.window.pollEvent(ml.event)) {
				if (ml.event.type == sf::Event::Closed)
					ml.window.close();
				if (ml.event.type == sf::Event::KeyPressed)
					for (int i = 0; i < 12; i++)
						if (ml.event.key.code == notes[i].key) {
							notes[i].keyPressed = true;
							notes[i].KeyPressed();
						}
				if (ml.event.type == sf::Event::KeyReleased)
					for (int i = 0; i < 12; i++)
						if (ml.event.key.code == notes[i].key) {
							notes[i].keyPressed = false;
							notes[i].KeyReleased();
						}
			}
			for (int i = 0; i < 12; i++) {
				notes[i].Update();
			}
			ml.simulatedTime += sf::milliseconds(Constants.updateMilliseconds);
		}

		for (int i = 0; i < 12; i++) {
			notes[i].Draw(&ml.window);
		}

		ml.Display();
	}

	return 0;
}

