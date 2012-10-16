/* Playback.cpp
 *
 * Written by Jack Cole,
 * Modified by Mark Mahony and Alan Cranbury, October 5th,2012
 *
 * Functions for playback of audio data, to be adapted for S2, Project 2. 
 * This version creates buffers for all notes from C0 to D#8.
 *
 * A testing main() is included that can be used in the following ways:
 * playback #1 #2  : play a half second of note number #1 then #2 etc (up to 10), 1 - NFREQUENCIES
 * playback A-Ga-g : play a half second of the notes A-G or a-g (up to 10)
 * playback r : record 6 seconds and then play it back. 
 *
 * Future: Follow the note with a duration as in ABC notation. Same with recording time.  Let note 0 or Z mean silence. 
 */

#define TESTING

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#include "console.h"
#include "playback.h"

#define	DEFAULT_NSAMPLES	4000	// 8000 is one second
#define MIN_BUFSIZE			1000

static float fFrequencies[] = {
/* C0 */  16.35, 17.32, 18.35, 19.45, 20.6,  21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87,			// 0 - 11
/* C1 */  32.7,  34.65, 36.71, 38.89, 41.2,  43.65, 46.25, 49.0, 51.91, 55.0, 58.27, 61.74,			// 12-23
/* C2 */  65.41, 69.3,  73.42, 77.78, 82.41, 87.31, 92.5,  98.0,103.83,110.0,116.54,123.47,			// 24-35
/* C3 */ 130.81,138.59,146.83,155.56,164.81,174.61,185.0, 196.0,207.65,220.0,233.08,246.94,			// 36-47
/* C4 */ 261.63,277.18,293.66,311.13,329.63,349.23,369.99,392.0,415.30,440.0,466.16,493.88,			// 48-59
/* C5 */ 523.25,554.37,587.33,622.25,659.26,698.46,739.99,783.99,830.6,880.0,932.33,987.77,			// 60-71
/* C6 */1046.5,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760.0,1864.66,1975.53,
/* C7 */2093,2217.46,2349.32,2489.02,2637.02,2793.83,2959.96,3135.96,3322.44,3520.0,3729.31,3951.07
};

#define	NFREQUENCIES (sizeof(fFrequencies)/sizeof(float))
#define RECORD_TIME		6		// seconds
#define SAMPLES_SEC		8000	// samples per second

// Buffers for the notes, plus 1 big buffer for recording.
static short *pFreqBufs[NFREQUENCIES] = {NULL};
static short nSamples[NFREQUENCIES] = { 0 };

// output and input channel parameters
static	int		g_nSamplesPerSec = SAMPLES_SEC;
static	int		g_nBitsPerSample = 16;
static	HWAVEOUT	HWaveOut;				/* Handle of opened WAVE Out and In device */
static  HWAVEIN		HWaveIn;
static	WAVEFORMATEX WaveFormat;			/* WAVEFORMATEX structure for reading in the WAVE fmt chunk */
static  WAVEHDR	WaveHeader[NFREQUENCIES];	/* WAVEHDR structures - 1 per buffer */
static  WAVEHDR	WaveHeaderSilence;
static  WAVEHDR WaveHeaderIn;

static int WaitOnHeader( WAVEHDR *wh, char cDit );

/* SetupFormat() initializes a WAVEFORMATEX strucutre to the required 
 *				 parameters (sample rate, bits per sample, etc)
 */
static void SetupFormat( WAVEFORMATEX *wf )
{	
	wf->wFormatTag = WAVE_FORMAT_PCM;
	wf->nChannels = 1;
	wf->nSamplesPerSec = g_nSamplesPerSec;
	wf->wBitsPerSample = g_nBitsPerSample;
	wf->nBlockAlign = wf->nChannels * (wf->wBitsPerSample/8);
	wf->nAvgBytesPerSec = wf->nSamplesPerSec * wf->nBlockAlign;
	wf->cbSize = 0;
	return;
}

/* InitializePlayback()
 */

int	InitializePlayback(void)
{
	int		rc;

	// set up the format structure, needed for playback (and recording)
	SetupFormat( &WaveFormat );

	// open the playback device
	rc = waveOutOpen(&HWaveOut, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0, CALLBACK_NULL);
	if ( rc ) {
		Errorp("Unable to open Output sound Device! Error %x.", rc);
		return(0);
	}

	return(1);
}

/* PlayBuffer()
 */
int PlayBuffer( short *piBuf, long lSamples )
{
	static	WAVEFORMATEX WaveFormat;	/* WAVEFORMATEX structure for reading in the WAVE fmt chunk */
	static  WAVEHDR	WaveHeader;			/* WAVEHDR structure for this buffer */
	MMRESULT	mmErr;
	int		rc;

	// stop previous note (just in case)
	waveOutReset(HWaveOut);   // is this good?

	// get the header ready for playback
	WaveHeader.lpData = (char *)piBuf;
	WaveHeader.dwBufferLength = lSamples*sizeof(short);
	rc = waveOutPrepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
	if ( rc ) {
		Errorp( "Failed preparing WAVEHDR, error 0x%x.", rc);
		return(0);
	}
	WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP|WHDR_ENDLOOP);

	// play the buffer. This is NON-blocking.
	mmErr = waveOutWrite(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
	if ( mmErr != MMSYSERR_NOERROR ) {
		char	szErrBuf[150];
		waveOutGetErrorText(mmErr, szErrBuf, sizeof(szErrBuf) );
		Errorp("Error 0x%x writing to playback device. %s", mmErr, szErrBuf );
		return(0);
	}

	// wait for completion
	rc = WaitOnHeader(&WaveHeader, 0);

	// give back resources
	waveOutUnprepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));

	return(rc);
}

