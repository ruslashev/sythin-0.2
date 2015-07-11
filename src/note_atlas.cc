#include "constants.hh"
#include "conv.hh"
#include "note_atlas.hh"

static std::map<std::string, sf::IntRect> positions;

bool note_atlas::CreateNoteTexture(sf::Font &font, sf::Texture *texture)
{
	struct noteSymbol { char letter, accidental; int octave; };
	std::vector<noteSymbol> noteNames;
	for (int i = 0; i < Constants.noteAtlas.precomputedOctaves; i++) {
		noteNames.push_back({ 'A', ' ', i+1 });
		noteNames.push_back({ 'A', '#', i+1 });
		noteNames.push_back({ 'B', ' ', i+1 });
		noteNames.push_back({ 'C', ' ', i+1 });
		noteNames.push_back({ 'C', '#', i+1 });
		noteNames.push_back({ 'D', ' ', i+1 });
		noteNames.push_back({ 'D', '#', i+1 });
		noteNames.push_back({ 'E', ' ', i+1 });
		noteNames.push_back({ 'F', ' ', i+1 });
		noteNames.push_back({ 'F', '#', i+1 });
		noteNames.push_back({ 'G', ' ', i+1 });
		noteNames.push_back({ 'G', '#', i+1 });
	}

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(Constants.text.size);

	sf::Color textColor = conv::HSVtoRGB(0, 0, Constants.text.colorValue),
		background = conv::HSVtoRGB(0, 0, Constants.text.backgroundValue);

	sf::RenderTexture textRT;
	if (!textRT.create(Constants.noteAtlas.width, Constants.noteAtlas.height))
		return false;
	textRT.clear(background);

	int counterX = 0;

	for (auto &note : noteNames) {
		int textureSegmentWidth = 0;
		text.setString(note.letter);
		textureSegmentWidth += text.getLocalBounds().width;
		if (note.accidental != ' ') {
			text.setString(note.accidental);
			textureSegmentWidth += text.getLocalBounds().width;
			textureSegmentWidth += Constants.text.spacing;
		}
		text.setString(std::to_string(note.octave));
		textureSegmentWidth += text.getLocalBounds().width;
		textureSegmentWidth += Constants.text.spacing;
		int textureSegmentHeight = text.getLocalBounds().height;

		text.setString(note.letter);
		auto rect = text.getLocalBounds();
		text.setPosition(-rect.left, -rect.top);
		text.move(counterX, 0);

		text.setColor(textColor);
		textRT.draw(text);

		int xof = 0;
		if (note.accidental != ' ') {
			text.setString(note.accidental);
			auto thisRect = text.getLocalBounds();
			text.setPosition(-thisRect.left, -thisRect.top);
			xof = rect.width + Constants.text.spacing;
			text.move(xof, 0);
			text.move(counterX, 0);

			text.setColor(textColor);
			textRT.draw(text);

			rect = text.getLocalBounds();
		}

		text.setString(std::to_string(note.octave));
		auto thisRect = text.getLocalBounds();
		text.setPosition(-thisRect.left, -thisRect.top);
		text.move(xof + rect.width + Constants.text.spacing, 0);
		text.move(counterX, 0);

		text.setColor(textColor);
		textRT.draw(text);

		textRT.display();

		std::string noteStr = "";
		noteStr += note.letter;
		noteStr += note.accidental;
		noteStr += std::to_string(note.octave);
		positions[noteStr] =
			sf::IntRect(counterX, 0, textureSegmentWidth, textureSegmentHeight);
		counterX += textureSegmentWidth;
	}
	*texture = textRT.getTexture();
	return true;
}

sf::IntRect note_atlas::LookupNotePosition(char letter, char accidental, int octave)
{
	std::string noteStr = "";
	noteStr += letter;
	noteStr += accidental;
	noteStr += std::to_string(octave);
	return positions[noteStr];
}

