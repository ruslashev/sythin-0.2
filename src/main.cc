#include "note.hh"

#include <SFML/Graphics.hpp>

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(800, 600), "sythin2",
			sf::Style::Titlebar | sf::Style::Close,
			settings);

	Note one;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);

		one.Draw(&window);

		window.display();
	}

	return 0;
}

