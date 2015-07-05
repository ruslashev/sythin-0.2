#include "note.hh"
#include "constants.hh"

#include <SFML/Graphics.hpp>

class MainLoop
{
public:
	sf::RenderWindow window;
	MainLoop() {
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
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

	while (ml.Update()) {
		for (int i = 0; i < 12; i++) {
			notes[i].Draw(&ml.window);
		}

		ml.Display();
	}

	return 0;
}

