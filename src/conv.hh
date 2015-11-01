#ifndef CONV_HH
#define CONV_HH

#include <SFML/Graphics.hpp>

#include "note.hh"

namespace conv {

double NoteNameToFreq(note::Name name, int octave);

sf::Color HSVtoRGB(int h_abs, int s_abs, int v_abs);

}

#endif

