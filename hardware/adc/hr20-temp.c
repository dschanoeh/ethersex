/*
 * Copyright (c) 2009 by Stefan Siegl <stesie@brokenpipe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "config.h"
#include "core/debug.h"

int16_t
hr20_temp_get (void)
{
  PIN_SET (TEMP_ENABLE);
  ADMUX = ADC_MUX_TEMP_SENSE | ADC_REF;

  /* Measure twice, i.e. wait for current to settle ... */
  ADCSRA |= _BV(ADSC);
  while (ADCSRA & _BV(ADSC));
  ADCSRA |= _BV(ADSC);
  while (ADCSRA & _BV(ADSC));

  uint16_t adc = ADC;
  debug_printf ("adc result: %d\n", adc);

  PIN_CLEAR (TEMP_ENABLE);
  return hr20_adc_to_temp (adc);
}
