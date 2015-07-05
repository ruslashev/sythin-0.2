#include "note.hh"
#include "constants.hh"

#include <SFML/Graphics.hpp>

class MainLoop
{
public:
	sf::RenderWindow window;
	MainLoop() {
		sf::ContextSettings settings;
		settings.antialiasingLevel = Constants.antialiasing;
		window.create(
				sf::VideoMode(Globals.windowWidth, Globals.windowHeight),
				"sythin2",
				sf::Style::Titlebar | sf::Style::Close,
				settings);
	};
	bool Update() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
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

	Note notes[12];
	for (int i = 0; i < 12; i++) {
		notes[i] = Note();
		int x = Constants.padding + i*(Constants.rectangle.size + Constants.padding);
		int y = Globals.windowHeight - Constants.padding - Constants.rectangle.size;
		notes[i].SetPosition(x, y);
	}

	notes[0].key  = sf::Keyboard::Num1;
	notes[1].key  = sf::Keyboard::Num2;
	notes[2].key  = sf::Keyboard::Num3;
	notes[3].key  = sf::Keyboard::Num4;
	notes[4].key  = sf::Keyboard::Num5;
	notes[5].key  = sf::Keyboard::Num6;
	notes[6].key  = sf::Keyboard::Num7;
	notes[7].key  = sf::Keyboard::Num8;
	notes[8].key  = sf::Keyboard::Num9;
	notes[9].key  = sf::Keyboard::Num0;
	notes[10].key = sf::Keyboard::Dash;
	notes[11].key = sf::Keyboard::Equal;

	while (ml.Update()) {
		for (int i = 0; i < 12; i++) {
			if (sf::Keyboard::isKeyPressed(notes[i].key))
				notes[i].keyPressed = true;
			else {
				if (notes[i].keyPressed)
					notes[i].KeyReleased();
				notes[i].keyPressed = false;
			}

			notes[i].Draw(&ml.window);
		}

		ml.Display();
	}

	return 0;
}

