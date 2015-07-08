#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>

const struct
{
	int antialiasing = 6;
	struct {
		int size = 50;
		int outline = -2;

		int saturation = 10;
		int pressedSaturation = 59;

		int value = 97;
		int outlineValue = 78;
	} rectangle {};
	struct {
		int size = 20;
		int spacing = -1;

		int saturation = 70;
		int value = 15;
		int pressedSaturation = 8;
		int pressedValue = 97;
	} text {};
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

