#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>
#include "../imgui/imgui.h"

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
		int graphHeight = 40;
		int samplesInPreviewMin = 5;
		int samplesInPreviewMax = 22050;
		float samplesInPreviewPower = 3.0;
		int volumePercent = 16;
		const char *VFCModeString =
			"Linear\0Exponential\0Square Root\0\0";
		int menuBarGuiOffset = 25;
		struct {
			ImColor modePlayingIdle        = ImColor::HSV( 60/360.,  37/100.,  40/100., 1.00);
			ImColor modePlayingHovered     = ImColor::HSV( 60/360.,  40/100.,  67/100., 1.00);
			ImColor modePlayingActive      = ImColor::HSV( 60/360.,  37/100.,  80/100., 1.00);

			ImColor modeWritingIdle       = ImColor::HSV(345/360.,  37/100.,  40/100., 1.00);
			ImColor modeWritingHovered    = ImColor::HSV(345/360.,  40/100.,  67/100., 1.00);
			ImColor modeWritingActive     = ImColor::HSV(345/360.,  37/100.,  80/100., 1.00);

			ImColor modeRepeatingIdle    = ImColor::HSV(150/360.,  37/100.,  40/100., 1.00);
			ImColor modeRepeatingHovered = ImColor::HSV(150/360.,  40/100.,  67/100., 1.00);
			ImColor modeRepeatingActive  = ImColor::HSV(150/360.,  37/100.,  80/100., 1.00);

			int modeSpacing = 11;
		} menuBar {};
		float exponentialStrengthMin = 1.0;
		float exponentialStrengthMax = 1000.0;
		struct {
			ImColor idle    = ImColor::HSV(0/360.,  37/100.,  40/100., 1.00);
			ImColor hovered = ImColor::HSV(0/360.,  40/100.,  67/100., 1.00);
			ImColor active  = ImColor::HSV(0/360.,  37/100.,  80/100., 1.00);
		} tabs {};
	} gui {};
	const char *defaultWaveScript =
		"function wave(w, t)\n"
		"	return sin(w*t)\n"
		"end\n";
} Constants {};

struct GlobalsHolder
{
	bool quit = false;

	int windowWidth = Constants.notesViewWidth + Constants.padding + Constants.gui.width;
	int windowHeight = 700;
	int volume = 5000;

	enum {
		Mode_Playing,
		Mode_Writing,
		Mode_Repeating
	} mode = Mode_Playing;

	enum {
		Tab_Settings,
		Tab_Wave
	} tab = Tab_Wave;

	bool playingOnKeys = true;
	bool showDemo = false;
};

extern GlobalsHolder Globals;

#endif

