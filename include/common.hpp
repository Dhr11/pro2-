#include<iostream>
#include<string>
#include<thread>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
// waitpid() might overwrite errno, so we save and restore it:
#include <sys/wait.h>
#include <signal.h>
#include<pthread.h>

using namespace std;
#define BACKLOG 10	 // how many pending connections queue will hold

enum command {
   invalid,
   search,
   share,
   get,
   del,
   exec
};

command hashit (std::string const& inString) {
    if (inString == "search") return search;
    if (inString == "share") return share;
    if (inString == "get") return get;
    if (inString == "del") return del;
    if (inString == "exec") return exec;
    return invalid;
}

vector<string> words;

int Mysockinit(int sockfd,string port,string server);
int threadsupply(int sockfd,void *(*connection_handler) (int*));
void *get_in_addr(struct sockaddr *sa);
int connecttoserver(int sockfd,char* client,char* port);
string messageprocessing(string inp);
