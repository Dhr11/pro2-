#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

#include <map>
#include <vector>
#include <fstream>


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
        static std::map<std::string, std::vector<File> > m_MapOfFiles;
        std::vector<File> m_SearchResults;

    public:
        Repository ();
        static bool InitRepository(const std::string &sRepoFilePath_in);
        std::vector<File>& Search(std::string sFileName_in);
    };

} /* Repository */

#endif
