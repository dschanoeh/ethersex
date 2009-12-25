/*
*
* Copyright (c) 2009 by Jan-Niklas Meier <dschanoeh@googlemail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 3
* of the License, or (at your option) any later version.
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

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include "core/eeprom.h"
#include "services/twitter_bot/twitter_bot.h"
#include "services/clock/clock.h"
#include "hardware/onewire/onewire.h"
#include "core/bit-macros.h"
#include "core/eeprom.h"




uint32_t last_post_time = 0;
uint8_t twitter_bot_error = 0;

struct ow_rom_code_t rom, *romptr;

void twitter_bot_init()
{
	TW_BOT_DEBUG("searching for temperature sensor\n");
	int8_t ret;
	ret = ow_search_rom(1);
	
	do
	{
		if(ret == 0)
		{
			twitter_bot_error = 1;
			TW_BOT_DEBUG("no temperature device has been found\n");
			break;
		}
		else if(ret == -1)
		{
			twitter_bot_error = 1;
			TW_BOT_DEBUG("there is no device on the bus\n");
			break;
		}
		else
		{
			if(ow_temp_sensor(&(ow_global.current_rom)))
			{
				TW_BOT_DEBUG("found sensor\n");
				rom = ow_global.current_rom;
				break;
			}
			ret = ow_search_rom(0);
		}
		ret = ow_search_rom(1);
	}
	while(1);

	
}

void twitter_bot_periodic()
{
	uint32_t current_time = clock_get_time();
	int16_t ret;

	/*check if we need to post a new message*/
	if(current_time > (last_post_time) + POSTING_INTERVAL)
	{
		TW_BOT_DEBUG("it's time to post\n");

		/*check if this is really a temperature sensor*/
		if(ow_temp_sensor(&rom))
		{
			
			uint8_t sreg = SREG;
    			cli();
			ret = ow_temp_start_convert_wait(&rom);
			SREG = sreg;

			if(ret==1)
			{
				uint8_t sreg = SREG;
				cli();
				struct ow_temp_scratchpad_t sp;
				ret = ow_temp_read_scratchpad(&rom, &sp);
				SREG = sreg;

				if(ret==1)
				{
					uint16_t t = ow_temp_normalize(&rom, &sp);
					TW_BOT_DEBUG("received temperature %d.%d C\n",HI8(t), LO8(t) > 0 ? 5 : 0);
					char message[140];
					sprintf(&message,"current temperature is %d.%d",HI8(t), LO8(t) > 0 ? 5 : 0);
					twitter_send(&message);

					last_post_time = current_time;
				}
				else
				{
					TW_BOT_DEBUG("could not read scratchpad\n");
				}
			}
			else
			{
				TW_BOT_DEBUG("converting failed\n");
			}

		}
		else
		{
			TW_BOT_DEBUG("this is no temperature sensor\n");
		}	

		


	}
}


/*
  -- Ethersex META --
  timer(50,twitter_bot_periodic())
  init(twitter_bot_init)
*/
