
#ifndef __DELAY_H__
#define __DELAY_H_

class Delay
{
public:
	Delay(const float delayTime, const float gain, const float feedback, const double sampleRate,
			const int nbChannels, const float dryAmount, const float wetAmount);
	~Delay();

	inline float read() const
	{
		return mBuffer[mPos] * mGain;
	}

	inline void write(const float value)
	{
		mBuffer[mPos] = value + (mBuffer[mPos] * mFeedback);
		incr();
	}

	inline float processSample(const float drySample, const float wetSample) const
	{
		return drySample * mDry + wetSample * mWet;
	}

private:
	float 	*mBuffer;
	int 	mBufferLength,
			mPos;
	float 	mGain,
			mFeedback,
			mDry,
			mWet;

	Delay();

	inline void incr()
	{
		mPos = (mPos < mBufferLength ? mPos + 1 : 0);
	}

};

#endif // __DELAY_H__
