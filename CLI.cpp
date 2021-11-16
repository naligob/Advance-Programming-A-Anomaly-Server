#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    CLI::dio = dio;
    auto *element1 = new UploadCommand(dio);
    CLI::commands[0] = element1;
    auto *element2 = new SetAlgorithmCommand(dio);
    CLI::commands[1] = element2;
    auto *element3 = new AnomalyDetectionCommand(dio);
    CLI::commands[2] = element3;
    auto *element4 = new ResultCommand(dio);
    CLI::commands[3] = element4;
    auto *element5 = new UploadAnoAnalyzeCommand(dio);
    CLI::commands[4] = element5;
    auto *element6 = new ExitCommand(dio);
    CLI::commands[5] = element6;
    this->size = 6;
}

void CLI::start(){
    commandsDataCollector cdc;
    printCommands();
    float input;
    this->dio->read(&input);
    while (input!=6) {
        commands[(int)input-1]->execute(cdc);
        printCommands();
        this->dio->read(&input);
    }
}

CLI::~CLI() {
    for (int i = 0; i < this->size; ++i) {
        delete(this->commands[i]);
    }
}

void CLI::printCommands() {
    this->dio->write("Welcome to the Anomaly Detection Server.\n"
                     "Please choose an option:\n");
    for (int i = 0; i < this->size; ++i) {
        this->dio->write(commands[i]->printDescription()+"\n");
    }
}

