/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - rdram_detection_hack.c                                  *
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

#include "rdram_detection_hack.h"

#include <stdint.h>

#include "device/ri/rdram.h"
#include "device/si/cic.h"

/* HACK: force detected RDRAM size
 * This hack is triggered just before initial ROM loading (see pi_controller.c)
 *
 * Proper emulation of RI/RDRAM subsystem is required to avoid this hack.
 */
void force_detected_rdram_size_hack(struct rdram* rdram, const struct cic* cic)
{
    uint32_t address = (cic->version != CIC_X105)
        ? 0x318
        : 0x3f0;

    rdram->dram[address/4] = rdram->dram_size;
}

