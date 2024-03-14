
//센서보드 온도센서(TMP37)

typedef unsigned char INT8;
typedef unsigned INT16;
#define F_CPU 8000000UL
//#define F_CPU 1000000UL //예제 3-5
#include <avr/interrupt.h>
#include <util/delay.h>
#include "clcd_d8.h"
#include <avr/io.h>
#include <stdio.h>

//#define adc_ch 3
//#define adc_ch 5 //예제 3-2
#define adc_ch 3
//채널을 5번으로 변경한다면 3채널을 사용하던 PF3에서 PF5로 변경해줘야 한다
#define tmp37 50
//#define tmp35 100 //예제 3-1
//#define ref_v 5.0
#define ref_v 2.56 //예제3-3
#define resol 1023
//#define resol 255 //예제3-4

void adc_init(INT8 channel);

volatile INT16 read_adc=0;
volatile INT8 adc_flag=0;

ISR(ADC_vect)
{
	read_adc=ADCL;//10비트 ADC값 저장
	read_adc+=(ADCH<<8);//ADC값저장
	//read_adc=ADCH; //3-4
	adc_flag=1;//ADC변환 확인을
}

int main(void)
{
	char buffer[64];
	
	clcd_port_init();
	clcd_init_8bit();
	volatile float tem_v=0.0;
	volatile float temp=0.0;
	clcd_position(0,0);
	clcd_str("20180747DAHOON");
	DDRG=0xff;	
	adc_init(adc_ch);
	sei();
			
    while (1) 
    {
		if(adc_flag==1)
		{
			adc_flag=0;
			tem_v=(float)read_adc*(ref_v/resol*1000);
			temp=tem_v*(float)tmp37/1000;
			PORTG=(INT8)temp;
			
			sprintf(buffer,"%3.0fmV Tem%3.2fC", tem_v, temp);
			clcd_position(1,0);
			clcd_str(buffer);
			
		}
		_delay_ms(200);
		ADCSRA|=(1<<ADSC);//start Conversion AD변환의 시작을 알리는 스크립트
    }
}

void adc_init(INT8 channel)
{
	asm("CLI");//==cli() inlineasembler (cli 클리어 인터럽트로 adc회로 수행중 인터럽트 발생을 막기위해 cli와 sei사이는 인터럽트 발생을 억제
	ADMUX=/*(0<<REFS1)*/(1<<REFS1)/*예제3-3*/|(1<<REFS0)|(0<<ADLAR)/*(1<<ADLAR)3-4*/|channel;
	ADCSRA=(1<<ADEN)|(1<<ADSC)|(0<<ADFR)/*(1<<ADFR) 예제3-6*/|(1<<ADIE)|
	(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
	//(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0); //3-5
	asm("SEI");//setinterrupt 클리어 인터럽트 종료
	/*
	
	*/
}
