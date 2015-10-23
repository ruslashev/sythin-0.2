#ifndef KEY_HH
#define KEY_HH

#include "conv.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class Key
{
	sf::RectangleShape rectangleShape;
	// sf::RectangleShape lineShape;
	sf::Color
		color, outlineColor,
		pressedColor, pressedOutlineColor;
	sf::RectangleShape textSprite;

	sf::SoundBuffer playSoundBuffer;
	sf::Sound playSound;

	void createSprites();
public:
	sf::Keyboard::Key key;
	double baseFrequency;
	bool keyPressed;

	conv::Name noteName;
	char noteLetter;
	char noteAccidental;
	int noteOctave;

	Key();
	void GenerateSamples();
	void SetPosition(int x, int y);
	void SetHue(int h);
	void SetNoteName(conv::Name nNoteName, int octave);
	void SetTexture(sf::Texture *texture);
	void Draw(sf::RenderWindow *window);

	void KeyPressed();
	void KeyReleased();
};

#endif

