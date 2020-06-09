#include "analog.h"
#include <avr/io.h>
#include "lufa.h"

// powering with PORT C Pin 6 (pololu pin 5)
//IO_struct potPower = { &DDRC, &PORTC, &PINC, 6 };

// getting reading from ADC channel 9, PORT D, Pin 6 (pololu pin 12)
//IO_struct potSignal = { &DDRD, &PORTD, &PIND, 6 };

// initialize adc
void initialize_adc() {

  // AREF = AVcc
  ADMUX = (1<<REFS0);

  // ADC Enable and prescaler of 128
  // 16000000/128 = 125000
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

}

void initialize_pot() {
  initialize_adc();
  USB_Mainloop_Handler();
  DDRE |= (1<<PIN6);
  PORTE |= (1<<PIN6);

  //DDRD &= ~(1<<PIN7);
}

// read adc value
uint16_t adc_read(uint8_t channel) {

  //ADMUX &= clear_channel;
  ADMUX &= 0b11100000;
  if (channel >= 8) {
    ADCSRB |= (1<<MUX5);
    ADMUX |= (channel - 8);
  }
  else {
    ADCSRB &= ~(1<<MUX5);
    ADMUX |= channel;
  }

  // start single conversion
  // write '1' to ADSC
  ADCSRA |= (1<<ADSC);

  // wait for conversion to complete
  // ADSC becomes '0' again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));

  return (ADC);
}
