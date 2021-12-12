#include <Arduino.h>
#include <EEPROM.h>
#include <IRrecv.h>

#include "Settings.h"
#include "Display/Display.h"
#include "Display/Pages/Monitor_1.h"
#include "Display/Pages/Monitor_2.h"
#include "Display/Pages/Audio_Switcher.h"
#include "Display/Pages/LED_Switcher.h"
#include "Adalight/Adalight.h"
#include "Util/Computer.h"
#include "Util/SerialHandler.h"
#include "Network/CmdProcessor.h"

#define SERIAL_BUFFER_SIZE  150

TaskHandle_t core0_task1;
TaskHandle_t core1_task1;

// Adalight
Adalight adalight;
// Display
Display display = Display();
Monitor_1 monitor_1;
Monitor_2 monitor_2;
Audio_Switcher audio_switcher;
LED_Switcher led_switcher;
Computer computer;
uint8_t main_page = 0;
// Serial
SerialHandler serial_handler;
char serial_buffer[SERIAL_BUFFER_SIZE];
CmdProcessor cmd_processor;
// IR Receiver
IRrecv irrecv(IR_PIN);
decode_results results;
uint32_t ir_value;

/**
 * Core dedicated to Adalight
 **/
void core0_task1_loop(void * pvParameters) {
  //// Setup
  // Initialize Serial
  Serial2.begin(921600, SERIAL_8N1, ADALIGHT_SERIAL_RX, ADALIGHT_SERIAL_TX);
  // Initialize Adalight
  adalight = Adalight(120, &Serial2);
  //// Loop
  while (true)
  {
    adalight.update();
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

/**
 * Core dedicated to Serial messages processing, Display driving and IR receiver
 **/
void core1_task1_loop(void * pvParameters) {
  //// Setup
  // Initialize Serial
  Serial.begin(500000);
  serial_handler = SerialHandler(&Serial, serial_buffer, SERIAL_BUFFER_SIZE, '\n');
  cmd_processor = CmdProcessor(&computer);
  // Initialize Display
  display.init(DISPLAY_CS, DISPLAY_DC, DISPLAY_MOSI, DISPLAY_SCK, DISPLAY_RST, DISPLAY_MISO);
  monitor_1 = Monitor_1(display.getTFT(), &computer);
  monitor_2 = Monitor_2(display.getTFT(), &computer);
  audio_switcher = Audio_Switcher(display.getTFT(), &computer, 0);
  led_switcher = LED_Switcher(display.getTFT(), &adalight, &computer, 0);
  display.setPage(&monitor_1);
  // IR Receiver
  irrecv.enableIRIn();
  //// Loop
  while (true) {
    // Read SerialPort & Process commands
    if (serial_handler.update() != 0) {
      char* response = (char*)cmd_processor.process(serial_handler.buffer);
      Serial.println(response);

      delete response;
      serial_handler.discardBuffer();
    }
    
    // Check for IR commands
    if (irrecv.decode(&results)) {
      ir_value = results.value;
      irrecv.resume();
    }
    
    // Update Display
    uint16_t display_response = display.update(ir_value);
    switch (display_response) {
      case 0:
      {
        main_page = 0;
        display.setPage(&monitor_1); 
        break;
      }
      case 1: 
      {
        audio_switcher.set_caller_page(main_page);
        display.setPage(&audio_switcher); 
        break;
      }
      case 2: 
      {
        led_switcher.set_caller_page(main_page);
        display.setPage(&led_switcher); 
        break;
      }
      case 3: 
      {
        main_page = 3;
        display.setPage(&monitor_2); 
        break;
      }
      default: break;
    }
    ir_value = UINT32_MAX;

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // General initialization
  setCpuFrequencyMhz(240);
  EEPROM.begin(EEPROM_SIZE);

  // Initialize cores
  xTaskCreatePinnedToCore(
                    core0_task1_loop,     /* Task function. */
                    "Core 0, Task 1",     /* name of task. */
                    10000,                /* Stack size of task */
                    NULL,                 /* parameter of the task */
                    1,                    /* priority of the task */
                    &core0_task1,         /* Task handle to keep track of created task */
                    0);                   /* pin task to core 0 */
  xTaskCreatePinnedToCore(
                    core1_task1_loop,     /* Task function. */
                    "Core 1, Task 1",     /* name of task. */
                    10000,                /* Stack size of task */
                    NULL,                 /* parameter of the task */
                    1,                    /* priority of the task */
                    &core1_task1,         /* Task handle to keep track of created task */
                    1);                   /* pin task to core 1 */
}

// Runs on core 1
void loop() { 
  
}