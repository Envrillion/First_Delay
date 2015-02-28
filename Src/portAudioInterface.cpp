
#include "portAudioInterface.h"
#include "Delay.h"


// Callback
int audioCallback(const void* input, void* output, unsigned long samples, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
//Todo

	return 0;
}

// PortAudio interface
PaInterface* PaInterface::mInstance = 0;

PaInterface* PaInterface::Instance()
{
	if(mInstance == 0)
	{
		mInstance = new PaInterface;
	}

	return mInstance;
}

PaInterface::PaInterface() : mAudioStream(0)
{
}

bool PaInterface::initialize(const int inChannels, const int outChannels, void* userData)
{
	PaStreamParameters inputParams, outputParams;
	PaError err;

	std::cout << "Initializing PortAudio.\n\n";
	err = Pa_Initialize();

	if(err != paNoError)
	{
		std::cerr << "Portaudio failed to initialize. Error: " << err << std::endl;
		std::cerr << Pa_GetErrorText(err) << std::endl;
		return false;
	}


	if(!getInputDevice(inputParams, inChannels))
	{
		return false;
	}

	if(!getOutputDevice(outputParams, outChannels))
	{
		return false;
	}


	err = Pa_OpenStream(&mAudioStream, &inputParams, &outputParams, SAMPLING_RATE, SAMPLE_BLOCK, paClipOff, audioCallback, userData);

	if(err != paNoError)
	{
		errorHandler(error);
		return false;
	}

	return true;
}

bool PaInterface::terminate()
{
	PaError err = Pa_Terminate();

	if(err != paNoError)
	{
		errorHandler(err);
		return false;
	}

	std::cout << "Terminating PortAudio" << std::endl;

	return true;
}

bool PaInterface::startStream()
{
	PaError err = Pa_StartStream(mAudioStream);

	if(err != paNoError)
	{
		errorHandler(err);
		return false;
	}

	std::cout << "\n Portaudio is running" << std::endl;

	return true;
}

bool PaInterface::stopStream()
{
	PaError err = Pa_StopStream(mAudioStream);

	if(err != paNoError)
	{
		errorHandler(err);
		return false;
	}


	err = Pa_CloseStream(mAudioStream);

	if(err != paNoError)
	{
		errorHandler(err);
		return false;
	}

	return true;
}

void PaInterface::errorHandler(PaError errorCode)
{
	std::cerr << "Portaudio error" << errorCode << std::endl;
	std::cerr << Pa_GetErrorText(errorCode) << std::endl << std::endl;

	Pa_Terminate();
}

bool PaInterface::getInputDevice(PaStreamParameters& streamParams, const int nbChannels)
{
//Todo

	return true;
}

bool PaInterface::getInputDevice(PaStreamParameters& streamParams, const int nbChannels)
{
//Todo

	return true;
}


