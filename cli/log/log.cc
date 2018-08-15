#include "./log.h"
#include <iostream>
#include <cassert>
#include "../common/Navigator/Navigator.h"
#include "../common/FileLineReader/FileLineReader.h"
#include "../common/Timestamp/Timestamp.h"
#include "../common/funcs/funcs.h"

static void listCommits(std::size_t commitsCounter) {
    assert(commitsCounter);
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    line::cli::common::FileLineReader commitsLogReader{navigator.navigateToCommitsLog()};
    std::cout << "Commits:" << std::endl;
    for(std::size_t counter = 1; counter <= commitsCounter; ++counter) {
        line::cli::common::Timestamp timestamp = line::cli::common::Timestamp::fromString((*commitsLogReader).beginning);
        ++commitsLogReader;
        std::cout << counter << ": \"" << (*commitsLogReader);
        ++commitsLogReader;
        std::cout << "\" from " << timestamp << " by " << (*commitsLogReader) << std::endl;
        ++commitsLogReader;
    }
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