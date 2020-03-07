/*
Basic WSPR example using JTEncode library and Si5351

See README in https://github.com/lu1aat/wspr-si5351 for instructions.
*/
#include <si5351.h>
#include <JTEncode.h>
#include <rs_common.h>
#include <int.h>
#include <string.h>
#include "Wire.h"

Si5351 si5351;
JTEncode jtencode;

// Global variables
#define WSPR_FREQ       14096900UL  // 20M, 14.094,60
#define LED_PIN                 13
unsigned long freq;
char call[] = "N0CALL";
char loc[] = "GF00";
uint8_t dbm = 27;
uint8_t tx_buffer[255];
uint8_t symbol_count;
uint16_t tone_delay, tone_spacing;

// Loop through the string, transmitting one character at a time.
void encode() {
    uint8_t i;

    // Reset the tone to the base frequency and turn on the output
    si5351.output_enable(SI5351_CLK0, 1);
    digitalWrite(LED_PIN, HIGH);

    for(i = 0; i < WSPR_SYMBOL_COUNT; i++) {
        si5351.set_freq((WSPR_FREQ * 100) + (tx_buffer[i] * 146), SI5351_CLK0);
        delay(683);
    }

    // Turn off the output
    si5351.output_enable(SI5351_CLK0, 0);
    digitalWrite(LED_PIN, LOW);
}

void set_tx_buffer() {
    // Clear out the transmit buffer
    memset(tx_buffer, 0, 255);
    jtencode.wspr_encode(call, loc, dbm, tx_buffer);
}

void setup() {
    // Initialize the Si5351
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);

    // Use the Arduino's on-board LED as a keying indicator.
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Set CLK0 output
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA); // Set for max power if desired
    si5351.output_enable(SI5351_CLK0, 0); // Disable the clock initially

    // Encode the message in the transmit buffer
    set_tx_buffer();
}

void loop() {
    // TX
    encode();

    // Sleep until next TX
    delay(125 * 1000UL);
}
