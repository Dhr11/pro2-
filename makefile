CC = g++
CFLAGS = -Wall -Wextra -fPIC -std=c++1y
SOFLAGS = -shared
DEBUG = -g
RELEASE = -O3
SOURCEPATH = ./source
INCPATH = ./include
INC = -I $(INCPATH)
DEBUGLINK = -L $(DEBUGDIR)
RELEASELINK = -L $(RELEASEDIR)
RELEASEDIR = ./bin/release
DEBUGDIR = ./bin/debug
PWD = $(shell pwd)
VPATH = $(INCPATH):$(SOURCEPATH):$(RELEASEDIR):$(DEBUGDIR)

all: libRepository.so libRepositoryd.so libCentralServer.so libCentralServerd.so Client Clientd Server Serverd Tester Testerd

libRepository.so: Repository.o
	$(CC) $(SOFLAGS) $(RELEASEDIR)/$? -o $(RELEASEDIR)/$@

Repository.o: Repository.cpp Repository.hpp common.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/Repository.cpp -o $(RELEASEDIR)/$@

libRepositoryd.so: Repositoryd.o
	$(CC) $(SOFLAGS) $(DEBUG) $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

Repositoryd.o: Repository.cpp Repository.hpp common.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/Repository.cpp -o $(DEBUGDIR)/$@

Tester: Tester.o
	$(CC) $(RELEASELINK) -Wl,rpath,'$(PWD)/$(RELEASEDIR)' -lRepository $(RELEASEDIR)/$? -o $(RELEASEDIR)/$@

Tester.o: Tester.cpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/Tester.cpp -o $(RELEASEDIR)/$@

Testerd: Testerd.o
	$(CC) $(DEBUGLINK) -Wl,rpath,'$(PWD)/$(DEBUGDIR)' -lRepositoryd $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

Testerd.o: Tester.cpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/Tester.cpp -o $(DEBUGDIR)/$@

Server: Server.o common.o
	$(CC) $(RELEASELINK) -Wl,rpath,'$(PWD)/$(RELEASEDIR)' -lCentralServer $(RELEASEDIR)/Server.o $(RELEASEDIR)/common.o -o $(RELEASEDIR)/$@

Serverd: Serverd.o commond.o
	$(CC) $(DEBUGLINK) -Wl,rpath,'$(PWD)/$(DEBUGDIR)' -lCentralServerd $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

Server.o: Server.cpp CentralServer.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/Server.cpp -o $(RELEASEDIR)/$@

Serverd.o: Server.cpp CentralServer.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/Server.cpp -o $(DEBUGDIR)/$@

libCentralServer.so: CentralServer.o
	$(CC) $(SOFLAGS) $(RELEASELINK) -lRepository $(RELEASEDIR)/$? -o $(RELEASEDIR)/$@

CentralServer.o: CentralServer.cpp CentralServer.hpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/CentralServer.cpp -o $(RELEASEDIR)/$@

libCentralServerd.so: CentralServerd.o
	$(CC) $(SOFLAGS) $(DEBUG) $(DEBUGLINK) -lRepositoryd $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

CentralServerd.o: CentralServer.cpp CentralServer.hpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/CentralServer.cpp -o $(DEBUGDIR)/$@

common.o: common.cpp common.hpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/common.cpp -o $(RELEASEDIR)/$@

commond.o: common.cpp common.hpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/common.cpp -o $(DEBUGDIR)/$@

Client: Client.o common.o
	$(CC) $(RELEASELINK) -Wl,rpath,'$(PWD)/$(RELEASEDIR)' -lRepository $(RELEASEDIR)/Client.o $(RELEASEDIR)/common.o -o $(RELEASEDIR)/$@

Clientd: Clientd.o commond.o
	$(CC) $(DEBUGLINK) $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

Client.o: Client.cpp common.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/Client.cpp -o $(RELEASEDIR)/$@

Clientd.o: Client.cpp common.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/Client.cpp -o $(DEBUGDIR)/$@
