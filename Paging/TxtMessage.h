#ifndef TXTMESSAGE_H
#define TXTMESSAGE_H

#include <Windows.h>

#define LENGTH 140	// Length of message in buffer
#define UNKNOWN 25	// Unknown purpose
#define FORTUNETXT	"FortuneCookies.txt"
#define READMODE	"r"
#define NEWMSGCODE  "%%"
#define NUMMSG		15
#define HEADERSIZE  32

struct message {
    char txt [LENGTH];
    short senderID;
    short receiverID;
    char priority;
    short msgNum;
    char notSure [UNKNOWN];
    struct message * next;
};

struct Header {
    long		lSignature;     // must be 0xDEADBEEF
    long		lReceiverAddr;	// receiver ID. 0xff=broadcast
    BYTE		bVersion;		// version 0 = no compression, version ff = huffman
    long		lDataLength;	// size of message
    long        lDataUncompressed; //Size of original uncompressed data.
    BYTE        bSenderAddr;    // sender ID.
    BYTE        bDataType;      // data type = 0, data is text. data type = ff, data is voice.
    char		bTBD[9];		// to be determined
    long		lPattern;	// must be 0xaa 0x55 0xaa 0x55
};

typedef struct Header Header;
typedef struct message Msg;
extern Msg * tail;
extern Msg * head;
extern int numberOfMessages;

void GetMessageFromFile(char *buffer,int iLen);
void InitQueue (void);
int isQueueEmpty (void);
void AddToQueue (Msg *newMsg);
Msg *DeleteFromQueue (void);




#endif // TXTMESSAGE_H
