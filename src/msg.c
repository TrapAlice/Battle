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
	free(message);
}

void Msg_init(){
	head = NULL;
	tail = NULL;
	messageAmount=0;
	int x;
	for( x=0; x<15; x++){
		Msg_addMessage("--");
	}
}



void Msg_addMessage(const char* msg, ...){
	char* buff=malloc(sizeof(char)*20);
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

	/*deleteMessage(MessageList[head]);
	head++;
	if(head==15)head=0;

	
	MessageList[tail]=message;
	tail++;
	if(tail==15)tail=0;
	printf("%s\n",buff);
	printf("%s\n",message->msg);*/
	//MOONMEM_memout();
}

char* Msg_getMessage(int pos){
	//if(head == NULL) return "";
	Message* temp = head;
	while(pos>0){
		if(temp == NULL) break;
		temp = temp->next;
		pos--;
	}
	return (temp == NULL) ? "-" : temp->msg;
}

int Msg_size(){
	//return TCOD_list_size(MessageList);
	return 14;
}

void Msg_uninit(){
	//TCOD_list_clear(MessageList);
}