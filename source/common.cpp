#include"common.hpp"
#include"Repository.hpp"


void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *connection_handler(int* sock)
{

    int read_size;
    char *message , client_message[2000];
    string reply;

    //Send some messages to the client
    message = "Greetings! I am your server\n";
    write(*sock , message , strlen(message));
    stringstream tmp;
    //Receive a message from client
    while( (read_size = recv(*sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
				printf("received: %s\n",client_message);
        int check(0);
        tmp<<client_message;
        //memset(client_message,' ',sizeof(client_message));
		}

    reply=messageprocessing(tmp.str());
    tmp.str("");
    int num=0;
    while(num<=reply.length())
      {
        int i=write(*sock , reply.c_str() , reply.length());
        num=num+i;
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
		close(*sock);
    free(sock);			//Free the socket pointer
    return 0;
}

int Mysockinit(int sockfd,string port,string server)
{
  struct addrinfo hints, *servinfo, *p;
	int rv;
	int yes=1;

  memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(server.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
    cerr<<"getaddrinfo: "<<gai_strerror(rv)<<endl;
		return -1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure
	if (p == NULL)  {
		cerr<<"server: failed to bind\n";
    exit(1);
	}

	return sockfd;
}

int threadsupply(int sockfd,void *(*connection_handler) (int*))
{
int *new_sock;
int new_fd;
struct sockaddr_storage their_addr; // connector's address information
socklen_t sin_size;

while(1) {  // main accept() loop
  sin_size = sizeof their_addr;
  while((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)))
  {
  

    new_sock = new int;
    *new_sock = new_fd;
    //if(
       thread clientthread(connection_handler,new_sock);//) < 0)          //doubt return value
     //{
    //     perror("could not create thread");
    //     return 1;
     //}
      //pthread_join( sniffer_thread , NULL);
      cout<<"Handler assigned";
  }
  if (new_fd == -1) {
    perror("accept");
    continue;
  }

  close(new_fd);  // parent doesn't need this
}
}


int connecttoserver(int sockfd,string client,string port)
{
  struct addrinfo hints, *servinfo, *p;
	int rv;
  memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(client.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
		cerr<<"getaddrinfo: "<<gai_strerror(rv)<<endl;
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
  char s[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),                    ///getting ip
			s, sizeof s);
	cout<<"client: connecting to "<< s<<endl;

	freeaddrinfo(servinfo); // all done with this structure
  return sockfd;
  }

string messageprocessing(string inp)
{
  string word;
  istringstream rawCommand;
  rawCommand.str(inp);

  int Qcount(0);
  stringstream ss;
  while ( getline( rawCommand, word, ' ' ) ) {
   //cout<<Command<<endl;
   if(word[0]=='"' && word[word.length()-1]!='"')
   {
     ss<<word<<' ';
     Qcount++;
     continue;
   }
   if(word[0]!='"' && word[word.length()-1]=='"')
   {
     ss<<word;
     words.push_back(ss.str());
     ss.str("");
     Qcount++;
     continue;
   }
   if(Qcount%2==0)
      words.push_back(word);
   else
      ss<<word<<' ';

 }

 for(int i=0;i<words.size();i++)
 {
   cout<<words.at(i)<<endl;
 }
    string reply;
    stringstream ss2;

    RepositoryNS::Repository repo;
    std::unique_ptr<std::vector<RepositoryNS::File> > Results;
    switch (hashit(words.at(0))) {
    case invalid:
        reply="FAILURE:INVALID ARGUMENTS\n";
        return reply;
    case search:
        cout<<"search command"<<endl;
        if(words.size()!=2)
          return "FAILURE:INVALID ARGUMENTS\n";

        Results=repo.Search(words[1]);
        for(int i=0;i<(*Results).size();i++)
        {
          ss<<(*Results)[i];
          ss<<'\n';
        }
        reply=ss.str();
        return reply;
    case share:
        cout<<"share commmand";
        if(words.size()!=2)
            return "FAILURE:INVALID ARGUMENTS\n";
        reply="share command\n";
        return reply;
    case get2:
        cout<<"get command";
        if(words.size()!=3 && words.size()!=4)
            return "FAILURE:INVALID ARGUMENTS\n";
        return "get command\n";
    case del:
        cout<<"del command";
        if(words.size()!=2)
            return "FAILURE:INVALID ARGUMENTS\n";
        return "del command";

    case exec:
        cout<<"exec command\n";
        if(words.size()!=3)
            return "FAILURE:INVALID ARGUMENTS\n";
        return "exec command\n";
    }
  return 0;
}
