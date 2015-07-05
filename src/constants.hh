#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>

const struct
{
	int antialiasing = 6;
	struct {
		int size = 50;
		int outline = -2;
		// sf::Color fillColor = sf::Color(230, 230, 230);
		// sf::Color outlineColor = sf::Color(190, 190, 190);
		sf::Color fillColor = sf::Color(230, 230, 230); // HSV: * 0 90
		sf::Color outlineColor = sf::Color(178, 178, 178); // HSV: * 0 70

		sf::Color pressedFillColor = sf::Color(169, 2, 217); // HSV: 284 99 90
		sf::Color pressedOutlineColor = sf::Color(132, 2, 178); // HSV: 284 99 70
	} rectangle {};
	struct {
		int thickness = 2;
		sf::Color color = sf::Color(30, 30, 30);
	} line {};
	int padding = 10;

	int channels = 1;
	int samplesPerSecond = 44100;
} Constants {};

struct
{
	int windowWidth =
		Constants.padding +
		12*(Constants.padding + Constants.rectangle.size);
	int windowHeight = windowWidth;
	int volume = 30000;
} Globals {};

#endif

