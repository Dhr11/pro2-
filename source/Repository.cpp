#ifdef __linux__
    #include <unistd.h>
    #define GetExecutableDirectory(x, y) readlink("proc/self/exe", x, *y)
#elif __APPLE__
    #include <mach-o/dyld.h>
    #define GetExecutableDirectory(x, y) _NSGetExecutablePath(x, y)
#endif
#include "Repository.hpp"

std::unique_ptr<std::map<std::string, std::vector<RepositoryNS::File> > > RepositoryNS::Repository::m_MapOfFiles = nullptr;
std::mutex RepositoryNS::Repository::m_FileMapMutex;

RepositoryNS::File::File(const std::string &sFileName_in, const std::string &sRelativePath_in, const std::string &sAliasName_in) {
    m_FileName = sFileName_in;
    m_RelativePath = sRelativePath_in;
    m_AliasName = sAliasName_in;
}

inline bool RepositoryNS::File::Compare(const File &aFile) {
    return (aFile.m_AliasName == m_AliasName && aFile.m_FileName == m_FileName && aFile.m_RelativePath == m_RelativePath);
}

inline RepositoryNS::FileParser::FileParser(const std::string &sRepoFilePath_in) {
    m_InputFileStream.open(sRepoFilePath_in, std::fstream::in | std::fstream::out);
    if (!m_InputFileStream.is_open()) {
        throw std::exception();
    }
}

bool RepositoryNS::FileParser::Parse(std::map<std::string, std::vector<File> > &aFileMap_inout) {
    std::string sCurrentLine;
    sCurrentLine.reserve(EXPECTEDLINESIZE);

    while (std::getline(m_InputFileStream, sCurrentLine)) {
        size_t iInitialPosition = 0, iFinalPosition = sCurrentLine.find(":");
        std::vector<std::string> sLineTokens;
        sLineTokens.reserve(3); //Line is split into name, relative path and client alias
        while (iInitialPosition < sCurrentLine.size()) {
            size_t iFinalPos = iFinalPosition == std::string::npos? sCurrentLine.size() : iFinalPosition;
            sLineTokens.emplace_back(sCurrentLine.substr(iInitialPosition, iFinalPos - iInitialPosition));
            iInitialPosition = iFinalPos + 1;
            iFinalPosition = sCurrentLine.find(":", iInitialPosition);
        }

        if (sLineTokens.size() != 3) {
            return false;
        }

        std::map<std::string, std::vector<File> >::iterator mapitr;
        if ((mapitr = aFileMap_inout.find(sLineTokens[0])) != aFileMap_inout.end()) {
            mapitr->second.emplace_back(sLineTokens[0], sLineTokens[1], sLineTokens[2]);
            continue;
        }

        aFileMap_inout.emplace(std::pair<std::string, std::vector<File> > (sLineTokens[0], {{sLineTokens[0], sLineTokens[1], sLineTokens[2]}}));
    }



    if (m_InputFileStream.bad()) {
        return false;
    }
    return true;
}

RepositoryNS::Repository::Repository(std::string &sClientAlias_in) : m_ClientAlias(sClientAlias_in) {

}
RepositoryNS::Repository::Repository() {

}
/**
    Read the repository file from the specified path
*/
bool RepositoryNS::Repository::InitRepository(const std::string &sRepoFilePath_in) {
    try {
        m_MapOfFiles = std::make_unique<std::map<std::string, std::vector<RepositoryNS::File> > >();
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (GetExecutableDirectory(buffer, &size) != 0)
            return false;
        std::string pwd = buffer;
        size_t iPos= pwd.rfind("/");
        pwd = pwd.substr(0, iPos + 1);
        pwd = pwd + sRepoFilePath_in;
        FileParser aFileParser(pwd);
        if(!aFileParser.Parse(*m_MapOfFiles))
            return false;
    }
    catch(std::exception) {
        return false;
    }
    return true;
}

/**
    Search the repository for specified file
*/
std::unique_ptr<std::vector<RepositoryNS::File> > RepositoryNS::Repository::Search(std::string sFileName_in) {
    m_SearchResults = std::make_unique<std::vector<RepositoryNS::File> >();
    std::vector<File>::iterator itrSearchResults = m_SearchResults->begin();

    for (auto itr : *m_MapOfFiles) {
        const std::string &sKey = itr.first;
        if (sKey.size() >= sFileName_in.size() && sKey.compare(0, sFileName_in.size(), sFileName_in) == 0) {
            m_SearchResults->insert(itrSearchResults, itr.second.begin(), itr.second.end());
            itrSearchResults = m_SearchResults->end();
        }
    }

    return std::move(m_SearchResults);
}

bool RepositoryNS::Repository::Share(std::string &sRelativePath_in) {
    std::string sFileName;

    int iPos = sRelativePath_in.rfind("/");
    sFileName = sRelativePath_in.substr(iPos + 1);

    //modify to make sure that if the file has already been shared from the same location, prevent addition to repository
    std::map<std::string, std::vector<File> >::iterator mapitr;
    if ((mapitr = m_MapOfFiles->find(sFileName)) != m_MapOfFiles->end()) {
        std::lock_guard<std::mutex> lock(m_FileMapMutex);
        mapitr->second.emplace_back(sFileName, sRelativePath_in, m_ClientAlias);
        return true;
    }

    std::lock_guard<std::mutex> lock(m_FileMapMutex);
    std::pair<std::map<std::string, std::vector<File> >::iterator, bool> pInsertionResult;
    pInsertionResult = m_MapOfFiles->emplace(std::pair<std::string, std::vector<File> > (sFileName, {{sFileName, sRelativePath_in, m_ClientAlias}}));

    return pInsertionResult.second;
}

//future modification: should return enum to indicate failure, success or does not exist
bool RepositoryNS::Repository::Deregister(std::string &sRelativePath_in) {
    std::string sFileName;

    int iPos = sRelativePath_in.rfind("/");
    sFileName = sRelativePath_in.substr(iPos + 1);

    std::map<std::string, std::vector<File> >::iterator mapitr;
    if ((mapitr = m_MapOfFiles->find(sFileName)) != m_MapOfFiles->end()) {
        std::vector<File>::iterator vectoritr;
        for(vectoritr = mapitr->second.begin(); vectoritr != mapitr->second.end(); ++vectoritr) {
            if(vectoritr->Compare({sFileName, sRelativePath_in, m_ClientAlias})) {
                std::lock_guard<std::mutex> lock(m_FileMapMutex);
                mapitr->second.erase(vectoritr);
                //return true from here if Share function is fixed
            }
        }

        return true;
    }

    return false;
}
