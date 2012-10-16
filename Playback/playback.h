/* playback.h
 */

int InitializeRecording(void);
int InitializePlayback(void);
int InitializeNotes(void);
int PlayNote(int iNote, int iDuration );
void ClosePlayback(void);
int WaitForNotePlayback(int iNote);
int PlayBuffer( short *piBuf, long lSamples );

