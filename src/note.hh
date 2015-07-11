#ifndef NOTE_HH
#define NOTE_HH

#include "conv.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class Note
{
	sf::RectangleShape rectangleShape;
	// sf::RectangleShape lineShape;
	sf::Color
		color, outlineColor,
		pressedColor, pressedOutlineColor;
	sf::RectangleShape textSprite;

	sf::SoundBuffer playSoundBuffer;
	sf::Sound playSound;

	void generateSamples();
	void createSprites();
public:
	sf::Keyboard::Key key;
	double baseFrequency;
	bool keyPressed;

	char noteLetter;
	char noteAccidental;
	int noteOctave;

	Note();
	void SetPosition(int x, int y);
	void SetHue(int h);
	void SetNoteName(conv::Name noteName, int octave);
	void SetTexture(sf::Texture *texture);
	void Draw(sf::RenderWindow *window);
	void Update();

	void KeyPressed();
	void KeyReleased();
};

#endif

