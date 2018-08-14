#include "./log.h"
#include <iostream>
#include "../common/Navigator/Navigator.h"
#include "../common/funcs/funcs.h"

static void listCommits(std::size_t commitsCounter) {
    // IMPLEMENT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

static void logCommits() {
    std::size_t commitsCounter = line::cli::common::funcs::readCommitsCounter();
    if(commitsCounter) {
        listCommits(commitsCounter);
    } else {
        std::cout << "Commits:" << std::endl << "Nothing to show. No commit has been made." << std::endl;
    }
}

void line::cli::log(int argc, char** argv) {
    const char* argumentDescription = "Directory wich is line repository wich commits should be listed.";
    if(!argc) {
        std::cout << "'line log' expects one argument. " << argumentDescription << std::endl;
        return;
    }
    if(argc > 1) {
        std::cout << "'line log' expects only one argument. " << argumentDescription << std::endl;
        return;
    }
    line::cli::common::Navigator::init(argv[0]);
    if(line::cli::common::funcs::isRepository()) {
        logCommits();
    }
}