#include "msg.h"
#include "libtcod.h"
#include <stdarg.h>
#include <stdio.h>
#include "moonmem.h"

MessageList* globalMessage;

Message* createMessage(char* msg){
	Message* message = malloc(sizeof(Message));
	message->msg = msg;
	return message;
}

void deleteMessage(Message* message){
	free(message->msg);
	free(message);
}

MessageList* createMessageList(int limit){
	MessageList* messageList = malloc(sizeof(MessageList));
	messageList->limit = limit;
	return messageList;
}



void addMessage(MessageList* messageList, const char* msg, ...){
	char* buff=malloc(sizeof(char)*32);
	int size = messageList->size;
	Message* head = messageList->head;
	Message* tail = messageList->tail;
	va_list ap;
	Message* message;
	Message* temp;
	va_start(ap, msg);
	vsprintf(buff, msg, ap);
	va_end(ap);
	

	size++;

	message = createMessage(buff);

	if( head==NULL ){
		head = message;
		tail = head;
	} else {
		tail->next = message;
		tail = message;
	}
	if(size>=messageList->limit){
		temp = head;
		head = head->next;
		deleteMessage(temp);
		size--;
	}
	messageList->head = head;
	messageList->tail = tail;
	messageList->size = size;
}

char* getMessage(MessageList* messageList, int pos){
	Message* temp = messageList->head;
	while(pos>0){
		if(temp == NULL) break;
		temp = temp->next;
		pos--;
	}
	return (temp == NULL) ? "" : temp->msg;
}

void clearMessageList(MessageList* messageList){
	Message* temp = messageList->head;
	Message* head = messageList->head;
	while(temp != NULL){
		head = head->next;
		deleteMessage(temp);
		temp = head;
		messageList->size--;
	}
	messageList->head = NULL;

}

void deleteMessageList(MessageList* messageList){
	clearMessageList(messageList);
	free(messageList);
}