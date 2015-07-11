#ifndef NOTE_ATLAS_HH
#define NOTE_ATLAS_HH

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <vector>

namespace note_atlas {

bool CreateNoteTexture(sf::Font &font, sf::Texture *texture);

sf::IntRect LookupNotePosition(char letter, char accidental, int octave);

}

#endif

