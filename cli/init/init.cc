#include "./init.h"
#include "../common/funcs/funcs.h"
#include "../common/Navigator/Navigator.h"
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <fstream>

struct CreatedResources {
    bool repoInfoDir = false;
    bool commits = false;
    bool objects = false;
    bool commitsCounter = false;
};

static void warnFailedToInitializeRepostiory(const char* directory) {
    std::cout << "Failed to initialize line repository in " << directory << std::endl;
}

static void cleanCreatedResourcesAndWarnFailInitialize(const CreatedResources& createdResources) {
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    if(createdResources.commitsCounter) {
        errno = 0;
        unlink(navigator.navigateToCommitsCounter());
    }
    if(createdResources.commits) {
        errno = 0;
        rmdir(navigator.navigateToCommits());
    }
    if(createdResources.objects) {
        errno = 0;
        rmdir(navigator.navigateToObjects());
    }
    if(createdResources.repoInfoDir) {
        errno = 0;
        rmdir(navigator.navigateToRepoInfoDir());
    }
    warnFailedToInitializeRepostiory(navigator.navigateToDirectory());
}

static void initRepository(const char* directory) {
    CreatedResources createdResources;
    line::cli::common::Navigator::init(directory);
    line::cli::common::Navigator& navigator = line::cli::common::Navigator::navigator();
    if(line::cli::common::funcs::mkdirWithPermisions(navigator.navigateToRepoInfoDir())) {
        if(errno == EEXIST) {
            std::cout << directory << " is a line repository." << std::endl;
        } else {
            warnFailedToInitializeRepostiory(directory);
        }
        return;
    }
    createdResources.repoInfoDir = true;
    if(line::cli::common::funcs::mkdirWithPermisions(navigator.navigateToObjects())) {
        cleanCreatedResourcesAndWarnFailInitialize(createdResources);
        return;
    }
    createdResources.objects = true;
    if(line::cli::common::funcs::mkdirWithPermisions(navigator.navigateToCommits())) {
        cleanCreatedResourcesAndWarnFailInitialize(createdResources);
        return;
    }
    createdResources.commits = true;
    std::ofstream commitsCounter{navigator.navigateToCommitsCounter()};
    if(!commitsCounter) {
        cleanCreatedResourcesAndWarnFailInitialize(createdResources);
        return;
    }
    createdResources.commitsCounter = true;
    commitsCounter << 0;
    if(!commitsCounter) {
        commitsCounter.close();
        cleanCreatedResourcesAndWarnFailInitialize(createdResources);
        return;
    }
    commitsCounter.close();
    std::cout << "Initialized empty line repository in " << directory << std::endl;
}

void line::cli::init(int argc, char** argv) {
    const char* argumentDescription = "The directory to be initialized as line repository.";
    if(!argc) {
        std::cout << "'line init' expects one argument. " << argumentDescription << std::endl;
        return;
    }
    if(argc > 1) {
        std::cout << "'line init' expects only one argument. " << argumentDescription << std::endl;
        return;
    }
    const char* directory = argv[0];
    if(line::cli::common::funcs::isDirectory(directory)) {
        initRepository(directory);
    }
}