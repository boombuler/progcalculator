#include <Arduino.h>
#include <string>

 /*   0
    5   1
      6
    4   2
      3
*/ 
const static uint8_t charTable [128] PROGMEM  = {
        /* NUL */ 0b11111111, /* SOH */ 0b11111111, /* STX */ 0b11111111, /* ETX */ 0b11111111,
        /* EOT */ 0b11111111, /* ENQ */ 0b11111111, /* ACK */ 0b11111111, /* BEL */ 0b11111111,
        /* BS  */ 0b11111111, /* HT  */ 0b11111111, /* LF  */ 0b11111111, /* VT  */ 0b11111111,
        /* FF  */ 0b11111111, /* CR  */ 0b11111111, /* SO  */ 0b11111111, /* SI  */ 0b11111111,
        /* DLE */ 0b11111111, /* DC1 */ 0b11111111, /* DC2 */ 0b11111111, /* DC3 */ 0b11111111,
        /* DC4 */ 0b11111111, /* NAK */ 0b11111111, /* SYN */ 0b11111111, /* ETB */ 0b11111111,
        /* CAN */ 0b11111111, /* EM  */ 0b11111111, /* SUB */ 0b11111111, /* ESC */ 0b11111111,
        /* FS  */ 0b11111111, /* GS  */ 0b11111111, /* RS  */ 0b11111111, /* US  */ 0b11111111,
        /* SP  */ 0b00000000, /* !   */ 0b11111111, /* "   */ 0b11111111, /* #   */ 0b11111111,
        /* $   */ 0b11111111, /* %   */ 0b11111111, /* &   */ 0b11111111, /* '   */ 0b11111111,
        /* (   */ 0b11111111, /* )   */ 0b11111111, /* *   */ 0b11111111, /* +   */ 0b11111111,
        /* ,   */ 0b11111111, /* -   */ 0b01000000, /* .   */ 0b11111111, /* /   */ 0b11111111,
        /* 0   */ 0b00111111, /* 1   */ 0b00000110, /* 2   */ 0b01011011, /* 3   */ 0b01001111,
        /* 4   */ 0b01100110, /* 5   */ 0b01101101, /* 6   */ 0b01111101, /* 7   */ 0b00000111,
        /* 8   */ 0b01111111, /* 9   */ 0b01101111, /* :   */ 0b11111111, /* ;   */ 0b11111111,
        /* <   */ 0b11111111, /* =   */ 0b11111111, /* >   */ 0b11111111, /* ?   */ 0b11111111,
        /* @   */ 0b11111111, /* A   */ 0b01110111, /* B   */ 0b11111111, /* C   */ 0b00111001,
        /* D   */ 0b11111111, /* E   */ 0b01111001, /* F   */ 0b01110001, /* G   */ 0b11111111,
        /* H   */ 0b01110110, /* I   */ 0b11111111, /* J   */ 0b11111111, /* K   */ 0b11111111,
        /* L   */ 0b00110001, /* M   */ 0b11111111, /* N   */ 0b00110111, /* O   */ 0b11111111,
        /* P   */ 0b01110011, /* Q   */ 0b11111111, /* R   */ 0b11111111, /* S   */ 0b11111111,
        /* T   */ 0b11111111, /* U   */ 0b00111110, /* V   */ 0b11111111, /* W   */ 0b11111111,
        /* X   */ 0b11111111, /* Y   */ 0b11111111, /* Z   */ 0b11111111, /* [   */ 0b11111111,
        /* \   */ 0b11111111, /* ]   */ 0b11111111, /* ^   */ 0b11111111, /* _   */ 0b00001000,
        /* `   */ 0b11111111, /* a   */ 0b11111111, /* b   */ 0b01111100, /* c   */ 0b01011000,
        /* d   */ 0b01011110, /* e   */ 0b11111111, /* f   */ 0b11111111, /* g   */ 0b11111111,
        /* h   */ 0b01110100, /* i   */ 0b11111111, /* j   */ 0b11111111, /* k   */ 0b11111111,
        /* l   */ 0b11111111, /* m   */ 0b11111111, /* n   */ 0b01010100, /* o   */ 0b01011100,
        /* p   */ 0b11111111, /* q   */ 0b11111111, /* r   */ 0b01010000, /* s   */ 0b11111111,
        /* t   */ 0b01111000, /* u   */ 0b00111000, /* v   */ 0b11111111, /* w   */ 0b11111111,
        /* x   */ 0b11111111, /* y   */ 0b01101110, /* z   */ 0b11111111, /* {   */ 0b11111111,
        /* |   */ 0b11111111, /* }   */ 0b11111111, /* ~   */ 0b11111111, /* DEL */ 0b11111111
    };


class Display {
    private:
        uint8_t PinData, PinClock, PinCS;

        const byte REG_NOOP = 0x00;
        const byte REG_DIGIT[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
        const byte REG_DECODE_MODE = 0x09;
        const byte REG_INTENSITY = 0x0A;
        const byte REG_SCAN_LIMIT = 0x0B;
        const byte REG_SHUTDOWN = 0x0C;
        const byte REG_DISPLAY_TEST = 0x0F;

        void shiftOut(volatile byte data);
        void spiTransfer(volatile byte reg, volatile byte data);
    public:
        Display(int data, int clock, int cs) {
            PinData = data;
            PinClock = clock;
            PinCS = cs;
        }
        void setup();
        // configures the display
        // enabled -> enable or disable the display
        // intensity -> value between 0 and 15 to set the brightness.
        void configure(bool enabled, uint8_t intensity);
        bool setText(std::string value);
};