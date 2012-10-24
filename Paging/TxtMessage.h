#ifndef TXTMESSAGE_H
#define TXTMESSAGE_H

#define LENGTH 140	// Length of message in buffer
#define UNKNOWN 25	// Unknown purpose
#define FORTUNETXT	"FortuneCookies.txt"
#define READMODE	"r"
#define NEWMSGCODE  "%%"
#define NUMMSG		15

struct message {
    char txt [LENGTH];
    short senderID;
    short receiverID;
    char priority;
    short msgNum;
    char notSure [UNKNOWN];
    struct message * next;
};

typedef struct message Msg;
Msg * tail, * head;

void GetMessageFromFile(char *buffer,int iLen);
void InitQueue (void);
int isQueueEmpty (void);
void AddToQueue (Msg *newMsg);
Msg *DeleteFromQueue (void);



#endif // TXTMESSAGE_H
