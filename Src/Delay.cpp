
#include <memory.h>

#include "Delay.h"


Delay::Delay() : mBuffer(0), mBufferLength(0), mPos(0), mGain(1.f), mFeedback(0.f), mDry(0.f), mWet(0.f)
{
}

// delayTime is in seconds
Delay::Delay(const float delayTime, const float gain, const float feedback, const double sampleRate,
			const int nbChannels, const float dryAmount, const float wetAmount)
: mBuffer(0), mBufferLength(0), mPos(0), mGain(gain), mFeedback(feedback), mDry(dryAmount), mWet(wetAmount)
{
	mBufferLength = static_cast<int>(delayTime * sampleRate) * nbChannels;
	int bufferSize = mBufferLength * sizeof(float);

	mBuffer = new float[mBufferLength];

	memset(mBuffer, 0, bufferSize);
}

Delay::~Delay()
{
	delete[] mBuffer;
}
