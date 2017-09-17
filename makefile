CC = g++
CFLAGS = -Wall -Wextra -std=c++11
SOFLAGS = -dynamiclib -install_name @rpath/.
DEBUG = -g
RELEASE = -O3
SOURCEPATH = ./src
INCPATH = ./include
INC = -I $(INCPATH)
DEBUGLINK = -L $(DEBUGDIR)
RELEASELINK = -L $(RELEASEDIR)
RELEASEDIR = ./bin/release
DEBUGDIR = ./bin/debug
PWD = $(shell pwd)
VPATH = $(INCPATH):$(SOURCEPATH):$(RELEASEDIR):$(DEBUGDIR)

all: libRepository.dylib libRepositoryd.dylib libCentralServer.dylib libCentralServerd.dylib Server Serverd Tester Testerd

libRepository.dylib: Repository.o
	$(CC) $(SOFLAGS)/$@ $(RELEASEDIR)/$? -o $(RELEASEDIR)/$@

Repository.o: Repository.cpp Repository.hpp Common.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/Repository.cpp -o $(RELEASEDIR)/$@

libRepositoryd.dylib: Repositoryd.o
	$(CC) $(SOFLAGS)/$@ $(DEBUG) $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

Repositoryd.o: Repository.cpp Repository.hpp Common.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/Repository.cpp -o $(DEBUGDIR)/$@

Tester: Tester.o
	$(CC) $(RELEASELINK) -rpath '$(PWD)/$(RELEASEDIR)' -lRepository $(RELEASEDIR)/$? -o $(RELEASEDIR)/$@

Tester.o: Tester.cpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/Tester.cpp -o $(RELEASEDIR)/$@

Testerd: Testerd.o
	$(CC) $(DEBUGLINK) -rpath '$(PWD)/$(DEBUGDIR)' -lRepositoryd $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

Testerd.o: Tester.cpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/Tester.cpp -o $(DEBUGDIR)/$@

Server: Server.o
	$(CC) $(RELEASELINK) -rpath '$(PWD)/$(RELEASEDIR)' -lCentralServer $(RELEASEDIR)/$? -o $(RELEASEDIR)/$@

Serverd: Serverd.o
	$(CC) $(DEBUGLINK) -rpath '$(PWD)/$(DEBUGDIR)' -lCentralServerd $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

Server.o: Server.cpp CentralServer.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/Server.cpp -o $(RELEASEDIR)/$@

Serverd.o: Server.cpp CentralServer.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/Server.cpp -o $(DEBUGDIR)/$@

libCentralServer.dylib: CentralServer.o
	$(CC) $(SOFLAGS)/$@ $(RELEASELINK) -lRepository $(RELEASEDIR)/$? -o $(RELEASEDIR)/$@

CentralServer.o: CentralServer.cpp CentralServer.hpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(RELEASE) -c $(SOURCEPATH)/CentralServer.cpp -o $(RELEASEDIR)/$@

libCentralServerd.dylib: CentralServerd.o
	$(CC) $(SOFLAGS)/$@ $(DEBUG) $(DEBUGLINK) -lRepositoryd $(DEBUGDIR)/$? -o $(DEBUGDIR)/$@

CentralServerd.o: CentralServer.cpp CentralServer.hpp Repository.hpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) -c $(SOURCEPATH)/CentralServer.cpp -o $(DEBUGDIR)/$@
