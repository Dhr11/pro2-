#include "CentralServer.hpp"

CentralServerNS::ConnectionManager::ConnectionManager() {

}

bool CentralServerNS::ConnectionManager::InitServer(const std::string &sFileName_in) {
    if(!RepositoryNS::Repository::InitRepository(sFileName_in)) {
        return false;
    }
    return true;
}

void CentralServerNS::ConnectionManager::ManageConnections() {
    //put server stuff here
}