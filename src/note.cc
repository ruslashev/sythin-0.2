#include "note.hh"

#include "constants.hh"
#include "conv.hh"

Note::Note()
{
	rectangleShape.setSize(sf::Vector2f(
				Constants.rectangle.size, Constants.rectangle.size));
	rectangleShape.setOutlineThickness(Constants.rectangle.outline);

	lineShape.setSize(sf::Vector2f(
				Constants.line.thickness,
				Globals.windowHeight - Constants.padding*2));
	lineShape.setFillColor(Constants.line.color);
	keyPressed = false;
}

void Note::generateSamples()
{
	sf::Int16 playSamples[Constants.samplesPerSecond];
	double x = 0;
	for (int i = 0; i < Constants.samplesPerSecond; i++) {
		playSamples[i] = Globals.volume * sin(M_PI*2*x);
		x += baseFrequency/Constants.samplesPerSecond;
	}

	if (!playSoundBuffer.loadFromSamples(playSamples,
				Constants.samplesPerSecond,
				Constants.channels,
				Constants.samplesPerSecond))
		puts("failed to copy sound buffer");

	playSound.setBuffer(playSoundBuffer);
	playSound.setLoop(true);
}

void Note::createTextures()
{
	nameText.setCharacterSize(Constants.text.size);
	int total_width = 0;
	nameText.setString(noteLetter);
	total_width += nameText.getLocalBounds().width;
	if (noteAccidental != ' ') {
		nameText.setString(noteAccidental);
		total_width += nameText.getLocalBounds().width;
		total_width += Constants.text.spacing;
	}
	nameText.setString(std::to_string(noteOctave));
	total_width += nameText.getLocalBounds().width;
	total_width += Constants.text.spacing;
	int total_height = nameText.getLocalBounds().height;

	if (!textRenderTexture.create(total_width, total_height))
		throw;
	textRenderTexture.clear(color);
	if (!pressedTextRenderTexture.create(total_width, total_height))
		throw;
	pressedTextRenderTexture.clear(pressedColor);

	nameText.setCharacterSize(Constants.text.size);

	nameText.setString(noteLetter);
	auto rect = nameText.getLocalBounds();
	nameText.setPosition(-rect.left, -rect.top);

	nameText.setColor(textColor);
	textRenderTexture.draw(nameText);
	nameText.setColor(pressedTextColor);
	pressedTextRenderTexture.draw(nameText);

	int xof = 0;
	if (noteAccidental != ' ') {
		nameText.setString(noteAccidental);
		auto thisRect = nameText.getLocalBounds();
		nameText.setPosition(-thisRect.left, -thisRect.top);
		xof = rect.width + Constants.text.spacing;
		nameText.move(xof, 0);

		nameText.setColor(textColor);
		textRenderTexture.draw(nameText);
		nameText.setColor(pressedTextColor);
		pressedTextRenderTexture.draw(nameText);

		rect = nameText.getLocalBounds();
	}

	nameText.setString(std::to_string(noteOctave));
	auto thisRect = nameText.getLocalBounds();
	nameText.setPosition(-thisRect.left, -thisRect.top);
	nameText.move(xof + rect.width + Constants.text.spacing, 0);

	nameText.setColor(textColor);
	textRenderTexture.draw(nameText);
	nameText.setColor(pressedTextColor);
	pressedTextRenderTexture.draw(nameText);

#if 0
	nameText.setString(noteLetter);
	nameText.setColor(textColor);
	nameText.setCharacterSize(Constants.text.size);
	auto bigRect = nameText.getLocalBounds();
	nameText.setPosition(-bigRect.left, -bigRect.top);
	textRenderTexture.draw(nameText);

	auto bigWidth = bigRect.width - bigRect.left;
	auto bigHeight = bigRect.height - bigRect.top;

	nameText.setCharacterSize(Constants.text.smallSize);

	nameText.setString(noteAccidental);
	auto accidentalRect = nameText.getLocalBounds();
	nameText.setPosition(
			-accidentalRect.left + bigWidth + Constants.text.xPadding,
			-accidentalRect.top);
	textRenderTexture.draw(nameText);

	nameText.setString(std::to_string(noteOctave));
	auto octaveRect = nameText.getLocalBounds();
	auto octaveHeight = octaveRect.height;
	nameText.setPosition(
			-octaveRect.left + bigWidth + Constants.text.xPadding,
			-octaveRect.top + bigHeight - octaveHeight);
	textRenderTexture.draw(nameText);
#endif

	textRenderTexture.display();

	textSprite.setTexture(textRenderTexture.getTexture());
	pressedTextSprite.setTexture(pressedTextRenderTexture.getTexture());
}

void Note::SetPosition(int x, int y)
{
	rectangleShape.setPosition(x, y);
	lineShape.setPosition(x + Constants.rectangle.size/2, Constants.padding);
}

void Note::SetHue(int h)
{
	int saturation = Constants.rectangle.saturation;
	int pressedSaturation = Constants.rectangle.pressedSaturation;
	int value = Constants.rectangle.value;
	int outlineValue = Constants.rectangle.outlineValue;
	int textSaturation = Constants.text.saturation;
	int textValue = Constants.text.value;
	int textPressedSaturation = Constants.text.pressedSaturation;
	int textPressedValue = Constants.text.pressedValue;

	color               = conv::HSVtoRGB(h, saturation,        value);
	outlineColor        = conv::HSVtoRGB(h, saturation,        outlineValue);
	pressedColor        = conv::HSVtoRGB(h, pressedSaturation, value);
	pressedOutlineColor = conv::HSVtoRGB(h, pressedSaturation, outlineValue);
	textColor           = conv::HSVtoRGB(h, textSaturation,    textValue);
	pressedTextColor    = conv::HSVtoRGB(h, textPressedSaturation, textPressedValue);
}

void Note::SetNoteName(conv::Name noteName, int octave)
{
	baseFrequency = conv::NoteNameToFreq(noteName, octave);
	generateSamples();

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
	createTextures();
}

void Note::Draw(sf::RenderWindow *window)
{
	window->draw(lineShape);

	if (keyPressed) {
		rectangleShape.setFillColor(pressedColor);
		rectangleShape.setOutlineColor(pressedOutlineColor);
	} else {
		rectangleShape.setFillColor(color);
		rectangleShape.setOutlineColor(outlineColor);
	}

	window->draw(rectangleShape);

	if (keyPressed) {
		pressedTextSprite.setPosition(
				rectangleShape.getPosition().x +
				rectangleShape.getLocalBounds().width/2 -
				pressedTextSprite.getLocalBounds().width/2,
				rectangleShape.getPosition().y +
				rectangleShape.getLocalBounds().height/2 -
				pressedTextSprite.getLocalBounds().height/2);
		window->draw(pressedTextSprite);
	} else {
		textSprite.setPosition(
				rectangleShape.getPosition().x +
				rectangleShape.getLocalBounds().width/2 -
				textSprite.getLocalBounds().width/2,
				rectangleShape.getPosition().y +
				rectangleShape.getLocalBounds().height/2 -
				textSprite.getLocalBounds().height/2);
		window->draw(textSprite);
	}
}

void Note::Update()
{
}

void Note::KeyPressed()
{
	playSound.play();
}

void Note::KeyReleased()
{
	playSound.stop();
}

