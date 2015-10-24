#ifndef NOTE_HH
#define NOTE_HH

#include <SFML/Audio.hpp>

struct NoteSym
{
	char letter, accidental;
	int octave;
};

class Note
{
	sf::SoundBuffer soundBuffer;
	sf::Sound sound;
public:
	double baseFrequency;

	Note();
	~Note();

	void GenerateSamples();
};

#endif

