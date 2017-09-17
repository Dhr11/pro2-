//#include <iostream>
//#include "CentralServer.hpp"
#include "Repository.hpp"
#include "common.hpp"
//using namespace std;

void *connection_handler(int* sock)
{

    int read_size;
    char *message , client_message[2000];
    string reply;

    //Send some messages to the client
    message = "Greetings! I am your server\n";
    write(*sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(*sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
				printf("received: %s\n",client_message);
        int check(0);
        reply=messageprocessing();
        write(sock , reply , reply.length());
        //memset(client_message,' ',sizeof(client_message));
		}

    if(read_size == 0)
    {
        //printf("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
		close(sock);
    free(sock);			//Free the socket pointer
    return 0;
}


int main(int argc, char const *argv[]) {
    if(argc != 5) {
        cout << "Input parameters invalid!" << endl;
        return 1;
    }

    string server=argv[1];
    string s_port=argv[2];
    string RepoFileName = argv[3];
    string LoggedinFile=argv[4];
    string Repoloc=argv[5];
    int sockfd(0);

    if(!RepositoryNS::Repository::InitRepository(RepoFileName)) {
        cout << "Server failed to initialize failed to initialize. Input filename: " << RepoFileName << endl;
        return 1;
    }

    //CentralServerNS::ConnectionManager aConnectionManager;
    //aConnectionManager.ManageConnections();
    sockfd=Mysockinit(sockfd,port,server);//hints,servinfo,p);
	  if(sockfd==-1)
		  return 1;

	  if (listen(sockfd, BACKLOG) == -1) {
		  perror("listen");
		  exit(1);
	  }


	  printf("server: waiting for connections...\n");
	  sockfd=threadsupply(sockfd, connection_handler);
    return 0;
}
