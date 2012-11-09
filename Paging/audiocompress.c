#include "audiocompress.h"

#include <stdlib.h>

#define ESC         0xFF
#define UPPERLIMIT  32767
#define LOWERLIMIT  -32768

// compresses the audio into differences
short * DifferentialCompress(short * audio, long length )
{
    long i;
    int leftover = 0, difference;

    short * buffer = (char *)calloc((length + 1), sizeof(short));

    for(i = length - 1; i > 0; i--)
    {
        difference = audio[i] - audio [i-1] + leftover;
        if(difference > UPPERLIMIT)
        {
            leftover = difference - UPPERLIMIT;
            buffer[i+1] = UPPERLIMIT;
        }else if(difference < LOWERLIMIT)
        {
            leftover = difference - LOWERLIMIT;
            buffer[i+1] = LOWERLIMIT;
        }
		else
			buffer[i+1] = difference;
    }

    // get the initial value into the buffer, in two parts
    buffer[0] = audio[0];
    return buffer;
}

// expands the differences into audio samples
short * DifferentialExpand(short *buffer, long length)
{
    long i;
    short * audio = (short *)calloc((length - 1), sizeof(short));

    audio[0] = buffer[0];

    for(i = 1; i < length; i++)
    {
        audio[i] = audio[i-1] + buffer[i];
    }

    return audio;
}

// encodes the buffer with run length encoding
char * RunLengthEncode(char * buffer, long length)
{
    long i, j;
    char ch;
    char count;

    char * out = (char *)calloc(length, sizeof(char));
    j = 0;
    ch = buffer[0];
    count = 1;

    for(i = 1; i < length; i++)
    {
        if(buffer[i] == ch)
        {
            count++;
        }
        else if(count > 2)
        {
            out[j++] = ESC;
            out[j++] = ch;
            out[j++] = count;
			ch = buffer[i];
			count = 1;
        }
        else
        {
            if(ch == ESC)
            {
                out[j++] = ESC;
                out[j++] = ch;
                out[j++] = 1;
            }
            else
			{
                while(count-- > 0)
				{
					out[j++] = ch;
				}
			}
			ch = buffer[i];
			count = 1;
        }
    }

	if(count > 1)
	{
		out[j++] = ESC;
        out[j++] = ch;
        out[j++] = count;
	}
	else if(count == 1)
		out[j] = ch;

    return out;
}

// decodes the buffer with run length encoding
char * RunLengthDecode(char * buffer, long length)
{
    long i, j;
    char ch;
    char count;

    char * out = (char *)calloc(length, sizeof(char));
    for(i = 0, j = 0; i < length; i++)
    {
        if(buffer[i] == ESC)
        {
            ch = buffer[i+1];
            count = buffer[i+2];
            while(count-- > 0)
            {
                out[j++] = ch;
            }
			i+=2;
        }
        else
        {
            out[j++] = buffer[i];
        }
    }
	return out;
}
