#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>

const struct
{
	struct {
		int size = 50;
		int outline = -2;
		sf::Color fillColor = sf::Color(230, 230, 230);
		sf::Color outlineColor = sf::Color(190, 190, 190);
	} rectangle {};
	struct {
		int thickness = 2;
		sf::Color color = sf::Color(30, 30, 30);
	} line {};
	int padding = 10;
} Constants {};

struct
{
	int windowWidth =
		Constants.padding +
		12*(Constants.padding + Constants.rectangle.size);
	int windowHeight = windowWidth;
} Globals {};

#endif

