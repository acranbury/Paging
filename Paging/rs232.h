#ifndef RS232_H
#define RS232_H

#include <Windows.h>

#define COMPORT4 "\\\\.\\COM4"
#define COMPORT3 "\\\\.\\COM3"
#define BUFSIZE     140

int OpenRS232Port();
int WriteToRS232(BYTE * writeBuf, DWORD *bufSize);
int SetUpDCB(int baudRate);
void ReadFromRS232(BYTE * readBuf, DWORD *numBytesTransferred);
void CloseRS232Port();

extern HANDLE hComm;
extern DCB portDCB;
static COMMTIMEOUTS timeouts;

#endif // RS232_H
