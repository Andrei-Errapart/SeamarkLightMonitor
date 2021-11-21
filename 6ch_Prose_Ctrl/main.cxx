/*
vim: ts=4
vim: shiftwidth=4
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/crc16.h>
#include "usart.h"

#define DEFAULT_BRIGHT 100

#define TMR0_FREQ 1000ul
#define TMR0_PRESC 256ul
#define TMR0_RELOAD (0ul - (F_CPU / (TMR0_FREQ * TMR0_PRESC)))

#define TMR2_FREQ 32000ul
#define TMR2_PRESC 8ul
#define TMR2_RELOAD (0ul - (F_CPU / (TMR2_FREQ * TMR2_PRESC)))

/** Timeout to use default intensity, seconds. */
#define	DEFAULT_TIMEOUT		900

#define F_UPD_BRIGHT 0

#define F_SPI_CS_STATE 		0
#define F_SPI_CLK_STATE 	1

#define EEPROM_PCKT_SIGNATURE 0x5B

volatile uint8_t sysflags	= 0;

uint8_t pwm_cnt;
volatile uint8_t pwm_val	= DEFAULT_BRIGHT;

uint8_t led_cur_bright;
uint8_t led_default_bright;
uint8_t led_new_bright;
uint8_t led_new_default_bright;

volatile uint8_t spi_flags;
uint8_t spi_bits_left;
uint8_t spi_data_holder;
uint8_t spi_data_phase;
uint8_t spi_data_crc;




uint8_t crc8;
uint8_t EEMEM eeprom_data_block[4];
uint8_t tmp_buf[4];


uint16_t	default_timeout_count = 0;

/*****************************************************/
ISR (TIMER0_OVF_vect)
/*****************************************************/
{
	static uint16_t	ticks1000 = 0;
	static uint8_t	led_on_time = 0;

	if (PINC & _BV(PC0))
	{
		if(led_on_time)
		{
			led_on_time--;
		}
		else
		{
			pwm_val = 0;
		}	
	}
	else
	{
		pwm_val = led_cur_bright;
		led_on_time = 20; // 20 milliseconds.
	}

	if (++ticks1000 >= 1000) {
		ticks1000 = 0;
		// 1Hz timer.
		if (++default_timeout_count >= DEFAULT_TIMEOUT) {
			led_cur_bright = led_default_bright;
			default_timeout_count = 0;
		}
	}

	TCNT0 = (uint8_t) TMR0_RELOAD;



	if(PINC & _BV(PC1))
	{
		spi_flags &= ~_BV(F_SPI_CS_STATE);
		spi_data_phase = 0;
	}
	else
	{
		if(!(spi_flags & _BV(F_SPI_CS_STATE)))
		{
			spi_flags |= _BV(F_SPI_CS_STATE);
			spi_bits_left = 8;
			spi_data_holder = 0;
		}

		if(PINC & _BV(PC2))
		{
			spi_flags |= _BV(F_SPI_CLK_STATE);
		}
		else
		{
			if(spi_flags & _BV(F_SPI_CLK_STATE))
			{
				if(spi_data_holder & 0x01)
					PORTC |= _BV(PC4);
				else
					PORTC &= ~_BV(PC4);

				spi_data_holder >>= 1;

				if(PINC & _BV(PC3))
					spi_data_holder &= ~0x80;
				else
					spi_data_holder |= 0x80;
				
				spi_bits_left--;		
				if(!spi_bits_left)
				{	
					spi_bits_left = 8;

					switch(spi_data_phase)
					{
						case 0:
							spi_data_crc = _crc_ibutton_update(0, spi_data_holder);
							if(spi_data_holder == 0x63)
								spi_data_phase = 10;
							else if(spi_data_holder == 0x6C)
							{
								spi_data_holder = led_cur_bright;
								spi_data_crc = _crc_ibutton_update(spi_data_crc , spi_data_holder);
								spi_data_phase = 20;
							}

							break;

						case 10:
							led_new_bright = spi_data_holder;
							spi_data_crc = _crc_ibutton_update(spi_data_crc , spi_data_holder);
							spi_data_phase++;
							break;

						case 11:
							led_new_default_bright = spi_data_holder;
							spi_data_crc = _crc_ibutton_update(spi_data_crc , spi_data_holder);
							spi_data_phase++;
							break;

						case 12:
							spi_data_crc = _crc_ibutton_update(spi_data_crc , spi_data_holder);
							if(!spi_data_crc) {
								default_timeout_count = 0; // yes, saved the day.
								sysflags |= _BV(F_UPD_BRIGHT);
							}
							spi_data_phase = 0;
							break;

						case 20:
							spi_data_holder = led_default_bright;
							spi_data_crc = _crc_ibutton_update(spi_data_crc , spi_data_holder);
							spi_data_phase++;
							break;

						case 21:
							spi_data_holder = spi_data_crc;
							spi_data_phase++;
							break;

						case 22:
							spi_data_phase = 0;
							break;

						default:
							spi_data_phase = 0;
							break;
					}
								
				}

			}
			spi_flags &= ~_BV(F_SPI_CLK_STATE);
		}
	}

	wdt_reset();
	reset_seq_wdt();
}

