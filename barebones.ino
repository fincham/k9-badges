#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Adafruit_NeoPixel.h>

#define SWITCH_PIN 2
#define POWER_PIN 1
#define SERIAL_PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, SERIAL_PIN);

/* ISR for wakeup interrupt, can be empty if using loop */
ISR(PCINT0_vect) {
}

void setup() {
  /* ensure power supplies are off */
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);  
    
  /* set internal pull up for wakeup interrupt */
  pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);
}

/* put microcontroller in to interruptible "power down" sleep, code adapted from <https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/> */
void sleep() {
  GIMSK |= _BV(PCIE);
  PCMSK |= _BV(PCINT2);
  ADCSRA &= ~_BV(ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  sleep_enable();
  sei();
  sleep_cpu();

  cli();
  PCMSK &= ~_BV(PCINT2);
  sleep_disable();
  ADCSRA |= _BV(ADEN);

  sei();
}

void loop() {
  sleep();
  digitalWrite(POWER_PIN, HIGH); /* turn on the power to EL wire and LED rings */
  
  /* cool graphics inserted and removed here */
  
  digitalWrite(POWER_PIN, LOW); /* turn off power */
  delay(500);
}
