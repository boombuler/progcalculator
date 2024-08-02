#include "Display.h"

void Display::setup() {
    pinMode(PinData, OUTPUT);
    pinMode(PinClock, OUTPUT);
    pinMode(PinCS, OUTPUT);
    digitalWrite(PinCS,HIGH);
    digitalWrite(PinClock, LOW);

    // Disable Display-Test
    spiTransfer(REG_DISPLAY_TEST, 0);
    // Since we are using "common anode", we need to set the scan limit to "all digits"
    // which means that the digits are instead the segments, and segments are the digits.
    spiTransfer(REG_SCAN_LIMIT, 7); 
    // Manual decoding:
    spiTransfer(REG_DECODE_MODE, 0);
    // clear display
    for (int i = 0; i < 8; i++)
        spiTransfer(REG_DIGIT[i], 0x00);
    // disable display for now.
    spiTransfer(REG_SHUTDOWN, 0);
}

void Display::shiftOut(volatile byte data) {
    for (uint8_t i = 0; i < 8; i++)  {    
        digitalWrite(PinData, (data & 0x80) != 0);
        data <<= 1;
        digitalWrite(PinClock, HIGH);
        digitalWrite(PinClock, LOW);
    }
}

void Display::spiTransfer(volatile byte reg, volatile byte data) {
    digitalWrite(PinCS, LOW);
    shiftOut(reg);
    shiftOut(data);
    digitalWrite(PinCS, HIGH);
}

void Display::configure(bool enabled, uint8_t intensity) {
    spiTransfer(REG_INTENSITY, intensity);
    spiTransfer(REG_SHUTDOWN, enabled ? 1 : 0);
}

bool Display::setText(std::string value) {
    byte buffer[8] = {0,0,0,0,0,0,0,0};

    int idx = 0;

    for (int i = value.length() - 1; i >= 0; --i) {
        if (idx >= 8) {
            return false;
        }

        char c = value[i];
        if (c > 127) {
            return false;
        }

        if (c == '.') {
            buffer[0] |= 1 << idx;
        } else {
            auto segments = pgm_read_byte_near(charTable + c);
            if (segments & 0x80) {
                return false;
            }
            buffer[1] |= ((segments >> 0) & 1) << idx;
            buffer[2] |= ((segments >> 1) & 1) << idx;
            buffer[3] |= ((segments >> 2) & 1) << idx;
            buffer[4] |= ((segments >> 3) & 1) << idx;
            buffer[5] |= ((segments >> 4) & 1) << idx;
            buffer[6] |= ((segments >> 5) & 1) << idx;
            buffer[7] |= ((segments >> 6) & 1) << idx;
        }
       
        idx++;
    }

    for (int i = 0; i < 8; i++) {
        spiTransfer(REG_DIGIT[i], buffer[i]);
    }
    return true;
}