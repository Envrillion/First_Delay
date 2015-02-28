
#include "portAudioInterface.h"
#include "Delay.h"


// Callback
int audioCallback(const void* input, void* output, unsigned long samples, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	const float *in = (const float*)input;
	float *out = (float*)output;
	Delay *delay = (Delay*)userData;
	float delaySample;

	for(unsigned int i = 0; i < samples; ++i)
	{
		// left channel
		delaySample = delay->read();
		delay->write(*in);
		*out++ = delay->processSample(*in++, delaySample);

		// right channel
		delaySample = delay->read();
		delay->write(*in);
		*out++ = delay->processSample(*in++, delaySample);
	}

	return paContinue;
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
		errorHandler(err);
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
	PaDeviceIndex inputDevice = Pa_GetDefaultInputDevice();
	if(inputDevice == paNoDevice)
	{
		std::cerr << "Error: no default input device" << std::endl;
		return false;
	}

	const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(inputDevice);
	const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);

	std::cout << "Opening audio input device: " << hostApiInfo->name << " " << deviceInfo->name << std::endl;

	streamParams.device = inputDevice;
	streamParams.channelCount = nbChannels;
	streamParams.sampleFormat = paFloat32;
	streamParams.suggestedLatency = deviceInfo->defaultLowInputLatency;
	streamParams.hostApiSpecificStreamInfo = NULL;

	return true;
}

bool PaInterface::getOutputDevice(PaStreamParameters& streamParams, const int nbChannels)
{
	PaDeviceIndex outputDevice = Pa_GetDefaultOutputDevice();
	if(outputDevice == paNoDevice)
	{
		std::cout << "Error: no default output device" << std::endl;
		return false;
	}

	const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(outputDevice);
	const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);

	std::cout << "Opening audio output device: " << hostApiInfo->name << " " << deviceInfo->name << std::endl;

	streamParams.device = outputDevice;
	streamParams.channelCount = nbChannels;
	streamParams.sampleFormat = paFloat32;
	streamParams.suggestedLatency = deviceInfo->defaultLowOutputLatency;
	streamParams.hostApiSpecificStreamInfo = NULL;

	return true;
}


