#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int run = 0;

ISR(INT0_vect){
    if (run == 0)
        run = 1 ; // still sleep status 
    else 
        run = 0 ; // out from sleep status 
}

void TIMER0_Init(uint8_t count, uint8_t dim) {
    TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << CS01);
    //OCR0A = count;
    OCR0B = dim;
}

void INTO_init(){
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0); // set internal interrupt PORTD2 
}

int main(void) {
    // set up PORTD5 
    DDRD |= (1 << DDD5);
    PORTD &= ~(1 << PORTD5);
    
    TIMER0_Init(255,0);
    INTO_init();
    
    sei();
    
    while (1){
        // led on
        for (int i = 0; i < 255; i++){
            OCR0B = i;
            _delay_ms(10);
        }
        // led off 
        for (int i = 255; i >= 0; i--){
            OCR0B = i;
            _delay_ms(10);
        }
        // stay in sleep mode 
        if (run == 0){
            set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
            sleep_enable();
            sleep_cpu();
            sleep_disable();
        }
    }
    
}