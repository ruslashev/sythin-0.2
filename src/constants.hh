#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>

const struct
{
	int updateMilliseconds = 16;
	int antialiasing = 6;
	unsigned int fontSizeGuess = 10 * 1024 * 1024;

	sf::Color backgroundColor = sf::Color(16, 16, 16);
	struct {
		int size = 50;
		int outline = -2;

		int saturation = 10;
		int pressedSaturation = 59;

		int value = 97;
		int outlineValue = 78;
	} rectangle {};
	struct {
		int size = 19;
		int spacing = -1;
		int outline = 4;
		int colorValue = 15;
		int backgroundValue = 96;
	} text {};
	struct {
		int thickness = 2;
		sf::Color color = sf::Color(30, 30, 30);
	} line {};
	int padding = 10;
	struct {
		int precomputedOctaves = 6;
		int width = 4000;
		int height = 500;
	} noteAtlas {};

	int channels = 1;
	int samplesPerSecond = 44100;
	double stdTuning = 440;

	int notesViewWidth =
		padding +
		12*(rectangle.size + padding);
	struct {
		int width = 500;
		int fontSize = 18;
		float alpha = 0.5f;
	} gui {};
} Constants {};

struct
{
	int windowWidth = Constants.notesViewWidth + Constants.padding + Constants.gui.width;
	int windowHeight = 700;
	int volume = 5000;
} Globals {};

#endif

