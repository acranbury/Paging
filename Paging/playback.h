/* playback.h
 */

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>


//static float fFrequencies[] = {
/* C0 */  //16.35, 17.32, 18.35, 19.45, 20.6,  21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87,			// 0 - 11
/* C1 */ // 32.7,  34.65, 36.71, 38.89, 41.2,  43.65, 46.25, 49.0, 51.91, 55.0, 58.27, 61.74,			// 12-23
/* C2 */  //65.41, 69.3,  73.42, 77.78, 82.41, 87.31, 92.5,  98.0,103.83,110.0,116.54,123.47,			// 24-35
/* C3 */ //130.81,138.59,146.83,155.56,164.81,174.61,185.0, 196.0,207.65,220.0,233.08,246.94,			// 36-47
/* C4 */ //261.63,277.18,293.66,311.13,329.63,349.23,369.99,392.0,415.30,440.0,466.16,493.88,			// 48-59
/* C5 */ //523.25,554.37,587.33,622.25,659.26,698.46,739.99,783.99,830.6,880.0,932.33,987.77,			// 60-71
/* C6 */ //1046.5,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760.0,1864.66,1975.53,
/* C7 */ //2093,2217.46,2349.32,2489.02,2637.02,2793.83,2959.96,3135.96,3322.44,3520.0,3729.31,3951.07
//};

#define MIN_BUFSIZE			1000
//#define	NFREQUENCIES (sizeof(fFrequencies)/sizeof(float))
#define RECORD_TIME		10		// seconds
#define SAMPLES_SEC		8000	// samples per second
#define BITSSAMPLE      16
#define NUMHEADERS      1

// Buffers for the notes, plus 1 big buffer for recording.
//static short *pFreqBufs[NFREQUENCIES] = {NULL};
//static short nSamples[NFREQUENCIES] = { 0 };

// output and input channel parameters
static	int		g_nSamplesPerSec = SAMPLES_SEC;
static	int		g_nBitsPerSample = BITSSAMPLE;
static	HWAVEOUT	HWaveOut;				/* Handle of opened WAVE Out and In device */
static  HWAVEIN		HWaveIn;
static	WAVEFORMATEX WaveFormat;			/* WAVEFORMATEX structure for reading in the WAVE fmt chunk */
static  WAVEHDR	WaveHeader[NUMHEADERS];	/* WAVEHDR structures - 1 per buffer */
static  WAVEHDR	WaveHeaderSilence;
static  WAVEHDR WaveHeaderIn;

static int WaitOnHeader( WAVEHDR *wh, char cDit );

int InitializeRecording(short *iBigBuf, long lBigBufSize);
int InitializePlayback(void);
int InitializeNotes(void);
int	RecordBuffer( short *piBuf, long lBufSize );
int PlayNote(int iNote, int iDuration );
void ClosePlayback(void);
int WaitForNotePlayback(int iNote);
int PlayBuffer( short *piBuf, long lSamples );
void CloseRecording(void);

#endif
