#ifndef CNTRLSERVER_HPP
#define CNTRLSERVER_HPP

#include "Repository.hpp"

namespace CentralServerNS {

    class Connection {
    public:
        Connection();

    private:
        std::string m_ClientIP;
        std::string m_ClientPort;
        std::string m_DownloadPort;
        std::string m_ClientAlias;
        std::string m_ClientRoot;
        std::string m_ServerIP;
        std::string m_ServerPort;

    };

    class ConnectionManager {
    public:
        ConnectionManager();
        static bool InitServer(const std::string &sFileName_in);
        void ManageConnections();

    private:
        std::vector<Connection> m_Client;
        //add sockets or whatever you need here
    };

}

#endif
