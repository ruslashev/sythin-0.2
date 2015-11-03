#include "constants.hh"
#include "note.hh"
#include "conv.hh"

Note::Note()
{
}

Note::Note(note::Name nName, int nOctave)
{
	name = nName;
	switch (name) {
		case note::A: case note::As:
			letter = 'A'; break;
		case note::B:
			letter = 'B'; break;
		case note::C: case note::Cs:
			letter = 'C'; break;
		case note::D: case note::Ds:
			letter = 'D'; break;
		case note::E:
			letter = 'E'; break;
		case note::F: case note::Fs:
			letter = 'F'; break;
		case note::G: case note::Gs:
			letter = 'G'; break;
	}
	switch (name) {
		case note::As:
		case note::Cs:
		case note::Ds:
		case note::Fs:
		case note::Gs:
			accidental = '#';
			break;
		default:
			accidental = ' ';
			break;
	}
	octave = nOctave;
}

void Note::GenerateSamples(Script *script)
{
	sf::Int16 samples[Constants.maxSamples];
	const double baseFrequency = conv::NoteNameToFreq(name, octave);
	const double omega = 2*M_PI*baseFrequency;
	unsigned long long int i = 0;
	const double secondsPerSample = 1.0 / Constants.samplesPerSecond;
	double t = 0;
	while (i < Constants.maxSamples) {
		const double value = Globals.volume*
			script->GetValue(omega, t);
		samples[i] = value;
		t += secondsPerSample;
		i++;
	}

	if (!soundBuffer.loadFromSamples(samples, i, Constants.channels,
				Constants.samplesPerSecond)) {
		puts("Failed to copy sound buffer");
		throw;
	}

	sound.setBuffer(soundBuffer);
}

