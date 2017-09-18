#ifndef CNTRLSERVER_HPP
#define CNTRLSERVER_HPP

#include "Repository.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>

namespace CentralServerNS {

    class Connection {
    public:
        Connection(std::string &ClientAlias_in, std::string &ClientIP_in, unsigned short ClientPort_in, unsigned short ClientDownloadPort_in);

    private:
        std::string m_ClientIP;
        unsigned short m_ClientPort;
        unsigned short m_DownloadPort;
        std::string m_ClientAlias;
        std::string m_ClientRoot;
        std::string m_ServerIP;          //not needed
        std::string m_ServerPort;        //not needed

    };

    class ConnectionManager {
    public:
        ConnectionManager();
        static bool InitServer(const std::string &sFileName_in);
        void ManageConnections();

    private:
        std::unique_ptr<std::map<std::string, Connection> > m_MapOfClient;
        std::mutex m_ClientMapMutex;
        //add sockets or whatever you need here

        bool AddClientInfo(std::string &ClientAlias_in, struct sockaddr_storage &ClientInfo_in, unsigned short usClientDownloadPort_in);
        bool DeleteClientInfo(std::string &ClientAlias_in);
    };

}

#endif
