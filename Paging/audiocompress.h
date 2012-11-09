#ifndef AUDIOCOMPRESS_H
#define AUDIOCOMPRESS_H

char * DifferentialCompress(short * audio, long length );
short * DifferentialExpand(char *buffer, long length);
char * RunLengthEncode(char * buffer, long length);
char * RunLengthDecode(char * buffer, long length);

#endif // AUDIOCOMPRESS_H
