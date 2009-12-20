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
#include "core/eeprom.h"
#include "services/twitter_bot/twitter_bot.h"
#include "services/clock/clock.h"
#include "hardware/onewire/onewire.h"
#include "core/bit-macros.h"

struct ow_rom_code_t rom, *romptr;


uint32_t last_post_time = 0;

void twitter_bot_init()
{

	//generate rom code
	uint8_t *addr = rom->bytewise;
	uint8_t end;
	int16_t ret = sscanf_P("1028080102080064", PSTR("%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%c"),
        	addr+0, addr+1, addr+2, addr+3,
        	addr+4, addr+5, addr+6, addr+7,
        	&end);
}

void twitter_bot_periodic()
{
	uint32_t current_time = clock_get_time();
	int16_t ret;

	if(current_time > (last_post_time) + POSTING_INTERVAL)
	{
		TW_BOT_DEBUG("it's time to post\n");

		if(ow_temp_sensor(&rom))
		{
			romptr = (ret < 0) ? NULL : &rom;
			uint8_t sreg = SREG;
    			cli();
			ret = ow_temp_start_convert_wait(romptr);
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
					uint16_t temp = ow_temp_normalize(&rom, &sp);
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
