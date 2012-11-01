#include "checksum.h"

// calculate the Fletcher's checksum of a buffer
int CalculateChecksum(char *buffer, int bytes)
{
    int sum1 = 0xff;
    int sum2 = 0xff;

    while(bytes)
    {
        int tlen = bytes > 20 ? 20 : bytes;
        bytes -= tlen;
        do
        {
            sum2 += sum1 += *buffer++;
        }while(--tlen);
        sum1 = (sum1 & 0xff) + (sum1 >> 8);
        sum2 = (sum2 & 0xff) + (sum2 >> 8);
    }
    sum1 = (sum1 & 0xff) + (sum1 >> 8);
    sum2 = (sum2 & 0xff) + (sum2 >> 8);
    return sum2 << 8 | sum1;
}
