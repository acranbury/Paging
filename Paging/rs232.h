#ifndef RS232_H
#define RS232_H

#include <Windows.h>

#define COMPORT4 "COM4"
#define COMPORT3 "COM3"

int OpenRS232Port();
int WriteToRS232(short * writeBuf, long *bufSize);
int SetUpDCB();
void ReadFromRS232(BYTE * readBuf, long *bufSize);
void CloseRS232Port();

static HANDLE hComm;
static DCB portDCB;

#endif // RS232_H
