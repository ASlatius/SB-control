#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <SoftwareSerial.h>
#include <HC12.h>

#include "gamescore.h"
#include "gametime.h"
#include "comm.h"

#define D4      4
#define D5      5
#define D6      6
#define D7      7
#define D8      8 
#define D9      9
#define D10     10
#define D11     11
#define D12     12

#define RESET_TIME          (0x01)
#define RESET_SCORE         (0x02)
#define TIME_STARTSTOP      (0x04)
#define TIME_SET            (0x08)
#define SCORE_HOME_PLUS     (0x10)
#define SCORE_HOME_MIN      (0x20)
#define SCORE_GUEST_PLUS    (0x40)
#define SCORE_GUEST_MIN     (0x80)

#define SCORE_ANY           (SCORE_HOME_PLUS | SCORE_HOME_MIN | SCORE_GUEST_PLUS | SCORE_GUEST_MIN)
#define TIME_ANY            (TIME_STARTSTOP | TIME_SET)
#define RESET_ANY           (RESET_TIME | RESET_SCORE)
#define LONGPRESS           15

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64

uint8_t keys=0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SoftwareSerial HC12Serial(2,3);
HC12 hc12(&HC12Serial, 13, 9600);
GameScore gameScore;
GameTime gameTime;
uint32_t longpress_rst_time=0, longpress_rst_score=0;

/******************************************************************************
 * Read all inputs to see if user pressed a key
 *****************************************************************************/
int scanKeys()
{
    if (0 == digitalRead(D4)) {
        //Serial.println("D4-RS");
        keys |= RESET_SCORE;
    }
    if (0 == digitalRead(D9)) {
        //Serial.println("D9-RT");
        keys |= RESET_TIME;
    }
    if (0 == digitalRead(D5)) {
        //Serial.println("D5-G-");
        keys |= SCORE_GUEST_MIN;
    }
    if (0 == digitalRead(D6)) {
        //Serial.println("D6-G+");
        keys |= SCORE_GUEST_PLUS;
    }
    if (0 == digitalRead(D7)) {
        //Serial.println("D7-H-");
        keys |= SCORE_HOME_MIN;
    }
    if (0 == digitalRead(D8)) {
        //Serial.println("D8-H+");
        keys |= SCORE_HOME_PLUS;
    }
    if (0 == digitalRead(D10)) {
        //Serial.println("D10-TSS");
        keys |= TIME_STARTSTOP;
    }
    if (0 == digitalRead(D11)) {
        //Serial.println("D11-TSE");
        keys |= TIME_SET;
    }
    Serial.print(keys);
    return keys;
}

/******************************************************************************
 * Handle all actions from the scanned keys.
 *****************************************************************************/
void handleKeys()
{
    if (keys & SCORE_HOME_PLUS) {
        gameScore.incScore(SCORE_HOME);
    }
    else if (keys & SCORE_HOME_MIN) {
        gameScore.decScore(SCORE_HOME);
    }
    else if (keys & SCORE_GUEST_PLUS) {
        gameScore.incScore(SCORE_GUEST);
    }
    else if (keys & SCORE_GUEST_MIN) {
        gameScore.decScore(SCORE_GUEST);
    }
    if (keys & TIME_STARTSTOP) {
        if (false == gameTime.isRunning()) {
            gameTime.run();
        } else {
            gameTime.stop();
        }
    }
    // Only change game time if not running
    if (keys & TIME_SET) {
        if (false == gameTime.isRunning())
        {
            gameTime.upGameTime();
        }
    }
    // Make user press reset key longer before reset is done
    if (keys & RESET_SCORE) {
        longpress_rst_score++;
        if (longpress_rst_score >= LONGPRESS) {
            gameScore.reset();
        }
    }
    else if (longpress_rst_score > 0) {
        longpress_rst_score = 0;
    }
    // Make user press reset key longer before reset is done
    if (keys & RESET_TIME) {
        longpress_rst_time++;
        if (longpress_rst_time >= LONGPRESS) {
            gameTime.reset();
        }
    }
    else if (longpress_rst_time > 0) {
        longpress_rst_time = 0;
    }
    
    // Filter for key bounce - bad way to do proper key bounce filtering. Works though... 
    if ((keys & ~(RESET_SCORE|RESET_TIME)) > 0) {
        delay(250);
    }
    keys = 0;
}

/******************************************************************************
 * Show the total game time, running game time and score on display.
 *****************************************************************************/
void updateDisplay()
{
    String text;
    int16_t x1, y1, i;
    uint16_t w, h;

    display.clearDisplay();

    display.setFont(&FreeSans9pt7b);
    text = gameTime.getTimeString(GAME_TIME) + " - " + gameTime.getTimeString(MAX_TIME);
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(((SCREEN_WIDTH-w)/2) ,14);
    display.setTextColor(WHITE);
    display.println(text);

    display.setFont(&FreeSans18pt7b);
    text = gameScore.getScore(SCORE_HOME) + " - " + gameScore.getScore(SCORE_GUEST);
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(((SCREEN_WIDTH-w)/2) ,50);
    display.setTextColor(WHITE);
    display.println(text); 

    if (longpress_rst_score > 0 || longpress_rst_time > 0) {
        i = longpress_rst_score + longpress_rst_time;
        display.fillRect(0, 61, ((i*128)/LONGPRESS), 4, WHITE);
    }

    display.display();
}

void showMeko74()
{
    display.setFont(&FreeSans9pt7b);
    display.setCursor(25,30);
    display.setTextColor(WHITE);
    display.println("MEKO '74"); 
    display.display();

    delay(2000);
}

/******************************************************************************
 * Interrupt service routine. Called every second, count time 
 *****************************************************************************/
ISR(TIMER1_COMPA_vect)
{
    gameTime.count();
}

/******************************************************************************
 * Initialize hardware etc.
 *****************************************************************************/
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);       // on board LED
    pinMode(D4, INPUT_PULLUP);          // Keys
    pinMode(D5, INPUT_PULLUP);
    pinMode(D6, INPUT_PULLUP);
    pinMode(D7, INPUT_PULLUP);
    pinMode(D8, INPUT_PULLUP);
    pinMode(D9, INPUT_PULLUP);
    pinMode(D10, INPUT_PULLUP);
    pinMode(D11, INPUT_PULLUP);
    pinMode(D12, OUTPUT);               // "Gestart" LED

    noInterrupts();                     // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A  = 15625;                     // compare match register 16MHz/1024/1Hz
    TCCR1B |= (1 << WGM12);             // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);    // 1024 prescaler 
    TIMSK1 |= (1 << OCIE1A);            // enable timer compare interrupt
    interrupts();                       // enable all interrupts

    hc12.begin();
    Serial.begin(9600);                 // Initialize Serial connectioan
    Wire.begin();                       // Initialize I2C
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display
    display.clearDisplay();

    showMeko74();
}

/******************************************************************************
 * Main loop
 *****************************************************************************/
void loop() {

    digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
    if (false != gameTime.isRunning()) {
        digitalWrite(D12, true);
    } else {
        digitalWrite(D12, false);
    }
    
    // Read and handle keys, update display.
    scanKeys();
    updateDisplay();
    handleKeys();

    // Send out data to display on HC12
    commSendDisplayData(hc12, gameScore, gameTime);

    delay(30);  //to make loop ~100ms
}