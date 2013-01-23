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

MessageList* createMessageList(int);
void clearMessageList(MessageList*);
void deleteMessageList(MessageList*);
void addMessage(MessageList*, const char*, ...);
char* getMessage(MessageList*, int);

#endif