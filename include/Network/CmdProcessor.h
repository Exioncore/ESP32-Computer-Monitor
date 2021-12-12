#ifndef CmdProcessor_h
#define CmdProcessor_h

#include <Arduino.h>
#include <StringProcessor.h>

#include "Display/Display.h"
#include "Util/Computer.h"

class CmdProcessor {
    public:
        // Constructor
        CmdProcessor() {};
        CmdProcessor(Computer* computer) { this->computer = computer; };

        char* process(char* cmd);

    private:
        StringProcessor stringProcessor;

        Computer* computer;
        void processComputer(char* cmd, char* response, char* buff, StringProcessor stringProcessor);
        void processAudio(char* cmd, char* response, char* buff, StringProcessor stringProcessor);
};

#endif