#include "TinyGPS++.h"
#include "ep154.h"

#define DEMO_DELAY 3000

// look-up tables, provided by Waveshare
const unsigned char full_mode[] = {
        0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 0x66, 0x69,
        0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 0x00, 0x00, 0x00, 0x00,
        0xf8, 0xb4, 0x13, 0x51, 0x35, 0x51, 0x51, 0x19, 0x01, 0x00 };
const unsigned char partial_mode[] = {
        0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Global Variables
unsigned char fbuf[EP_FRAMEMEMSIZE];
TinyGPSPlus gps;
int ledState = 1;

static void smartDelay(unsigned long ms);
void clearDisplay();

void setup(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    Serial1.begin(9600);

    ep_spi_init();
    clearDisplay();
    ep_init((unsigned char*)partial_mode);

    Serial.println("Setup Done");
}

void loop() {
    smartDelay(DEMO_DELAY);
    Serial.println(gps.charsProcessed());

    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;

    Serial.print("seconds: ");
    Serial.println(gps.time.second());

    char kphString[5];
    //dtostrf(gps.speed.kmph(), 3, 1, kphString);
    dtostrf(random(0, 10), 3, 1, kphString);

    ep_setbg(fbuf, ALL_WHITE);
    ep_write_text(fbuf, kphString, 0, 2, COL_BLACK, 4.0, 4.0, NULL, PEN_THICK);
    ep_setframe(fbuf, 0, 0, EP_WIDTH, EP_HEIGHT);
    ep_displayframe();
}

void clearDisplay() {
    ep_init((unsigned char*)full_mode);
    ep_setbg(fbuf, ALL_WHITE);
    ep_setframe(fbuf, 0, 0, EP_WIDTH, EP_HEIGHT);
    ep_displayframe();
}

// This custom version of delay() ensures that the gps object is being "fed".
static void smartDelay(unsigned long ms) {
    unsigned long start = millis();
    do
    {
      while (Serial1.available())
        gps.encode(Serial1.read());
    } while (millis() - start < ms);
}
