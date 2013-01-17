#ifndef _MSG_H
#define _MSG_H

typedef struct Message{
	char* msg;
	struct Message* next;
}Message;

typedef struct{
	Message* head;
	Message* tail;
	int size;
	int limit;
}MessageList;

MessageList* Msg_create(int);
void Msg_clear(MessageList*);
void Msg_delete(MessageList*);
void Msg_addMessage(MessageList*, const char*, ...);
char* Msg_getMessage(MessageList*, int);

#endif//_MSG_H