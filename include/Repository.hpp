#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

#include <map>
#include <vector>
#include <fstream>
#include <memory>
#include <mutex>

namespace RepositoryNS {

    const int EXPECTEDLINESIZE = 512;

    class File {
    private:
        std::string m_FileName;
        std::string m_RelativePath;
        std::string m_AliasName;
        friend std::ostream& operator<< (std::ostream &outputStream, const File& fFile);

    public:
        File (const std::string &sFileName_in, const std::string &sRelativePath_in, const std::string &sAliasName_in);
        bool Compare(const File &aFile);
    };

    inline std::ostream& operator<< (std::ostream &outputStream, const File& fFile) {
        outputStream << fFile.m_FileName << ":"
                     << fFile.m_RelativePath << ":"
                     << fFile.m_AliasName << std::endl;
        return outputStream;
    }

    class FileParser {
    private:
        std::fstream m_InputFileStream;

    public:
        FileParser (const std::string &sRepoFilePath_in);
        virtual bool Parse(std::map<std::string, std::vector<File> > &aFileMap_inout);
    };

    class Repository {
    private:
        static std::unique_ptr<std::map<std::string, std::vector<File> > > m_MapOfFiles;
        static std::mutex m_FileMapMutex;
        std::unique_ptr<std::vector<File> > m_SearchResults;
        std::string m_ClientAlias; //later can create a factory for this class to allow registered users access

    public:
        Repository (std::string &sClientAlias_in);
        Repository();
        ~Repository(); //Write logic to write to file
        static bool InitRepository(const std::string &sRepoFilePath_in);
        std::unique_ptr<std::vector<File> > Search(std::string sFileName_in);
        bool Share(std::string &sRelativePath_in);
        bool Deregister(std::string &sRelativePath_in);
    };

} /* Repository */

#endif
