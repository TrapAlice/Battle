#include "msg.h"
#include "libtcod.h"
#include <stdarg.h>
#include <stdio.h>
#include "moonmem.h"

messagelist_t* globalMessage;

static message_t* _createMessage(char* msg){
	message_t* message = malloc(sizeof(message_t));
	message->msg = msg;
	return( message );
}

static void _deleteMessage(message_t* message){
	free(message->msg);
	free(message);
}


messagelist_t* createMessageList(int limit){
	messagelist_t* messageList = malloc(sizeof(messagelist_t));
	messageList->limit = limit;
	return( messageList );
}

void addMessage(messagelist_t* const messageList, const char* msg, ...){
	char* buff;
	int size;
	message_t* head;
	message_t* tail;
	va_list ap;
	message_t* message;
	message_t* temp;
	

	if( !messageList ){
		return;
	}

	buff = malloc(sizeof(char)*64);
	va_start(ap, msg);
	vsprintf(buff, msg, ap);
	va_end(ap);

	size = messageList->size;
	head = messageList->head;
	tail = messageList->tail;

	size++;

	message = _createMessage(buff);

	if( !head ){
		head = message;
		tail = head;
	} else {
		tail->next = message;
		tail = message;
	}
	if(size>=messageList->limit){
		temp = head;
		head = head->next;
		_deleteMessage(temp);
		size--;
	}
	messageList->head = head;
	messageList->tail = tail;
	messageList->size = size;
}

char* getMessage(const messagelist_t* const messageList, int pos){
	message_t* temp = messageList->head;
	while( pos>0 ){
		if( temp == NULL ) break;
		temp = temp->next;
		pos--;
	}
	return( temp == NULL ? "" : temp->msg );
}

int getMessageListSize(const messagelist_t* const messageList){
	return( messageList->size );
}

void clearMessageList(messagelist_t* const messageList){
	message_t* temp = messageList->head;
	message_t* head = messageList->head;
	int x=0;
	while( temp ){
		head = head->next;
		_deleteMessage(temp);
		temp = head;
		messageList->size--;
		x++;
	}
	messageList->head = NULL;

}

void deleteMessageList(messagelist_t* const messageList){
	clearMessageList(messageList);
	free(messageList);
}