static int WaitOnHeader( WAVEHDR *wh, char cDit )
{
	long	lTime = 0;

	// wait for whatever is being played, to finish. Quit after 10 seconds.
	for ( ; ; ) {
		if ( wh->dwFlags & WHDR_DONE ) return(1);
		// idle for a bit so as to free CPU
		Sleep(100L);
		lTime += 100; 
		if ( lTime >= 10000 ) return(0);  // timeout
		if ( cDit ) printf("%c", cDit);
	}
}

void ClosePlayback(void)
{
	int		i;

	for ( i=0; i < NFREQUENCIES; ++i ) {
		waveOutUnprepareHeader(HWaveOut, &WaveHeader[i], sizeof(WAVEHDR));
	}

	// close the playback device
	waveOutClose(HWaveOut);
	
	return;
}

/* RECORDING ********************************************************************/

int InitializeRecording(short *iBigBuf, long lBigBufSize)
{
	MMRESULT rc;

	// set up the format structure, needed for recording.
	SetupFormat( &WaveFormat );

	// open the recording device
	rc = waveInOpen(&HWaveIn, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0, CALLBACK_NULL);
	if ( rc ) {
		Errorp("Unable to open Input sound Device! Error %x.", rc);
		return(0);
	}

	// prepare the buffer header for use later on
	WaveHeaderIn.lpData = (char *)iBigBuf;
	WaveHeaderIn.dwBufferLength = lBigBufSize*sizeof(short);
	rc = waveInPrepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
	if ( rc ) {
		Errorp( "Failed preparing input WAVEHDR, error 0x%x.", rc);
		return(0);
	}

	return(1);

}

/* RecordBuffer() : FiIl in the buffer from input device. 
 * Returns 0 for failure. 
 */
int	RecordBuffer( short *piBuf, long lBufSize )
{
	static	WAVEFORMATEX WaveFormat;	/* WAVEFORMATEX structure for reading in the WAVE fmt chunk */
	static  WAVEHDR	WaveHeader;			/* WAVEHDR structure for this buffer */
	MMRESULT	mmErr;
	int		rc;

	printf("Recording now....." );

	// stop previous recording (just in case)
	waveInReset(HWaveIn);   // is this good?

	// get the header ready for recording.  This should not be needed here AND in init.
	WaveHeader.lpData = (char *)piBuf;
	WaveHeader.dwBufferLength = lBufSize*sizeof(short);
	rc = waveInPrepareHeader(HWaveIn, &WaveHeader, sizeof(WAVEHDR));
	if ( rc ) {
		Errorp( "Failed preparing WAVEHDR, error 0x%x.", rc);
		return(0);
	}
	WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP|WHDR_ENDLOOP);

	// Give the buffer to the recording device to fill.
	mmErr = waveInAddBuffer(HWaveIn, &WaveHeader, sizeof(WAVEHDR) ); 
	if ( mmErr != MMSYSERR_NOERROR ) {
		char	szErrBuf[150];
		waveOutGetErrorText(mmErr, szErrBuf, sizeof(szErrBuf) );
		Errorp("Error 0x%x writing to playback device. %s", mmErr, szErrBuf );
		return(0);
	}

	// Record the buffer. This is NON-blocking.
	mmErr = waveInStart(HWaveIn); 
	if ( mmErr != MMSYSERR_NOERROR ) {
		char	szErrBuf[150];
		waveOutGetErrorText(mmErr, szErrBuf, sizeof(szErrBuf) );
		Errorp("Error 0x%x writing to playback device. %s", mmErr, szErrBuf );
		return(0);
	}

	// wait for completion
	rc = WaitOnHeader(&WaveHeader, '.');

	// probably not necessary
	waveInStop(HWaveIn);

	return(rc);
}

void CloseRecording(void)
{
	waveInUnprepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));

	// close the playback device
	waveInClose(HWaveIn);
	
	return;
}


#ifdef TESTING  // a testing mainline

int	main( int argc, char *argv[] )
{
	int printUsage = 0;

	// Check command line arguments for sample rate and bits per sample
	if(argc > 2){
		if(toupper(*argv[1]) == 'S')
			g_nSamplesPerSec = atoi(argv[2]);
		else if(toupper(*argv[1]) == 'B')
			g_nBitsPerSample = atoi(argv[2]);
		else printUsage = 1;
	}
	if(argc > 4){
		if(toupper(*argv[3]) == 'S')
			g_nSamplesPerSec = atoi(argv[4]);
		else if(toupper(*argv[3]) == 'B')
			g_nBitsPerSample = atoi(argv[4]);
		else printUsage = 1;
	}

	if(printUsage == 1){
		printf("\nUsage: playback.exe [Options] [Value]\n\n [Options]\n\n s - sample rate \n b - bits per sample\n");
		exit(0);
	}
	printf(" %d %d", g_nSamplesPerSec, g_nBitsPerSample);
	
	static short *iBigBuf = (short *)malloc(sizeof(short) * g_nSamplesPerSec * RECORD_TIME);		
	static long	 lBigBufSize = g_nSamplesPerSec * RECORD_TIME;	// in samples

	InitializePlayback();

	
	// Record the special buffer
	InitializeRecording(iBigBuf, lBigBufSize);
	RecordBuffer(iBigBuf, lBigBufSize );
	CloseRecording();
	// play the special buffer
	printf("Playing special %d..\n", 60);
	PlayBuffer( iBigBuf, lBigBufSize );	

	ClosePlayback();
	free(iBigBuf);
	return(0);
}

#endif

	
