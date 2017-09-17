/*
** Client.cpp
*/

#include "common.hpp"
#define MAXDATASIZE 2000 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
//char* DPORT_OWN,*DPORT_SRC,*SERVER_PORT;
//char* client;



//void *C2C_handler(int *sock)
//{
//	int sock = *(int*)socket_desc;					//Get the socket descriptor
//	int numbytes;
//	char *message;
//	message="Message received from another client\n";//, client_message[2000];
//	if ((numbytes = send(sock, message, MAXDATASIZE, 0)) == -1) {
//			perror("send failed");
//			exit(1);
//	}
//}


//void *local_server(int *sockfd)
//{
//int sockfd=*(int*)socket_desc;
//sockfd=Mysockinit(sockfd,DPORT_OWN,client);

//if(sockfd==-1)
//return 0;

//if (listen(sockfd, BACKLOG) == -1) {
//	perror("listen");
//	exit(1);
//}

//printf("client: waiting for connections...\n");
//sockfd=threadsupply(sockfd, C2C_handler);


//return 0;
//}

int main(int argc, char *argv[])
{

	int sockfd, numbytes,ownfd;

	//char s[INET6_ADDRSTRLEN];

	if (argc != 8) {
	    cerr<<"usage: client hostname\n";
	    exit(1);
	}
  m_ClientAlias=argv[1];
  m_ClientIP=argv[2];
  m_ClientPort=argv[3];
  m_ServerIP=argv[4];
  m_ServerPort=argv[5];
  m_DownloadPort=argv[6];
  m_ClientRoot=argv[7];

	//DPORT_OWN=argv[2];
	//SERVER_PORT=argv[3];
	//client=argv[1];              //.client clienthostname ownport serverport

	//ownfd=Mysockinit(ownfd, DPORT_OWN, client);

	//pthread_t localserver_thread;
	//int *newsock;
	//newsock = new int;
	//*newsock = ownfd;
	//if( pthread_create( &localserver_thread , NULL ,  local_server , (void*) newsock) < 0)
	 //{
		//	 perror("could not create thread");
		//	 return 1;
	 //}
	 //printf("thread create after");
		//pthread_join( sniffer_thread , NULL);
	//	printf("Handler assigned");

	sockfd=connecttoserver(sockfd,m_ClientIP,m_ServerPort);


	//char message[MAXDATASIZE];//,sreply[MAXDATASIZE];
  string message;
  while(1)
{
	cout<<"Enter message ";
	//fgets(message,MAXDATASIZE,stdin);
  getline(std::cin,message);
  if ((numbytes = send(sockfd, message, MAXDATASIZE, 0)) == -1) {
			perror("send failed");
			exit(1);
	}

char buffer[1024];

int n;
while((errno = 0, (n = recv(sockfd, buffer, sizeof(buffer), 0))>0) ||
	errno == EINTR)
	{
		if(n>0)
			printf("received:: %s",buffer);//output.append(buffer, n);
		else if(n==0)            ///to come out when no interaction
			break;
		//memset(buffer,' ',1024);
	}
if(errno<0)
{
	perror("receive failed");
}
cout<<" after server interaction";
	//buf[numbytes] = '\0';

//char ch[5];//=NULL;
//printf("Do You want to send something to other client(y/n)\n");
//fgets(ch,1,stdin);
//read(fileno(stdin),ch,1);
//if(ch[0]=='y')
//{
//	int confd;
//	printf("Enter the port of client you want to connect to");
	//read(fileno(stdin),DPORT_SRC,4);
//	scanf("%s",DPORT_SRC);
//	confd=connecttoserver(confd,client,DPORT_SRC);    //here client from crs files and port too

//	char buffer[1024];
//	int n;
//	while((errno = 0, (n = recv(confd, buffer, sizeof(buffer), 0))>0) ||
//		errno == EINTR)
//		{
//			if(n>0)
//				printf("received:: %s",buffer);//output.append(buffer, n);
			//memset(buffer,' ',1024);
//		}
//	if(errno<0)
//	{
//		perror("receive failed");
//	}

//		close(confd);
//}
close(sockfd);
}
	return 0;
}
