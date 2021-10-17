/*
 * x11-calc-cpu.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2020   MT
 *
 * Simulates the ACT processor.
 *
 * Contains  the type definitions and functions definitions used to emulate
 * the CPU microcode.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful, but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 10 Sep 20         - Initial version - MT
 * 24 Aug 21         - Added  properties for the p and data  registers, and
 *                     an  array of flags to keep track of things like  the
 *                     arithmetic mode and instruction tracing - MT
 * 5 Sep 21          - Changed  first and last into signed integers  (fixed
 *                     segmentation fault in register left shift)  - MT
 *                   - Added a code and key pressed properties to store the
 *                     key  code of the key and the state of the actual key
 *                     (necessary as clearing status bit 15 when the key is
 *                     released does NOT work!) - MT
 * 30 Sep 21         - Added properties for enable and select - MT
 */

#ifndef REGISTERS
#include "x11-calc.h"

#define REGISTERS      8     /* A, B, C(X), D(Y), E(Z), F(T), M, N(M2) */
#define FLAGS          10
#define REG_SIZE       14
#define EXP_SIZE       3     /* Two digit exponent plus a sign digit */
#define STACK_SIZE     2

#define A_REG          0
#define B_REG          1
#define C_REG          2
#define Y_REG          3
#define Z_REG          4
#define T_REG          5
#define M_REG          6
#define N_REG          7

#define MODE           0
#define CARRY          1
#define PREV_CARRY     2
#define DELAYED_ROM    3
#define BANK_SWITCH    4
#define DISPLAY_ENABLE 5
#define TIMER          8
#define TRACE          9

typedef struct {
   char id;
   unsigned int nibble[REG_SIZE];
} oregister;

typedef struct {
   oregister *reg[REGISTERS];       /* Registers */
   oregister *mem[MEMORY_SIZE];     /* Memory registers */
   int *rom;
   int first;
   int last;
   unsigned int rom_number;         /* ROM number */
   unsigned int delayed_rom_number; /* Delayed ROM number */
   unsigned char flags[FLAGS];      /* Processor flags + TRACE flag*/
   unsigned char status[16];        /* Status (S0 - S15) */
   unsigned int stack[STACK_SIZE];  /* Call stack */
   unsigned int pc;                 /* Program counter */
   unsigned int sp;                 /* Stack pointer */
   unsigned int p;                  /* P register */
   unsigned int f;                  /* F register */
   unsigned int addr;               /* Address register */
   unsigned int base;               /* Current arithmetic base */
   unsigned int code;               /* Key code */
   unsigned char keypressed;        /* Key pressed */
   unsigned char select;            /* Save switch state */
   unsigned char enabled;           /* Enabled */
} oprocessor;

oprocessor *h_processor_create(int *h_rom);

void v_processor_reset(oprocessor *h_processor);

void v_processor_save(oprocessor *h_processor);

void v_processor_tick(oprocessor *h_procesor);

void v_reg_load(oregister *h_register, ...);

void v_fprint_state(FILE *h_file, oprocessor *h_procesor);
#endif
