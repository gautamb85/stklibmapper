#include "SineWave.h"
#include "RtAudio.h"
#include <stdio.h>
#include <signal.h>
#include <mapper/mapper.h>
#include <unistd.h>
using namespace stk;


int done = 0;

void ctrlc(int)
{
	done = 1;
}
SineWave *c;

void handler_freq(mapper_signal msig,
		mapper_db_signal props,
		mapper_timetag_t *timetag,
		void *value)
{
	float *pfreq = (float*)value;
	c->setFrequency(*pfreq);
}
// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
  SineWave *c = (SineWave *) dataPointer;
  register StkFloat *samples = (StkFloat *) outputBuffer;

  for ( unsigned int i=0; i<nBufferFrames; i++ )
    *samples++ = c->tick();

  return 0;
}


int main()
{
  // Set the global sample rate before creating class instances.
  Stk::setSampleRate( 44100.0 );

  c = new SineWave;
  RtAudio dac;
  
    printf("got to here\n");	
  // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferFrames = RT_BUFFER_SIZE;

	signal(SIGINT, ctrlc);

    //declare new input device
	mapper_device dev = mdev_new("Clar", 9000, 0);
 
	float min0 = 0.0;
	float max1 = 1.0;
	float max1000 = 1000.0;
    
    //add input for frequency which calls on the freq handler function
	mdev_add_input(dev, "/freq", 1, 'f', "Hz", &min0, &max1000,
			handler_freq, 0);

    printf("got to her 2e\n");	
	
	dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&c );
	
    c->setFrequency(440);
    //c->startBlowing(0.9);	
    dac.startStream();

//	while (!done)
    
	mdev_poll(dev, 0);

    

  // Shut down the output stream.
	//	dac->closeStream();


  return 0;
}
