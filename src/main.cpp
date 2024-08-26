#include <Arduino.h>
#include "Display.h"
#include "Calculator.h"


static const uint8_t PCol1 = PIN_PC0;
static const uint8_t PCol2 = PIN_PC1;
static const uint8_t PCol3 = PIN_PC2;
static const uint8_t PCol4 = PIN_PC3;
static const uint8_t PCol5 = PIN_PC4;
static const uint8_t PDspl_CS = PIN_PC5;

static const uint8_t PRow1 = PIN_PD2;
static const uint8_t PRow2 = PIN_PD3;
static const uint8_t PRow3 = PIN_PD4;
static const uint8_t PRow4 = PIN_PD5;
static const uint8_t PRow5 = PIN_PD6;
static const uint8_t PRow6 = PIN_PD7;

static const uint8_t PMode1 = PIN_PB0;
static const uint8_t PMode2 = PIN_PB1;
static const uint8_t PMode3 = PIN_PB2;

Calculator calc;
Display display(MOSI, SCK, PDspl_CS);

#define RowCount 6
const uint8_t Rows[RowCount] = {PRow1, PRow2, PRow3, PRow4, PRow5, PRow6};
#define ColCount 5
const uint8_t Cols[ColCount] = {PCol1, PCol2, PCol3, PCol4, PCol5};


const char KeyMap[RowCount][ColCount] = {
    { 'd', 'e', 'f', '<', '>' },  
    { 'a', 'b', 'c', '-', '&' },  
    { '7', '8', '9', '+', '|' },  
    { '4', '5', '6', '*', '^' },  
    { '1', '2', '3', '/', '%' },  
    { '0', '(', ')', '\b', '\r' }
};

uint8_t currentKeyStates[RowCount];
uint8_t lastKeyStates[RowCount];
unsigned long lastDebounceTime;
const int debounceDelay = 20;  // in milliseconds

Calculator::Mode readMode() {
    byte dMode = ((digitalRead(PMode1) ^ HIGH) << 0)
               | ((digitalRead(PMode2) ^ HIGH) << 1) 
               | ((digitalRead(PMode3) ^ HIGH) << 2);
    const Calculator::Mode Modes[] = {Calculator::Mode::Dec, Calculator::Mode::Bin, Calculator::Mode::Hex, Calculator::Mode::Oct};
    return Modes[dMode % 4];
}

char readKey() {
    bool stateChanged = false;

     // Scan the keyboard
    for (int r = 0; r < RowCount; r++) {
        // Set the current row HIGH
        digitalWrite(Rows[r], HIGH);

        // Read all columns
        for (int c = 0; c < ColCount; c++) {
            bool currentState = (digitalRead(Cols[c]) == HIGH);

            // Check if the current state is different from the last state
            if (currentState != bitRead(currentKeyStates[r], c)) {
                stateChanged = true;
                bitWrite(currentKeyStates[r], c, currentState);
            }
        }

        // Set the current row LOW again
        digitalWrite(Rows[r], LOW);
    }

    // Update debounce time and apply debounce logic
    if (stateChanged) {
        lastDebounceTime = millis();
    } else if ((millis() - lastDebounceTime) > debounceDelay) {
        // Update the current key states after debounce delay
        for (int r = 0; r < RowCount; r++) {
            for (int c = 0; c < ColCount; c++) {
                bool wasActive = bitRead(lastKeyStates[r], c);
                bool isActive = bitRead(currentKeyStates[r], c);
                bitWrite(lastKeyStates[r], c, isActive);                
                if (isActive && !wasActive) {
                    // new Keypress
                    return KeyMap[r][c];
                }
            }
        }
    }
    
    return 0;
}

void displayMode() {
    string text;
    switch (calc.mode())
    {
    case Calculator::Mode::Bin: text = "bn"; break;
    case Calculator::Mode::Oct: text = "oc"; break;
    case Calculator::Mode::Hex: text = "HE"; break;    
    default: text = "dc"; break;
    }
    Serial.print("mode");
    Serial.println(text.c_str());
    display.setText(text);
    delay(1000);
    display.setText("");
    delay(50);
    display.setText(calc.display);
}

bool updateMode() {
    auto oldMode = calc.mode();
    auto newMode = readMode();
    if (oldMode == newMode)
        return false;
    do
    {
        oldMode = newMode;
        delay(debounceDelay);
        newMode = readMode();
    } while(newMode != oldMode);
    return calc.setMode(newMode);
}

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < RowCount; i++) {
        pinMode(Rows[i], OUTPUT);
        digitalWrite(Rows[i], LOW);
    }
    for (int i = 0; i < ColCount; i++)
        pinMode(Cols[i], INPUT);
    pinMode(PMode1, INPUT);
    pinMode(PMode2, INPUT);
    pinMode(PMode3, INPUT);

    display.setup();
    display.configure(true, 5);

    memset(currentKeyStates, 0, sizeof(currentKeyStates));
    memset(lastKeyStates, 0, sizeof(lastKeyStates));

    updateMode();
    displayMode();

    lastDebounceTime = millis();
}

void loop() {
    bool update = false;
    if (updateMode()) {
        displayMode();
        update = true;
    }

    char k = readKey();
    if (k) {
        Serial.print(": '");
        Serial.print(k);
        Serial.println("'");
        calc.input(k);
        update = true;
    }

    if (update) {
        Serial.print("> ");
        Serial.println(calc.display.c_str());
        display.setText(calc.display);
    }
}
