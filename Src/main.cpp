
#include <iostream>
#include <stdio.h>


#include "Delay.h"
#include "portAudioInterface.h"

// delay parameters
#define DELAY_TIME 		(0.42f)
#define DELAY_GAIN		(0.85f)
#define DELAY_FEEDBACK	(0.42f)
#define DELAY_DRY		(0.8f)
#define DELAY_WET		(0.55f)

#define INPUT_CHANNELS 	(2)
#define OUTPUT_CHANNELS (2)

int main(int argc, const char *argv[])
{
	std::cout << "Main" << std::endl;

	Delay delay(DELAY_TIME, DELAY_GAIN, DELAY_FEEDBACK, SAMPLING_RATE, OUTPUT_CHANNELS, DELAY_DRY, DELAY_WET);

	if(PaInterface::Instance()->initialize(INPUT_CHANNELS, OUTPUT_CHANNELS, &delay))
	{
		PaInterface::Instance()->startStream();

		std::cout << "Enter 'q' to quit: ";
		while(getchar() != 'q')
			Pa_Sleep(100);

		PaInterface::Instance()->stopStream();
		PaInterface::Instance()->terminate();
	}
	
	return 0;	
}
