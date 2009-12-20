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

#define POSTING_INTERVAL 300 //in seconds

void twitter_bot_init();
void twitter_bot_periodic();

#include "config.h"
#ifdef DEBUG_TWITTER_BOT
# include "core/debug.h"
# define TW_BOT_DEBUG(a...)  debug_printf("twitter-bot: " a)
#else
# define TW_BOT_DEBUG(a...)
#endif

