#include "msg.h"
#include "libtcod.h"
#include <stdarg.h>
#include <stdio.h>
#include "moonmem.h"

Message* head;
Message* tail;
int messageAmount;

Message* createMessage(char* msg){
	Message* message = malloc(sizeof(Message));
	message->msg = msg;
	return message;
}

void deleteMessage(Message* message){
	free(message->msg);
	free(message);
}

void Msg_init(){
	head = NULL;
	tail = NULL;
	messageAmount=0;
}



void Msg_addMessage(const char* msg, ...){
	char* buff=malloc(sizeof(char)*32);
	va_list ap;
	va_start(ap, msg);
	vsprintf(buff, msg, ap);
	va_end(ap);
	messageAmount++;
	Message* message = createMessage(buff);
	if( head==NULL ){
		head = message;
		tail = head;
	} else {
		tail->next = message;
		tail = message;
	}
	if(messageAmount>14){
		Message* temp = head;
		head = head->next;
		deleteMessage(temp);
		messageAmount--;
	}
}

char* Msg_getMessage(int pos){
	Message* temp = head;
	while(pos>0){
		if(temp == NULL) break;
		temp = temp->next;
		pos--;
	}
	return (temp == NULL) ? "" : temp->msg;
}

void Msg_uninit(){
	Message* temp = head;
	while(temp != NULL){
		head = head->next;
		deleteMessage(temp);
		temp = head;
	}
}