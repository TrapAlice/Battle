#ifndef _MSG_H
#define _MSG_H

typedef struct message_t message_t;

struct message_t{
	char      *msg;
	message_t *next;
};

typedef struct messagelist_t{
	message_t  *head;
	message_t  *tail;
	int         size;
	int         limit;
}messagelist_t;

extern messagelist_t* globalMessage;

messagelist_t*  createMessageList  ( int );
void            clearMessageList   ( messagelist_t* );
void            deleteMessageList  ( messagelist_t* );
void            addMessage         ( messagelist_t*, const char*, ... );
char*           getMessage         ( const messagelist_t*, int );
int             getMessageListSize ( const messagelist_t* );

#endif