/*****************************************************/
ISR (TIMER2_OVF_vect)
/*****************************************************/
{
	// Note: adding more code into this interrupt will cause time slot overflow and blinking of the light.
	// Do not add more code.
	TCNT2 = (uint8_t) TMR2_RELOAD;

	if(pwm_val)
	{
		if(pwm_cnt <= pwm_val)
		{
			PORTB |= _BV(PB1);
			PORTB |= _BV(PB2);
			PORTB |= _BV(PB3);
			PORTD |= _BV(PD3);
			PORTD |= _BV(PD5);
			PORTD |= _BV(PD6);
		}	
		else
		{
			PORTB &= ~_BV(PB1);
			PORTB &= ~_BV(PB2);
			PORTB &= ~_BV(PB3);
			PORTD &= ~_BV(PD3);
			PORTD &= ~_BV(PD5);
			PORTD &= ~_BV(PD6);

		}
	}
	else
	{
		PORTB &= ~_BV(PB1);
		PORTB &= ~_BV(PB2);
		PORTB &= ~_BV(PB3);
		PORTD &= ~_BV(PD3);
		PORTD &= ~_BV(PD5);
		PORTD &= ~_BV(PD6);
	}

	pwm_cnt++;
}

/*****************************************************/
void update_params(void)
/*****************************************************/
{	
	int i;
	uint8_t crc8 = 0;
	
	tmp_buf[0] = EEPROM_PCKT_SIGNATURE;
	tmp_buf[1] = led_default_bright;
	tmp_buf[2] = 0;
	for(i = 0; i < 3; i++)
		crc8 = _crc_ibutton_update(crc8, tmp_buf[i]);
	tmp_buf[3] = crc8;
	eeprom_write_block((void*)tmp_buf, (void*)eeprom_data_block, 4);
}

/*****************************************************/
void reload_params(void)
/*****************************************************/
{
	int i;
	uint8_t crc8 = 0;

	eeprom_read_block((void*)&tmp_buf, (void*)&eeprom_data_block, 4);
	for(i = 0; i < 4; i++)
		crc8 = _crc_ibutton_update(crc8, tmp_buf[i]);
	if(crc8)
	{
		led_cur_bright = led_default_bright = DEFAULT_BRIGHT;
		update_params();
	}
	else
	{
		if(tmp_buf[0] != EEPROM_PCKT_SIGNATURE)
		{
			led_cur_bright = 10;
			led_default_bright = DEFAULT_BRIGHT;
			update_params();
		}
		else
		{
			led_cur_bright = led_default_bright = tmp_buf[1];
		}
	}
}



/*****************************************************/
void io_Init(void)
/*****************************************************/
{

	PORTB = 0;
	DDRB = _BV(PB1) | _BV(PB2) | _BV(PB3);

	PORTC = _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	DDRC = _BV(PC4);

	PORTD = 0;
	DDRD = _BV(PD3) | _BV(PD5) | _BV(PD6);
	
	TCCR0 = _BV(CS02);
	TCNT0 = (uint8_t) TMR0_RELOAD;

		
	TCCR2 = _BV(CS21);
	TCNT2 = (uint8_t) TMR2_RELOAD;

	TIMSK = _BV(TOIE0) | _BV(TOIE2);
}



/*****************************************************/
int main(void)
/*****************************************************/
{
	wdt_disable();
	io_Init();
	usart_Init();
	reload_params();
	wdt_enable(WDTO_15MS);

	sysflags = 0;
	spi_flags = 0;
	spi_data_phase = 0;
	pwm_cnt = 0;
	pwm_val = 0;
	default_timeout_count = 0;
	
	sei();
	println_P(PSTR("BOOT"));
	while(1)
	{
		if(sysflags & _BV(F_UPD_BRIGHT))
		{
			sysflags &= ~_BV(F_UPD_BRIGHT);

			led_cur_bright = led_new_bright;
			println_hex08(PSTR("New bright "), led_new_bright);
			println_hex08(PSTR("New default"), led_default_bright);

			if(led_default_bright != led_new_default_bright)
			{
				led_default_bright = led_new_default_bright;
				update_params();			
			}
		}
	}
}
