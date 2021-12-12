#include <EEPROM.h>

#include "Adalight/Adalight.h"
#include "Settings.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define MAGICSIZE  sizeof(magic)

// Check values are header byte # - 1, as they are indexed from 0
#define HICHECK    (MAGICSIZE)
#define LOCHECK    (MAGICSIZE + 1)
#define CHECKSUM   (MAGICSIZE + 2)

// Settings
#define LED_TYPE        WS2812B // led strip type for FastLED
#define COLOR_ORDER     GRB     // color order for bitbang
#define SERIAL_FLUSH            // Serial buffer cleared on LED latch
#define CLEAR_ON_START          // LEDs are cleared on reset

Adalight::Adalight(uint16_t numLeds, HardwareSerial* serial) {
	setColor(EEPROM.readByte(EEPROM_ADALIGHT_R), EEPROM.readByte(EEPROM_ADALIGHT_G), EEPROM.readByte(EEPROM_ADALIGHT_B));

	// Adalight
    magic[0] = 'A'; magic[1] = 'd'; magic[2] = 'a';

    leds.reserve(numLeds);
    for (uint16_t i = 0; i < numLeds; i++) leds.push_back(CRGB());
	ledsRaw = (uint8_t*)leds.data();

    this->data_pin = data_pin;
    this->serial = serial;

    FastLED.addLeds<LED_TYPE, ADALIGHT_PIN_DATA, COLOR_ORDER>(leds.data(), numLeds);
    FastLED.setBrightness(brightness);
    #ifdef CLEAR_ON_START
		FastLED.show();
	#endif

	this->serial->print("Ada\n");           // Send ACK string to host

	lastByteTime = millis();  // Set initial counters
	
	initialized = true;
}

void Adalight::setMode(uint8_t mode) {
	switch (mode) {
		case 0: enabled = false; FastLED.setBrightness(255); animation_type = 0; break;
		case 1: enabled = true; FastLED.setBrightness(255); break;
		case 2: enabled = false; animation_type = mode - 1; break;
	}
}

void Adalight::setColor(uint8_t r, uint8_t g, uint8_t b) {
	static_color.r = r; static_color.g = g; static_color.b = b;
	RGBtoHSV(
		static_color.raw[0] / 255.0, static_color.raw[1] / 255.0, static_color.raw[2] / 255.0, 
		static_color_hsv[0], static_color_hsv[1], static_color_hsv[2]
	);
	// EEPROM saving
	if (!initialized) return;
	EEPROM.writeByte(EEPROM_ADALIGHT_R, r);
	EEPROM.writeByte(EEPROM_ADALIGHT_G, g);
	EEPROM.writeByte(EEPROM_ADALIGHT_B, b);
	EEPROM.commit();
}

void Adalight::setBrightness(uint8_t value) {
	FastLED.setBrightness(value);
}

void Adalight::update() {
    t = millis(); // Save current time

	if (this->enabled) {
		// If there is new serial data
		while ((c = serial->read()) >= 0){
			lastByteTime = t; // Reset timeout counters

			switch(mode) {
				case Header:
					headerMode();
					break;
				case Data:
					dataMode();
					break;
			}
		}
	} else {
		// 60 FPS
		if (t - t_anim > 16) {
			t_anim = t;
			animation(animation_type);
		}
	}
}

void Adalight::animation(uint8_t animation_type) {
	switch (animation_type)
	{
		case 0: // Turn Off LEDs
			memset(leds.data(), 0, leds.size() * sizeof(CRGB));
			FastLED.show();
			break;
		case 1:
			// Animation Type 1
			{
				// anim_var_1 represents multiplier value
				// anim_var_2 represents h variation (hsv color space)
				// anim_var_3 represent the current rotation
				for (uint16_t i = 0; i < leds.size(); i++) {
					float h = static_color_hsv[0] + anim_var_2 * sin(((PI / 180.0 * 10) * i) + anim_var_1);
					while (h > 360.0) h -= 360.0;
					while (h < 0.0) h += 360.0;

					float rgb[3];
					HSVtoRGB(rgb[0], rgb[1], rgb[2], h, static_color_hsv[1], static_color_hsv[2]);

					leds[i].r = rgb[0] * 255;
					leds[i].g = rgb[1] * 255;
					leds[i].b = rgb[2] * 255;
				}
				// Speed of h variation
				anim_var_1 += PI / 180.0 * 3.0; 
				if (anim_var_1 > 2.0 * PI) anim_var_1 -= 2.0 * PI;

				FastLED.show();
			}
			break;
		
		default:
			break;
	}
}

