#ifndef __PA_INTERFACE_H__
#define __PA_INTERFACE_H__

#include <iostream>
#include <cmath>

#include "portaudio.h"

#define SAMPLE_BLOCK	(256)
#define SAMPLING_RATE	(44100.)

class PaInterface
{
public:
	
	static PaInterface* Instance();

	bool initialize(const int inChannels, const int outChannels, void* userData);
	bool terminate();
	
	bool startStream();
	bool stopStream();	

private:

	static PaInterface *mInstance;
	PaStream *mAudioStream;

	PaInterface();

	void errorHandler(PaError errorCode);

	bool getInputDevice(PaStreamParameters& streamParams, const int nbChannels);
	bool getOutputDevice(PaStreamParameters& streamParams, const int nbChannels);

};


#endif // __PA_INTERFACE_H__
