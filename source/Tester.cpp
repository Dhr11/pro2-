#include <iostream>
#include <vector>
#include "Repository.hpp"

int main() {
    if(!RepositoryNS::Repository::InitRepository("repo")) {
        std::cout << "Failed to initialize the repository!" << std::endl;
        return 1;
    }

    RepositoryNS::Repository aRepository;
    std::unique_ptr<std::vector<RepositoryNS::File> > searchResult = aRepository.Search("billy.txt");

    for (size_t i = 0; i < (*searchResult).size(); i++) {
        std::cout << (*searchResult)[i];
    }

    return 0;
}
