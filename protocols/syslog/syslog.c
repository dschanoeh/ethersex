/*
 *
 * Copyright (c) 2007 by Christian Dietrich <stettberger@dokucode.de>
 * Copyright (c) 2008 by Stefan Siegl <stesie@brokenpipe.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include <avr/pgmspace.h>
#include <stdarg.h>

#include "protocols/uip/uip.h"
#include "config.h"
#include "core/debug.h"
#include "protocols/uip/uip_neighbor.h"
#include "protocols/uip/uip_router.h"
#include "syslog.h"
#include "syslog_net.h"


static char send_buffer[MAX_DYNAMIC_SYSLOG_BUFFER + 1];
extern uip_udp_conn_t *syslog_conn;
static struct SyslogCallbackCtx syslog_callbacks[SYSLOG_CALLBACKS];

static void syslog_send_cb_P(void *data) 
{
  strcpy_P(uip_appdata, data);
  uip_udp_send(strlen_P(data));
}

static void syslog_send_cb(void *data) 
{
  char *p = data;

  strcpy(uip_appdata, p);
  uip_udp_send(strlen(p));

  if (p == send_buffer)
    p[0] = 0;
}


uint8_t 
syslog_send_P(PGM_P message)
{
  return syslog_insert_callback(syslog_send_cb_P, (void *)message);
}

uint8_t 
syslog_send(const char *message)
{
  uint16_t offset = strlen (send_buffer);

  if (strlen (message) + offset + 1 > MAX_DYNAMIC_SYSLOG_BUFFER)
    return 0;

  strcat (send_buffer + offset, message);

  if (! offset)
    /* If there used to be a few bytes in the buffer already, we don't have
       to insert the callback, since it should have been added already. */
    return syslog_insert_callback(syslog_send_cb, (void *)send_buffer);
  return 1;
}

uint8_t 
syslog_sendf(const char *message, ...)
{
  va_list va;
  uint16_t offset = strlen (send_buffer);

  va_start(va, message);
  vsnprintf(send_buffer + offset, MAX_DYNAMIC_SYSLOG_BUFFER - offset, 
            message, va);
  va_end(va);

  send_buffer[MAX_DYNAMIC_SYSLOG_BUFFER] = 0;

  if (!offset)
    return syslog_insert_callback(syslog_send_cb, (void *)send_buffer);
  return 1;
}

uint8_t 
syslog_send_ptr(void *message)
{
  return syslog_insert_callback(syslog_send_cb, message);
}


void
syslog_flush (void)
{

  /* FIXME: use perhaps router to determine target Stack */
  uip_stack_set_active(STACK_ENC);

  if (syslog_check_cache ())
    return;			/* ARP cache not ready, don't send request
				   here (would flood, wait for poll event). */

  uip_slen = 0;
  uip_appdata = uip_sappdata = uip_buf + UIP_IPUDPH_LEN + UIP_LLH_LEN;

  for (uint8_t i = 0; i < SYSLOG_CALLBACKS; i++)
    if (syslog_callbacks[i].callback != NULL) {
      syslog_callbacks[i].callback(syslog_callbacks[i].data);
      syslog_callbacks[i].callback = NULL;
      break;
    }

  if (! uip_slen)
    return;

  uip_udp_conn = syslog_conn;
  uip_process (UIP_UDP_SEND_CONN);
  router_output ();

  uip_slen = 0;
}


uint8_t
syslog_insert_callback(syslog_callback_t callback, void *data)
{
  uint8_t i;
  for (i = 0; i < SYSLOG_CALLBACKS; i++)
    if (syslog_callbacks[i].callback == NULL) {
      syslog_callbacks[i].callback = callback;
      syslog_callbacks[i].data = data;
      return 1;
    }
  return 0; /* No empty callback found */
}


uint8_t
syslog_check_cache(void)
{
  uip_ipaddr_t ipaddr;

#ifdef IPV6_SUPPORT

  if(memcmp(syslog_conn->ripaddr, uip_hostaddr, 8))
    /* Remote address is not on the local network, use router */
    uip_ipaddr_copy(&ipaddr, uip_draddr);
  else
    /* Remote address is on the local network, send directly. */
    uip_ipaddr_copy(&ipaddr, syslog_conn->ripaddr);

  if (uip_ipaddr_cmp(&ipaddr, &all_zeroes_addr))
    return 1;	     /* Cowardly refusing to send IPv6 packet to :: */

  if(uip_neighbor_lookup (ipaddr))
    return 0;

#else  /* IPV4_SUPPORT */

  if(!uip_ipaddr_maskcmp(syslog_conn->ripaddr, uip_hostaddr, uip_netmask))
    /* Remote address is not on the local network, use router */
    uip_ipaddr_copy(&ipaddr, uip_draddr);
  else
    /* Remote address is on the local network, send directly. */
    uip_ipaddr_copy(&ipaddr, syslog_conn->ripaddr);

#ifdef ETHERNET_SUPPORT
  /* uip_arp_lookup returns a pointer if the mac is in the arp cache */
  if(uip_arp_lookup (ipaddr))
#endif
    return 0;

#endif

  return 1;
}

/*
  -- Ethersex META --
  header(protocols/syslog/syslog.h)
  mainloop(syslog_flush)
*/
