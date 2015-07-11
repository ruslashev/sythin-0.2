#ifndef CONV_HH
#define CONV_HH

#include <SFML/Graphics.hpp>

namespace conv {

sf::Color HSVtoRGB(int h_abs, int s_abs, int v_abs);

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
double NoteNameToFreq(Name name, int octave);

}

#endif

