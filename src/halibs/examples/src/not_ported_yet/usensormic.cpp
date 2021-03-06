/**
 *	\file	examples/application/usensor.cpp
 *	\brief	Example illustrating usage of sensor classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#define CPU_FREQUENCY 16000000UL

#include "avr-halib/avr/uart.h"
#include "avr-halib/ext/sensor.h"
#include "avr-halib/share/cdevice.h"
#include "avr-halib/share/delay.h"
#include "avr-halib/share/syncsensor.h"

#include "avr-halib/ext/ledblock.h"
#include "avr-halib/portmaps/robbyboard.h"
#include "avr-halib/avr/timer.h"


UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
UseInterrupt(SIG_ADC);

UseInterrupt(SIG_OUTPUT_COMPARE3A);

struct RBoard
{
	enum
	{
		controllerClk=16000000
	};

};


struct TestSensor
{
	typedef	ADConv<RBoard>	ADConverter;
	typedef	uint16_t ReturnType;
	enum
	{
// 		mux = 0x0b, //( 0 und 1) x200
// 		mux = 0x09, //( 0 und 1) x10
		mux = 0x0f, //( 2 und 3) x200
// 		mux = 0x0d, //( 2 und 3) x10
		refV = (ADConverter::ref_internal2_56),
// 		refV = (ADConverter::ref_avcc),
		prescaler = (ADConverter::recommendedPrescalar )
	};
};

enum
{
	num=1,
	controllerClk=16000000,
	prescaler=1,
	frequency=8000*num,
	waitcycle= controllerClk / prescaler / frequency -1
	
};
struct UartConfiguration:public Uart1<RBoard>
{
	enum{
// 	baudrate=19200
// 	baudrate=115200
	baudrate=230400
	
	};
}; 


// SyncSensor< AnalogSensor< TestSensor > > as;
AnalogSensorInterrupt< TestSensor > as;
SecOut< Uart< UartConfiguration > > uart;
LedBlock<LedBlock0123> leds;
void get()
{
	leds.set(1);
// 	static uint8_t i = 0 ;
// 	if (i==3) 
	static uint8_t send;
	send++;
	send %= num;
	static uint16_t value[num];
	bool gotv = as.getCachedValue(value[send]);
	bool started = as.startGetValue();
	if(send == 0)
	{
		leds.set(2);
		uint16_t val=0;
		for(uint8_t i=0;i<num;i++)
			val += value[i] ^ 0x200;
		val /= num;
// 		val ^=0x200; //(uint)
#if 0 // 1 für uint8 0 für uint16
 		uart.put((char)  0xff & (val >> 2) );
#else
		uart.put((char)  0xff & (val << 6 ));
		uart.put((char)  0xff & (val << 6 >> 8));
#endif
	}
// 	i++;
// 	i%=10;
	if (started) leds.set(8); else leds.set(4);
	
}


int main()
{
// 	CDevice< Uart< Uart1 > > uart(115200);
// 	LedBlock<LedBlock0123> leds;
	
// 	uart << "Reset! Messungen: 4 3 2 1\n\r";
	uart.put('h');
	uart.put('a');
	uart.put('l');
	uart.put('l');
	uart.put('o');
	uart.put('!');
	
	UseRegmap(timerregister, Timer3);
	
	Timer<Timer3> timer;
	
	as.init();
	
	timer.selectClock(Timer3::ps1);
	
	timer.setWaveformGenerationMode(Timer3::ctc);
	
	volatile static uint16_t heinz = 0xceff;//waitcycle;
	
	timerregister.outputCompareAH =(waitcycle >> 8);
	timerregister.outputCompareA = 0xff & waitcycle;
	
	timer.setInterruptMask(Timer3::im_outputCompareAEnable);
	heinz;
	redirectISRF(SIG_OUTPUT_COMPARE3A, &get);
	sei();

	
	while(true); 
	{
// 		uint8_t a;
// 		uart.put('a');
// 		for (int i = 0; i<77; i++)
			{
// 				uart.put((char) as.getValue()); 
// 				leds.set(a>>3);
				delay_us(100);
			}
// 		uart.put('\n');
// 		uart.put('\r');
		
// 		for (volatile uint32_t i = 50000; i; i--) ;//warten
	}
	
}
