#ifndef NOTE_HH
#define NOTE_HH

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class Note
{
	sf::RectangleShape rectangleShape;
	sf::RectangleShape lineShape;
	sf::Color baseFillColor, baseOutlineColor,
		pressedFillColor, pressedOutlineColor;

	sf::SoundBuffer playSoundBuffer;
	sf::Sound playSound;

	sf::RenderTexture renderTexture;
	sf::Sprite unpressedTextSprite, pressedTextSprite;
public:
	sf::Keyboard::Key key;
	double baseFrequency;
	bool keyPressed;

	sf::Text nameText;
	char noteLetter;
	char noteAccidental;
	int noteOctave;

	Note();
	void SetPosition(int x, int y);
	void SetHue(int h);
	void SetFrequency(double frequency);
	void SetNoteName(char letter, char accidental, int octave);
	void Draw(sf::RenderWindow *window);
	void Update();

	void KeyPressed();
	void KeyReleased();
};

#endif

