#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>

const struct
{
	struct {
		int size = 60;
		int outline = -3;
		sf::Color fillColor = sf::Color(240, 240, 240);
		sf::Color outlineColor = sf::Color(190, 190, 190);
	} rectangle {};
	int padding = 10;
} Constants {};

#endif

