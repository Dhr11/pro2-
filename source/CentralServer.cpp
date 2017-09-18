#include "CentralServer.hpp"

CentralServerNS::ConnectionManager::ConnectionManager() {
    m_MapOfClient = std::make_unique<std::map<std::string, Connection> >();
}

bool CentralServerNS::ConnectionManager::InitServer(const std::string &sFileName_in) {
    if(!RepositoryNS::Repository::InitRepository(sFileName_in)) {
        return false;
    }
    return true;
}

bool CentralServerNS::ConnectionManager::AddClientInfo(std::string &ClientAlias_in, struct sockaddr_storage &ClientInfo_in, unsigned short usClientDownloadPort_in) {
    std::string sClientIP;
    unsigned short usClientPort;
    if(ClientInfo_in.ss_family == AF_INET) {
        struct sockaddr_in* saClientInfo = reinterpret_cast<struct sockaddr_in*>(&ClientInfo_in);
        usClientPort = saClientInfo->sin_port;
        char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string
        inet_ntop(AF_INET, &(saClientInfo->sin_addr), ip4, INET_ADDRSTRLEN);
        sClientIP = ip4;
    }
    else {
        struct sockaddr_in6* saClientInfo = reinterpret_cast<struct sockaddr_in6*>(&ClientInfo_in);
        usClientPort = saClientInfo->sin6_port;
         char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
         inet_ntop(AF_INET6, &(saClientInfo->sin6_addr), ip6, INET6_ADDRSTRLEN);
        sClientIP = ip6;
    }

    std::lock_guard<std::mutex> lock(m_ClientMapMutex);
    std::pair<std::map<std::string, Connection>::iterator, bool> pInsertionResult;
    pInsertionResult = m_MapOfClient->emplace(std::pair<std::string, Connection>(ClientAlias_in, {ClientAlias_in, sClientIP, usClientPort, usClientDownloadPort_in}));
    return pInsertionResult.second;
}

inline bool CentralServerNS::ConnectionManager::DeleteClientInfo(std::string &ClientAlias_in) {
    std::lock_guard<std::mutex> lock(m_ClientMapMutex);
    return static_cast<bool>(m_MapOfClient->erase(ClientAlias_in));
}

void CentralServerNS::ConnectionManager::ManageConnections() {
    //put server stuff here
}

CentralServerNS::Connection::Connection(std::string &ClientAlias_in, std::string &ClientIP_in, unsigned short ClientPort_in, unsigned short DownloadPort_in) :
    m_ClientIP(ClientIP_in), m_ClientPort(ClientPort_in), m_DownloadPort(DownloadPort_in), m_ClientAlias(ClientAlias_in) {

}