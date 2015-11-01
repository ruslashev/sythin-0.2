#ifndef NOTE_HH
#define NOTE_HH

#include <SFML/Audio.hpp>

#include "script.hh"

namespace note {

enum Name {
	C,
	Cs,
	D,
	Ds,
	E,
	F,
	Fs,
	G,
	Gs,
	A,
	As,
	B,
};

}

class Note
{
	sf::SoundBuffer soundBuffer;
public:
	sf::Sound sound;

	note::Name name;
	char letter, accidental;
	int octave;

	Note();
	Note(note::Name nName, int nOctave);

	void GenerateSamples(Script *script);
};

#endif

