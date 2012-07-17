#include "SineWave.h"
#include "RtWvOut.h"
#include <stdio.h>
#include <signal.h>
#include <mapper/mapper.h>

using namespace stk;

int done = 0;

void ctrlc(int)
{
	done = 1;
}

RtWvOut *dac = 0;
SineWave *c;


int nFrames = 100000;

void handler_freq(mapper_signal msig,
		mapper_db_signal props,
		mapper_timetag_t *timetag,
		void *value)
{
	float *pfreq = (float*)value;
	c->setFrequency(*pfreq);
}

int main()
{

	// Set the global sample rate before creating class instances.
	Stk::setSampleRate( 44100.0 );
	try {
		// Define and open the default realtime output device for one-channel playback
		dac = new RtWvOut( 1 );
	}
	catch ( StkError &e ) {
		printf("Error: ");
		e.printMessage();
		exit( 1 );
	}
	printf("Got here \n");
	c = new SineWave;
	signal(SIGINT, ctrlc);

	mapper_device dev = mdev_new("Clar", 9000, 0);
    
	printf("Got 2 here \n");
	
    float min0 = 0;
	float max1 = 1;
	float max1000 = 1000;

	mdev_add_input(dev, "/freq", 1, 'f', "Hz", &min0, &max1000,
			handler_freq, 0);
    
	printf("Got 3 here \n");

	c->setFrequency(440.0);


	while (!done)
	{
		mdev_poll(dev, 0);

		// Option 2: Single-sample computations
		for ( int i=0; i<nFrames; i++ ) {
			printf("got to 4 \n");
			dac->tick( c->tick() );
		   }
	}          
	mdev_free(dev);

cleanup:
	delete dac;	

	return 0;
}