void Adalight::headerMode(){
	static uint8_t
		headPos,
		hi, lo, chk;

	if(headPos < MAGICSIZE){
		// Check if magic word matches
		if(c == magic[headPos]) {headPos++;}
		else {headPos = 0;}
	}
	else{
		// Magic word matches! Now verify checksum
		switch(headPos){
			case HICHECK:
				hi = c;
				headPos++;
				break;
			case LOCHECK:
				lo = c;
				headPos++;
				break;
			case CHECKSUM:
				chk = c;
				if(chk == (hi ^ lo ^ 0x55)) {
					// Checksum looks valid. Get 16-bit LED count, add 1
					// (# LEDs is always > 0) and multiply by 3 for R,G,B.
					bytesRemaining = 3L * (256L * (long)hi + (long)lo + 1L);
					outPos = 0;
                    memset(leds.data(), 0, leds.size() * sizeof(CRGB));
					mode = Data; // Proceed to latch wait mode
				}
				headPos = 0; // Reset header position regardless of checksum result
				break;
		}
	}
}

void Adalight::dataMode(){
	// If LED data is not full
	if (outPos < leds.size() * 3){
		dataSet();
	}
	bytesRemaining--;
 
	if(bytesRemaining == 0) {
		// End of data -- issue latch:
		mode = Header; // Begin next header search
		FastLED.show();
		#ifdef SERIAL_FLUSH
			serialFlush();
		#endif
	}
}

void Adalight::dataSet(){
	ledsRaw[outPos++] = c; // Issue next byte
}

void Adalight::serialFlush(){
	while(serial->available() > 0) {
		serial->read();
	}
}

/*! \brief Convert RGB to HSV color space
  
  Converts a given set of RGB values `r', `g', `b' into HSV
  coordinates. The input RGB values are in the range [0, 1], and the
  output HSV values are in the ranges h = [0, 360], and s, v = [0,
  1], respectively.
  
  \param fR Red component, used as input, range: [0, 1]
  \param fG Green component, used as input, range: [0, 1]
  \param fB Blue component, used as input, range: [0, 1]
  \param fH Hue component, used as output, range: [0, 360]
  \param fS Hue component, used as output, range: [0, 1]
  \param fV Hue component, used as output, range: [0, 1]
  
*/
void Adalight::RGBtoHSV(float fR, float fG, float fB, float& fH, float& fS, float& fV) {
	float fCMax = max(max(fR, fG), fB);
	float fCMin = min(min(fR, fG), fB);
	float fDelta = fCMax - fCMin;

	if(fDelta > 0) {
		if(fCMax == fR) {
			fH = 60 * (fmod(((fG - fB) / fDelta), 6));
		} else if(fCMax == fG) {
			fH = 60 * (((fB - fR) / fDelta) + 2);
		} else if(fCMax == fB) {
			fH = 60 * (((fR - fG) / fDelta) + 4);
		}

		if(fCMax > 0) {
			fS = fDelta / fCMax;
		} else {
			fS = 0;
		}

		fV = fCMax;
	} else {
		fH = 0;
		fS = 0;
		fV = fCMax;
	}

	if(fH < 0) {
		fH = 360 + fH;
	}
}


/*! \brief Convert HSV to RGB color space
  
  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.
  
  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]
  \param fH Hue component, used as input, range: [0, 360]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]
  
*/
void Adalight::HSVtoRGB(float& fR, float& fG, float& fB, float fH, float fS, float fV) {
	float fC = fV * fS; // Chroma
	float fHPrime = fmod(fH / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = fV - fC;

	if(0 <= fHPrime && fHPrime < 1) {
		fR = fC;
		fG = fX;
		fB = 0;
	} else if(1 <= fHPrime && fHPrime < 2) {
		fR = fX;
		fG = fC;
		fB = 0;
	} else if(2 <= fHPrime && fHPrime < 3) {
		fR = 0;
		fG = fC;
		fB = fX;
	} else if(3 <= fHPrime && fHPrime < 4) {
		fR = 0;
		fG = fX;
		fB = fC;
	} else if(4 <= fHPrime && fHPrime < 5) {
		fR = fX;
		fG = 0;
		fB = fC;
	} else if(5 <= fHPrime && fHPrime < 6) {
		fR = fC;
		fG = 0;
		fB = fX;
	} else {
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;
}