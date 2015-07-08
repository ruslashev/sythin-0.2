#include "constants.hh"
#include "conv.hh"
#include "note.hh"
#include <SFML/Graphics.hpp>

class MainLoop
{
public:
	sf::RenderWindow window;
	sf::Event event;
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

	sf::Font font;
	if (!font.loadFromFile("MuseoSans_500.otf"))
		return 1;

	Note notes[12];
	for (int i = 0; i < 12; i++) {
		notes[i].SetHue((i/12.0)*360);
		int x = Constants.padding + i*(Constants.rectangle.size + Constants.padding);
		int y = Globals.windowHeight - Constants.padding - Constants.rectangle.size;
		notes[i].SetPosition(x, y);
		notes[i].nameText.setFont(font);
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
			notes[i].Draw(&ml.window);
		}

		ml.Display();

        sf::sleep(sf::milliseconds(100));
	}

	return 0;
}

