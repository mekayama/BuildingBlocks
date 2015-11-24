/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"

void adc_setup (void)
{
	// Set the prescaler to clock/128 & enable ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
	ADMUX |= 1 << REFS0;
}

enum { MAX_POTS = 4 };

static const uint8_t g_potMap[MAX_POTS] =
{
	5, 3, 4, 6
};

static uint16_t g_pots[MAX_POTS];

uint16_t adc_read(uint8_t pot)
{
	ADMUX = g_potMap[pot];

	// Start the conversion
	ADCSRA |= (1 << ADSC);

	// Wait for it to finish
	while (ADCSRA & (1 << ADSC));

	uint16_t result = ADC;

	return result;
}

int main(void)
{
	spi_init();
	spi_disable();

	adc_setup();

	uint8_t pot = 0;
	for (;;)
	{
		// Check the SS pin, if it is low, send the data through SPI.
		if (!(PINB & 0x08))
		{
			spi_enable();
			for (uint8_t i=0; i<MAX_POTS; ++i)
			{
				spi_transfer16(g_pots[i]);
			}
			spi_disable();
		}

		g_pots[pot] = adc_read(pot);
		pot = (pot+1) % MAX_POTS;
	}
}
