/* Raoul Larios   rlari006@ucr.edu
 * rlari006_lab10_part1.c
 * Lab Section : 024
 * Lab 10 Part 1
 *
 * I acknowledge all content is my own work while following
 * the instructions and lab manuals as reference to my work
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimeISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		TimeISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum state_machine1 {one, two, three} ThreeLEDsSM;
enum state_machine2 {one1, two2} BlinkingLEDSM;
enum state_machine3 {one11, two22} CombineLEDsSM;

unsigned char ThreeLed = 0x00;
unsigned char BlinkLed = 0x00;

void ThreeLEDs(){
	switch(ThreeLEDsSM){
		case one:
		ThreeLEDsSM = two;
		break;
		case two:
		ThreeLEDsSM = three;
		break;
		case three:
		ThreeLEDsSM = one;
		break;
		default:
		ThreeLEDsSM = one;
		break;
	}
	switch(ThreeLEDsSM){
		case one:
		ThreeLed = 0x01;
		break;
		case two:
		ThreeLed = 0x02;
		break;
		case three:
		ThreeLed = 0x04;
		break;
	}
}

void BlinkingLEDs(){
	switch(BlinkingLEDSM){
		case one1:
		BlinkingLEDSM = two2;
		break;
		case two2:
		BlinkingLEDSM = one1;
		break;
		default:
		BlinkingLEDSM = one1;
		break;
	}
	switch(BlinkingLEDSM){
		case one1:
		BlinkLed = 0x08;
		break;
		case two2:
		BlinkLed = 0x00;
		break;
	}
}

void CombineLEDs(){
	switch(CombineLEDsSM){
		case one11:
		CombineLEDsSM = two22;
		break;
		case two22:
		CombineLEDsSM = one11;
		break;
		default:
		CombineLEDsSM = one11;
		break;
	}
	switch(CombineLEDsSM){
		case one11:
		PORTB = ThreeLed | BlinkLed;
		break;
		case two22:
		PORTB = ThreeLed | BlinkLed;
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerOn();
	TimerSet(1000);
	
    while (1) 
    {
		ThreeLEDs();
		BlinkingLEDs();
		CombineLEDs();
		while (!TimerFlag){};
		TimerFlag = 0;
    }
}

