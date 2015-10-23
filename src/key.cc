#include "key.hh"

#include "constants.hh"
#include "conv.hh"
#include "note_atlas.hh"

Key::Key()
{
	rectangleShape.setSize(sf::Vector2f(
				Constants.rectangle.size, Constants.rectangle.size));
	rectangleShape.setOutlineThickness(Constants.rectangle.outline);

	// lineShape.setSize(sf::Vector2f(
	// 			Constants.line.thickness,
	// 			Globals.windowHeight - Constants.padding*2));
	// lineShape.setFillColor(Constants.line.color);
	keyPressed = false;

	playSound.setBuffer(playSoundBuffer);
}

void Key::GenerateSamples()
{
	sf::Int16 playSamples[Constants.samplesPerSecond];
	double omega = 2*M_PI*baseFrequency;
	double period = 1 / baseFrequency;
	unsigned int samples = period*Constants.samplesPerSecond;
	double x = 0;
	for (unsigned int i = 0; i < samples; i++) {
		playSamples[i] = Globals.volume*sin(omega*x);
		x += 1.0/Constants.samplesPerSecond;
	}

	if (!playSoundBuffer.loadFromSamples(playSamples, samples,
				Constants.channels, Constants.samplesPerSecond)) {
		puts("Failed to copy sound buffer");
		throw;
	}
}

void Key::createSprites()
{
	sf::IntRect rect = note_atlas::LookupNotePosition(noteLetter,
				noteAccidental, noteOctave);
	textSprite.setSize(sf::Vector2f(rect.width, rect.height));
	textSprite.setTextureRect(rect);
	textSprite.setOutlineThickness(Constants.text.outline);
	textSprite.setOutlineColor(conv::HSVtoRGB(0, 0, Constants.text.backgroundValue));
	textSprite.setPosition(
			rectangleShape.getPosition().x +
			rectangleShape.getLocalBounds().width/2 -
			textSprite.getLocalBounds().width/2 +
			Constants.text.outline,
			rectangleShape.getPosition().y +
			rectangleShape.getLocalBounds().height/2 -
			textSprite.getLocalBounds().height/2 +
			Constants.text.outline);
}

void Key::SetPosition(int x, int y)
{
	rectangleShape.setPosition(x, y);
	// lineShape.setPosition(x + Constants.rectangle.size/2, Constants.padding);
}

void Key::SetHue(int h)
{
	int saturation = Constants.rectangle.saturation;
	int pressedSaturation = Constants.rectangle.pressedSaturation;
	int value = Constants.rectangle.value;
	int outlineValue = Constants.rectangle.outlineValue;

	color               = conv::HSVtoRGB(h, saturation,        value);
	outlineColor        = conv::HSVtoRGB(h, saturation,        outlineValue);
	pressedColor        = conv::HSVtoRGB(h, pressedSaturation, value);
	pressedOutlineColor = conv::HSVtoRGB(h, pressedSaturation, outlineValue);
}

void Key::SetNoteName(conv::Name nNoteName, int octave)
{
	noteName = nNoteName;
	baseFrequency = conv::NoteNameToFreq(noteName, octave);
	GenerateSamples();

	switch (noteName) {
		case conv::A: case conv::As:
			noteLetter = 'A'; break;
		case conv::B:
			noteLetter = 'B'; break;
		case conv::C: case conv::Cs:
			noteLetter = 'C'; break;
		case conv::D: case conv::Ds:
			noteLetter = 'D'; break;
		case conv::E:
			noteLetter = 'E'; break;
		case conv::F: case conv::Fs:
			noteLetter = 'F'; break;
		case conv::G: case conv::Gs:
			noteLetter = 'G'; break;
	}
	switch (noteName) {
		case conv::As:
		case conv::Cs:
		case conv::Ds:
		case conv::Fs:
		case conv::Gs:
			noteAccidental = '#';
			break;
		default:
			noteAccidental = ' ';
			break;
	}
	noteOctave = octave;
	createSprites();
}

void Key::SetTexture(sf::Texture *texture)
{
	textSprite.setTexture(texture);
}

void Key::Draw(sf::RenderWindow *window)
{
	// window->draw(lineShape);

	if (keyPressed) {
		rectangleShape.setFillColor(pressedColor);
		rectangleShape.setOutlineColor(pressedOutlineColor);
	} else {
		rectangleShape.setFillColor(color);
		rectangleShape.setOutlineColor(outlineColor);
	}

	window->draw(rectangleShape);

	window->draw(textSprite);
}

void Key::KeyPressed()
{
	playSound.setLoop(true);
	playSound.play();
}

void Key::KeyReleased()
{
	playSound.setLoop(false);
}

