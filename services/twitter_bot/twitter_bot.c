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

#include "services/twitter_bot/twitter_bot.h"
#include "services/clock/clock.h"

uint32_t last_post_time = 0;

void twitter_bot_init()
{
	DDRD |= 0b10000000;
}

void twitter_bot_periodic()
{
	uint32_t current_time = clock_get_time();
	if(current_time > (last_post_time) + POSTING_INTERVAL)
	{
		TW_BOT_DEBUG("it's time to post");
	}
}


/*
  -- Ethersex META --
  timer(50,twitter_bot_periodic())
  init(twitter_bot_init)
*/
