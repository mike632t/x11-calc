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
 * 26 Oct 21         - Added definition for processor_load() - MT
 * 28 Nov 21         - Made the trace flag a processor property - MT
 * 11 Dec 21         - Started to add additional instructions and processor
 *                     flags to support the HP67 - MT
 * 21 Dec 21         - Added the READY flag to allow the HP67 to behave  as
 *                     if it has continuous memory - MT
 *
 */

#ifndef REGISTERS
#include "x11-calc.h"

#define REGISTERS       8              /* A, B, C(X), D(Y), E(Z), F(T), M, N(M2) */
#define REG_SIZE        14
#define EXP_SIZE        3              /* Two digit exponent plus a sign digit */
#define STACK_SIZE      2
#define STATUS_BITS     16

#define A_REG           0
#define B_REG           1
#define C_REG           2
#define Y_REG           3
#define Z_REG           4
#define T_REG           5
#define M_REG           6
#define N_REG           7

#define MODE            0
#define CARRY           1
#define PREV_CARRY      2
#define DELAYED_ROM     3

#define BANK_SWITCH     4
#define DISPLAY_ENABLE  5
#define TIMER           8
#define FLAGS           9

#if defined(HP67)
#define MERGE           0              /* Merge flag (F0) */
#define PAUSE           1
#define DISPLAY         2
#define CARD            3              /* Card inserted */

#define ANYKEY          4              /* Key pressed */
#define MOTOR           5              /* Motor on */
#define FUNCTION        6              /* Default function flag */
#define READY           7

#define STATES          8
#endif

typedef struct {
   char id;
   unsigned int nibble[REG_SIZE];
} oregister;

typedef struct {
   oregister *reg[REGISTERS];          /* Registers */
   oregister *mem[MEMORY_SIZE];        /* Memory registers */
   int *rom;
   int first;
   int last;
#if defined(HP67)
   unsigned char crc[STATES];          /* Card reader states */
#endif
   unsigned char flags[FLAGS];         /* Processor flags*/
   unsigned char status[STATUS_BITS];  /* Status (S0 - S15) */
   unsigned int stack[STACK_SIZE];     /* Call stack */
   unsigned int opcode;                /* Last opcode */
   unsigned int bank;                  /* Bank number */
   unsigned int pc;                    /* Program counter */
   unsigned int sp;                    /* Stack pointer */
   unsigned int f;                     /* F register */
   unsigned int p;                     /* P register */
   unsigned int addr;                  /* Address register */
   unsigned int base;                  /* Current arithmetic base */
   unsigned int code;                  /* Key code */
   unsigned int rom_number;            /* Delayed ROM number */
   unsigned char keypressed;           /* Key pressed */
   unsigned char select;               /* Save switch state */
   unsigned char enabled;              /* Enabled */
   unsigned char trace;                /* Trace flag */
   unsigned char step;                 /* Step flag */
} oprocessor;

oprocessor *h_processor_create(int *h_rom);

void v_processor_reset(oprocessor *h_processor);

void v_processor_load(oprocessor *h_processor, char *s_pathname);

void v_processor_restore(oprocessor *h_processor);

void v_processor_save(oprocessor *h_processor);

void v_fprint_registers(FILE *h_file, oprocessor *h_procesor);

void v_fprint_memory(FILE *h_file, oprocessor *h_procesor);

void v_processor_tick(oprocessor *h_procesor);
#endif
