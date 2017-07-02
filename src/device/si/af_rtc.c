/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - af_rtc.c                                                *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2014 Bobby Smiles                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "af_rtc.h"
#include "pif.h"

#include "api/callbacks.h"
#include "api/m64p_types.h"
#include "backends/clock_backend.h"

#include <time.h>

static unsigned char byte2bcd(int n)
{
    n %= 100;
    return ((n / 10) << 4) | (n % 10);
}

static void time2data(uint8_t* data, time_t now)
{
    const struct tm* tm = localtime(&now);

    data[0] = byte2bcd(tm->tm_sec);
    data[1] = byte2bcd(tm->tm_min);
    data[2] = 0x80 + byte2bcd(tm->tm_hour);
    data[3] = byte2bcd(tm->tm_mday);
    data[4] = byte2bcd(tm->tm_wday);
    data[5] = byte2bcd(tm->tm_mon + 1);
    data[6] = byte2bcd(tm->tm_year);
    data[7] = byte2bcd(tm->tm_year / 100);
}

static void update_rtc(struct af_rtc* rtc)
{
    /* update rtc->now */
    time_t now = clock_get_time(rtc->clock);
    rtc->now += now - rtc->last_update_rtc;
    rtc->last_update_rtc = now;
}



void init_af_rtc(struct af_rtc* rtc,
                 struct clock_backend* clock)
{
    rtc->clock = clock;
}

void poweron_af_rtc(struct af_rtc* rtc)
{
    /* blocks 1&2 are read/write, timer is activated */
    rtc->control = 0x0200;

    rtc->now = 0;
    rtc->last_update_rtc = 0;
}

void af_rtc_status_command(struct af_rtc* rtc, uint8_t* cmd)
{
    /* AF-RTC status query */
    uint8_t* const type = &cmd[3];
    uint8_t* const status = &cmd[5];

    type[0] = (uint8_t)(PIF_PDT_AF_RTC >> 0);
    type[1] = (uint8_t)(PIF_PDT_AF_RTC >> 8);
    *status =  0x00;
}

void af_rtc_read_command(struct af_rtc* rtc, uint8_t* cmd)
{
    /* read RTC block */
    uint8_t block = cmd[3];
    uint8_t* data = &cmd[4];
    uint8_t* status = &cmd[12];

    switch (block)
    {
    case 0:
        data[0] = (uint8_t)(rtc->control >> 0);
        data[1] = (uint8_t)(rtc->control >> 8);
        *status = 0x00;
        break;
    case 1:
        DebugMessage(M64MSG_ERROR, "AF-RTC reading block 1 is not implemented !");
        break;
    case 2:
        update_rtc(rtc);
        time2data(data, rtc->now);
        *status = 0x00;
        break;

    default:
        DebugMessage(M64MSG_ERROR, "AF-RTC read invalid block: %u", block);
    }
}

void af_rtc_write_command(struct af_rtc* rtc, uint8_t* cmd)
{
    /* write RTC block */
    uint8_t block = cmd[3];
    const uint8_t* data = &cmd[4];
    uint8_t* status = &cmd[12];

    switch (block)
    {
    case 0:
        rtc->control = (data[1] << 8) | data[0];
        *status = 0x00;
        break;
    case 1:
        /* block 1 read-only when control[0] is set */
        if (rtc->control & 0x01) {
            break;
        }
        DebugMessage(M64MSG_ERROR, "AF-RTC writing block 1 is not implemented !");
        break;
    case 2:
        /* block 2 read-only when control[1] is set */
        if (rtc->control & 0x02) {
            break;
        }

        /* TODO: implement block 2 writes */
        DebugMessage(M64MSG_ERROR, "AF-RTC writing block 2 is not implemented !");
        break;

    default:
        DebugMessage(M64MSG_ERROR, "AF-RTC write invalid block: %u", block);
    }
}

