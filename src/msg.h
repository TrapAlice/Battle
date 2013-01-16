#ifndef _MSG_H
#define _MSG_H

typedef struct Message{
	char* msg;
	struct Message* next;
}Message;

void Msg_init();
void Msg_uninit();
void Msg_addMessage(const char*, ...);
char* Msg_getMessage(int);

#endif//_MSG_H