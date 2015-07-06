#ifndef CONV_HH
#define CONV_HH

#include <SFML/Graphics.hpp>
sf::Color HSVtoRGB(int h_abs, int s_abs, int v_abs);

enum Name {
	A,
	As,
	B,
	C,
	Cs,
	D,
	Ds,
	E,
	F,
	Fs,
	G,
	Gs
};
double NoteNameToFreq(Name name, int octave);

#endif

