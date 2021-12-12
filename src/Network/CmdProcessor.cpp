#include "Network/CmdProcessor.h"

#define BUFFER_MAX_LEN      50
#define RESPONSE_MAX_LEN    200

#define COMPUTER_PREFIX         'C'
#define AUDIO_PREFIX            'A'
#define IDENTIFICATION_PREFIX   '?'

/**
 * Process a command and return a response
 * @result response
 */
char* CmdProcessor::process(char* cmd) {
    char* response = new char[RESPONSE_MAX_LEN];

    if (strlen(cmd) > 0) {
        char* buff = new char[BUFFER_MAX_LEN];
        StringProcessor stringProcessor = StringProcessor();
        stringProcessor.setSeparators(',', '\0');
        stringProcessor.setInput(cmd + 2, strlen(cmd + 2));
        switch (cmd[0]) {
            case COMPUTER_PREFIX:
                processComputer(cmd + 1, response, buff, stringProcessor);
                break;
            case AUDIO_PREFIX:
                processAudio(cmd + 1, response, buff, stringProcessor);
                break;
            case IDENTIFICATION_PREFIX:
                sprintf(response, "Ryder Display 1.0");
                break;
            default:
                sprintf(response, "Invalid Command");
                break;
        }
        delete buff;
        return response;
    }
    sprintf(response, "Empty command");
    return response;
}

void CmdProcessor::processComputer(char* cmd, char* response, char* buff, StringProcessor stringProcessor) {
    // Process command
    switch (cmd[0]) {
        case 'I':   // Initialize Computer
        {
            char* name = new char[50];

            // CPU name
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            sprintf(name, "%s", buff);
            computer->metrics.cpu.name = name;
            // GPU name
            name = new char[50];
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            sprintf(name, "%s", buff);
            computer->metrics.gpu.name = name;
            // RAM Size
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.ram.memCapacity = atoi(buff);

            computer->metrics.initialized = true;
            sprintf(response, "OK");
        }
        case 'U':   // Update Computer
        {
            // CPU Clock
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.cpu.clock.set(atoi(buff));
            // CPU Temperature
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.cpu.temperature.set(atoi(buff));
            // CPU Load
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.cpu.load.set(atoi(buff));
            // CPU Max Thread Load
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.cpu.maxThreadLoad.set(atoi(buff));
            // CPU Power
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.cpu.power.set(atoi(buff));
            // GPU Core Clock
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.gpu.coreClock.set(atoi(buff));
            // GPU Temperature
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.gpu.temperature.set(atoi(buff));
            // GPU Load
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.gpu.coreLoad.set(atoi(buff));
            // GPU Memory clock
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.gpu.memClock.set(atoi(buff));
            // GPU Power
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.gpu.power.set(atoi(buff));
            // GPU Memory Usage
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.gpu.memUsage.set(atoi(buff));
            // Ram Usage
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.ram.memUsage.set(atoi(buff));
            // Framerate
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.framerate.set(atoi(buff));
            // Liquid Temperature
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.fanController.liquidTemperature.set(atof(buff));
            // Ambient Temperature
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            computer->metrics.fanController.ambientTemperature.set(atof(buff));

            computer->metrics.last_update = millis();
            
            if (!computer->metrics.initialized) {
                sprintf(response, "LC%d\nCI", computer->prismatik.enabled);
            } else {
                sprintf(response, "OK");
            }
        }
    }
}

void CmdProcessor::processAudio(char* cmd, char* response, char* buff, StringProcessor stringProcessor) {
    // Process command
    switch (cmd[0]) {
        case 'I':   // Initialize Audio Presets
        {
            if (computer->audio.initialized) {
                for (auto val : computer->audio.presets) delete val;
                computer->audio.presets.clear();
            }

            stringProcessor.next(buff, BUFFER_MAX_LEN);
            int n = atoi(buff);
            for (int i = 0; i < n; i++) {
                stringProcessor.next(buff, BUFFER_MAX_LEN);
                char* name = new char[strlen(buff)];
                sprintf(name, "%s", buff);
                computer->audio.presets.push_back(name);
            }
            computer->audio.initialized = true;

            sprintf(response, "OK");
        }
    }
}