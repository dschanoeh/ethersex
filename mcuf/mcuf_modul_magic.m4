dnl This m4 script uses quite a few divert levels, these are essentially:
dnl   1: char array in program space
dnl   2: the modul list
dnl   4: function list trailer
dnl   5: (optional) function implementations 
dnl
dnl ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dnl
dnl   Copyright (c) 2009 by Stefan Riepenhausen <rhn@gmx.net>
dnl   Copyright (c) 2007,2009 by Christian Dietrich <stettberger@dokucode.de>
dnl   Copyright (c) 2007,2008 by Stefan Siegl <stesie@brokenpipe.de>
dnl  
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License version 2 as
dnl   published by the Free Software Foundation.
dnl  
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl  
dnl   You should have received a copy of the GNU General Public License
dnl   along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl  
dnl   For more information on the GPL, please go to:
dnl   http://www.gnu.org/copyleft/gpl.html
dnl
divert(0)dnl
/* This file has been generated by alias_defs.m4 automatically.
   Please do not modify it, edit the m4 scripts instead. */
divert(1)dnl
/* Char array definitions follow */
divert(2)dnl

/* Definition of alias pointer array follows */
const struct mcuf_modul_t PROGMEM mcuf_display_modules[] = {
divert(-1)dnl
define(`modul_counter', 0)

define(`modul', `dnl
divert(1)dnl
`//void $1(void);
const char PROGMEM mcuf_modul_$1_title[] = $2;'

divert(2)	`{ $1 , mcuf_modul_$1_title },'

define(`modul_counter', incr(modul_counter))dnl
divert(-1)')

define(`modul_ifdef', `dnl
divert(1)#ifdef $1
divert(2)#ifdef $1
divert(-1)')

define(`modul_ifndef', `dnl
divert(1)#ifndef $1
divert(2)#ifndef $1
divert(-1)')

define(`modul_else', `dnl
divert(1)#else
divert(2)#else
divert(3)#else
divert(-1)')

define(`modul_endif', `dnl
divert(1)#endif
divert(2)#endif
divert(-1)')

divert(4)dnl
        { NULL, "" }
};
divert(-1)dnl
dnl yippie, we're done!