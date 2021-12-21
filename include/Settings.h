/////////////////////
// Display related //
/////////////////////
#define DISPLAY_CS 15   // CS (MUST BE LOW if not used)
#define DISPLAY_DC 14   // Used by Default
#define DISPLAY_RST 5   // Reset (MUST BE HIGH if not used)
#define DISPLAY_SCK 18  // Used by Default
#define DISPLAY_MISO -1 // NOT NEEDED (19, LEFT FLOATING)
#define DISPLAY_MOSI 23 // Used by Default
#define IR_PIN 26       // IR Receiver Pin
#define ADALIGHT_PIN_DATA 13
#define ADALIGHT_SERIAL_TX 17
#define ADALIGHT_SERIAL_RX 16

#define EEPROM_SIZE 3
#define EEPROM_ADALIGHT_R 0
#define EEPROM_ADALIGHT_G 1
#define EEPROM_ADALIGHT_B 2