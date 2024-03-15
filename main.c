#include <portaudio.h>
#include <stdio.h>
#include <math.h>

#define NUM_SECONDS 5
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64

#ifndef M_PI
#define M_PI (3.14159265)
#endif

#define TABLE_SIZE 200

typedef struct {
	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
	char messege[20];
}
PaTestData;
static int Callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
static void StreamFinished(void*);

int main(){
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	PaTestData data;
	int i;

	for (i = 0; i < TABLE_SIZE; i++){
		data.sine[i] = (float)sin(i);
	}
	data.left_phase = data.right_phase = 0;

	err = Pa_Initialize();
	if (err<0) {printf("init\n");goto error;}
	//err = outputParameters.device = Pa_GetDefaultOutputDevice();
	
	err = outputParameters.device = 1;
	if (err<0) {printf("init\n");goto error;}

	outputParameters.channelCount = 2;
	outputParameters.sampleFormat = paFloat32;
	err = outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device) -> defaultLowOutputLatency;
	if (err<0) {printf("init\n");goto error;}

	outputParameters.hostApiSpecificStreamInfo = NULL;


	err = Pa_OpenStream(&stream,
			    NULL,
			    &outputParameters,
			    SAMPLE_RATE,
			    FRAMES_PER_BUFFER,
			    paClipOff,
			    Callback,
			    &data);
	if (err<0) {printf("open\n");goto error;}
	err = Pa_SetStreamFinishedCallback(stream, StreamFinished);
	if (err<0) {printf("setFin\n");goto error;}
	err = Pa_StartStream(stream);
	if (err<0) {printf("start\n");goto error;}
	err = Pa_StopStream(stream);
	if (err<0) {printf("stop\n");goto error;}
	err = Pa_CloseStream(stream);
	if (err<0) {printf("close\n");goto error;}
	Pa_Terminate();
	return 0;
error:
	printf("Fuck\n");
	printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
	return 0;
}

static int
Callback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	PaTestData *data = (PaTestData*)userData;
	float *out = (float*)outputBuffer;
	unsigned long i;


	//the followings 3 strings are needet to prevent unused variable warning
	(void) timeInfo;
	(void) statusFlags;
	(void) inputBuffer;
	for (i = 0; i < framesPerBuffer; i++)
	{
		*out++ = data->sine[data->left_phase];
		*out++ = data->sine[data->right_phase];

		data->left_phase+=1;
		if (data->left_phase >= TABLE_SIZE) data->left_phase -= TABLE_SIZE;
		
		data->right_phase +=1;
		if (data->right_phase >= TABLE_SIZE) data->right_phase -= TABLE_SIZE;
	}
	return paContinue;	
}


static void
StreamFinished(void *userData)
{
	printf("Fuck you\n");
}
