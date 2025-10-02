#ifndef ROBOFORGE_ARDUINO_H
#define ROBOFORGE_ARDUINO_H

#include "_time.h"
#include "hserial.h"
#include "funcs.h"
#include "sock.h"
#include "engine.h"
#include "components/bundle.h"
#include <vector>

#define hr_clock std::chrono::high_resolution_clock
#define hr_time_point hr_clock::time_point
#define hr_duration hr_clock::duration

#define millis() get_millis(start_time)
#define micros() get_micros(start_time)

#define empty()

auto ard = Arduino::create(33, 5);

hr_time_point start_time;
HSerial Serial;

json container;

std::vector<shared_ptr<Component>> comps;
std::vector<shared_ptr<Wire>> wires;

bool sleeping = false;

#define String string
// both are not the same but do this temporarily

#define pinMode(a, b) pinMode_(ard, a, b)
#define digitalWrite(a, b) digitalWrite_(ard, a, b)
#define digitalRead(a) digitalRead_(ard, a)
#define analogRead(a) analogRead_(ard, a)
#define analogReference(a) analogReference_(ard, a)
#define analogWrite(a, b) analogWrite_(ard, a, b)

#define pulseIn(a, b, c) pulseIn_(ard, a, b, c)
#define pulseInLong(a, b, c) pulseInLong_(ard, a, b, c)

#define attachInterrupt(a, b, c) attchInterrupt_(ard, a, b, c)
#define detachInterrupt(a) detachInterrupt_(ard, a)

#define delay(a) send_msg(); delay(a); send_msg()
#define delayMicroseconds(a) send_msg(); delayMicroseconds(a); send_msg();

#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0
#define DISPLAY 1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define DEFAULT 1
#define EXTERNAL 0
#define INTERNAL1V1 2
#define INTERNAL 3
#define INTERNAL2V56 3

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() empty()
#define noInterrupts() empty()

#define F_CPU 16000000L

#define clockCyclesPerMicrosecond() 16
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

#define analogInPinToBit(P) (P)

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_AN_INTERRUPT (-1)

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

#define PIN_A0 14
#define PIN_A1 15
#define PIN_A2 16
#define PIN_A3 17
#define PIN_A4 18
#define PIN_A5 19
#define PIN_A6 20
#define PIN_A7 21

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8

#define TIMER3A 9
#define TIMER3B 10
#define TIMER3C 11
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
#define TIMER4D 15
#define TIMER5A 16
#define TIMER5B 17
#define TIMER5C 18

#define NUM_DIGITAL_PINS 20
#define NUM_ANALOG_INPUTS 6
#define analogInputToDigitalPin(p) ((p < 6) ? (p) + 14 : -1)

#define digitalPinHasPWM(p) ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (18)
#define PIN_WIRE_SCL        (19)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;



static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))


#define SERIAL_PORT_MONITO  R   Serial
#define SERIAL_PORT_HARDWARE  Serial


#endif
