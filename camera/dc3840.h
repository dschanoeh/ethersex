/*
 * Copyright (c) 2009 by Stefan Siegl <stesie@brokenpipe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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


#ifndef DC3840_H
#define DC3840_H

/* DC3840 command set (0xFF, 0xFF, 0xFF, 0xnn) */
#define DC3840_CMD_INITIAL	1
#define DC3840_CMD_GET_PICTURE	4
#define DC3840_CMD_SNAPSHOT	5
#define DC3840_CMD_SAVE_DATA	6
#define DC3840_CMD_RESET	8
#define DC3840_CMD_POWER_OFF	9
#define DC3840_CMD_DATA		10
#define DC3840_CMD_SYNC		13
#define DC3840_CMD_ACK		14
#define DC3840_CMD_NAK		15
#define DC3840_CMD_COMPRESS	16
#define DC3840_CMD_LIGHT	18
#define DC3840_CMD_COLOR	23
#define DC3840_CMD_DIG_ZOOM	24


/* Initialize DC3840 communication. */
void dc3840_init (void);


/* (SYSLOG) debugging support */
#if 1
#include "../syslog/syslog.h"
/* FIXME We ought to put the format string in program space. */
# define DC3840_DEBUG(a...) syslog_sendf(a)
#else
# define DC3840_DEBUG(a...) do { } while(0)
#endif	/* SYSLOG debugging */

#endif	/* DC3840_H */