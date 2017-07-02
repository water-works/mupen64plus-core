/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - cp1.h                                                   *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2002 Hacktarux                                          *
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

#ifndef M64P_DEVICE_R4300_CP1_H
#define M64P_DEVICE_R4300_CP1_H

#include <stdint.h>
#include "new_dynarec/new_dynarec.h" /* for NEW_DYNAREC_ARM */

struct cp1
{
    int64_t regs[32];

#if NEW_DYNAREC != NEW_DYNAREC_ARM
/* ARM dynarec uses a different memory layout */
    uint32_t fcr0;
    uint32_t fcr31;

    float* regs_simple[32];
    double* regs_double[32];
#endif

    /* This is the x86 version of the rounding mode contained in FCR31.
     * It should not really be here. Its size should also really be uint16_t,
     * because FLDCW (Floating-point LoaD Control Word) loads 16-bit control
     * words. However, x86/gcop1.c and x86-64/gcop1.c update this variable
     * using 32-bit stores. */
    uint32_t rounding_mode;

#if NEW_DYNAREC == NEW_DYNAREC_ARM
/* ARM dynarec uses a different memory layout */
    struct new_dynarec_hot_state* new_dynarec_hot_state;
#endif
};

void init_cp1(struct cp1* cp1, struct new_dynarec_hot_state* new_dynarec_hot_state);
void poweron_cp1(struct cp1* cp1);

int64_t* r4300_cp1_regs(struct cp1* cp1);
float** r4300_cp1_regs_simple(struct cp1* cp1);
double** r4300_cp1_regs_double(struct cp1* cp1);

uint32_t* r4300_cp1_fcr0(struct cp1* cp1);
uint32_t* r4300_cp1_fcr31(struct cp1* cp1);

void shuffle_fpr_data(struct cp1* cp1, uint32_t oldStatus, uint32_t newStatus);
void set_fpr_pointers(struct cp1* cp1, uint32_t newStatus);

void update_x86_rounding_mode(struct cp1* cp1);

#endif /* M64P_DEVICE_R4300_CP1_H */

