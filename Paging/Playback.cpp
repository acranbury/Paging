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

#define NOTTESTING

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#include "playback.h"
#include "senderwindow.h"
#include <QMessageBox>


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
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Unable to open Output sound Device! Error %1.").arg(QString::number(rc)));
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
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Failed preparing WAVEHDR, error 0x%1.").arg(QString::number(rc)));
		return(0);
	}
	WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP|WHDR_ENDLOOP);

	// play the buffer. This is NON-blocking.
	mmErr = waveOutWrite(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
	if ( mmErr != MMSYSERR_NOERROR ) {
		char	szErrBuf[150];
        waveOutGetErrorText(mmErr, (LPWSTR)szErrBuf, sizeof(szErrBuf) );
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Error 0x%1 writing to playback device. %2").arg(QString::number(mmErr), QString(szErrBuf)));
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

    for ( i=0; i < NUMHEADERS; ++i ) {
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
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Unable to open Input sound Device! Error %1.").arg(QString::number(rc)));
		return(0);
	}

	// prepare the buffer header for use later on
	WaveHeaderIn.lpData = (char *)iBigBuf;
	WaveHeaderIn.dwBufferLength = lBigBufSize*sizeof(short);
	rc = waveInPrepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
	if ( rc ) {
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Failed preparing input WAVEHDR, error 0x%1.").arg(QString::number(rc)));
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

    //printf("Recording now....." );

	// stop previous recording (just in case)
	waveInReset(HWaveIn);   // is this good?

	// get the header ready for recording.  This should not be needed here AND in init.
	WaveHeader.lpData = (char *)piBuf;
	WaveHeader.dwBufferLength = lBufSize*sizeof(short);
	rc = waveInPrepareHeader(HWaveIn, &WaveHeader, sizeof(WAVEHDR));
	if ( rc ) {
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Failed preparing WAVEHDR, error 0x%1.").arg(QString::number(rc)));
		return(0);
	}
	WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP|WHDR_ENDLOOP);

	// Give the buffer to the recording device to fill.
	mmErr = waveInAddBuffer(HWaveIn, &WaveHeader, sizeof(WAVEHDR) ); 
	if ( mmErr != MMSYSERR_NOERROR ) {
		char	szErrBuf[150];
        waveOutGetErrorText(mmErr, (LPWSTR)szErrBuf, sizeof(szErrBuf) );
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Error 0x%1 writing to playback device. %2").arg(QString::number(mmErr), QString(szErrBuf)));
		return(0);
	}

	// Record the buffer. This is NON-blocking.
	mmErr = waveInStart(HWaveIn); 
	if ( mmErr != MMSYSERR_NOERROR ) {
		char	szErrBuf[150];
        waveOutGetErrorText(mmErr, (LPWSTR)szErrBuf, sizeof(szErrBuf) );
        QMessageBox::information(NULL,
                                 "Error!",
                                 QString("Error 0x%1 writing to playback device. %2").arg(QString::number(mmErr), QString(szErrBuf)));
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

	
