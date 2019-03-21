//server.ver 0.1

#include <iostream>
#pragma comment( lib, "ws2_32.lib" )
#include<windows.h>
#include<conio.h>
#define PORT 666 

SOCKET conect;
SOCKET* connections;
SOCKET server1;
int kol=0;

char* s; 
using namespace std;

void funck(int ip){
	 char* buff=new char[1024];
     while(1){
     	memset(buff,0,sizeof(buff));
        if(recv(connections[ip],buff,sizeof(buff),0)){
        	cout << buff << std::endl;
            for(int i=0;i<=kol;i++) 
			send(connections[i],buff,strlen(buff),0);
	    }
	}
	kol--;
    printf("-отключение [%s]\n",s);// PRINTNUSERS
   delete buff;
}
int main(){
	WSADATA dat;
	WORD versya;
	versya=MAKEWORD(2,0);
	connections=(SOCKET*)calloc(64,sizeof(SOCKET));
	printf("Сервер");
	if(WSAStartup(versya,&dat)){
		printf("Ошибка ");
        return -1;
	}
	
	 if ((server1=socket(AF_INET,SOCK_STREAM,0))<0){
      printf("Ошибка при создании socket %d\n",WSAGetLastError());
      WSACleanup();//удаляем сокет
      return -1;
    }
	sockaddr_in adr;
	adr.sin_family=AF_INET;
	adr.sin_port=htons(PORT);
	adr.sin_addr.s_addr=0;//IPPROTO_TCP;
	if (bind(server1,(sockaddr *) &adr,sizeof(adr))){
      printf("\nОшибка присвоение имени сокету bind %d\n",WSAGetLastError());
      closesocket(server1); 
      WSACleanup();
      return -1;
    }

	if (listen(server1, 20))/*SOMAXCONN))*/{
      printf("\nОшибка ожидание клиента %d\n",WSAGetLastError());
      closesocket(server1);
      WSACleanup();
      return -1;
    }
	printf("\waiting conection\n");
	sockaddr_in cl_addr;
	int cl_addr_size=sizeof(cl_addr);
	char buf_con[]="Done\n";
	while((conect=accept(server1,(sockaddr*)&cl_addr,&cl_addr_size))){
		 connections[kol]=conect;
		 printf("[%s] new dude!\n",inet_ntoa(cl_addr.sin_addr));
		 s=new char[cl_addr_size];
         s=inet_ntoa(cl_addr.sin_addr);                 //чтобы потом при закрытии показывало адрес компа
         send(connections[kol],buf_con,strlen(buf_con),0);	
         kol++;
         CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)funck,(LPVOID)(kol-1),NULL,NULL);
	}
	return 0;
}
