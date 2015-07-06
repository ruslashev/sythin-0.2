#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>

const struct
{
	int antialiasing = 6;
	struct {
		int size = 50;
		int outline = -2;
		int baseSaturation = 10;
		int pressedSaturation = 59;

		int baseValue = 97;
		int outlineValue = 78;
	} rectangle {};
	struct {
		int thickness = 2;
		sf::Color color = sf::Color(30, 30, 30);
	} line {};
	int padding = 10;

	int channels = 1;
	int samplesPerSecond = 44100;
	double stdTuning = 440;
} Constants {};

struct
{
	int windowWidth =
		Constants.padding +
		12*(Constants.padding + Constants.rectangle.size);
	int windowHeight = windowWidth;
	int volume = 5000;
} Globals {};

#endif

