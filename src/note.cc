#include "constants.hh"
#include "note.hh"

Note::Note()
{
	sound.setBuffer(soundBuffer);
}

void Note::GenerateSamples()
{
	sf::Int16 samples[Constants.maxSamples];
	const double omega = 2*M_PI*baseFrequency;
	unsigned long int i = 0;
	const double secondsPerSample = 1.0 / Constants.samplesPerSecond;
	double t = 0;
	while (i < Constants.maxSamples) {
		const double value = Globals.volume*
			Globals.tempScriptPtr->GetValue(omega, t);
		samples[i] = value;
		// if (abs(value) < Globals.volume/1000.0)
		// 	break;
		t += secondsPerSample;
		i++;
	}

	if (!soundBuffer.loadFromSamples(samples, i, Constants.channels,
				Constants.samplesPerSecond)) {
		puts("Failed to copy sound buffer");
		throw;
	}
}

