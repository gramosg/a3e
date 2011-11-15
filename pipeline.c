/*
 * a3e - ARMv7 emulator
 * Copyright (C) 2011 Guillermo Ramos <0xwille@gmail.com>
 *
 * This file is part of a3e
 *
 * a3e is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * a3e is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with a3e.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "types.h"
#include "memory.h"

#define STAGES	3


/*
 * stage 0 -> Pipeline is fetching but not decoding nor executing
 * stage 1 -> Pipeline is fetching and decoding, but not executing
 * stage 2 -> Pipeline is fetching, decoding and executing. Ideal stage.
 */
int stage = 0;


/*
 * Advance one stage in pipeline status
 *
 * This is done to prevent the main program from executing invalid instructions
 * when a jump/branch has been taken or the program is first run
 */
void do_cycle(void)
{
	*pc += 4;
	if (stage < STAGES-1)
		stage++;
}


/*
 * This auxiliar function is executed every time a cycle is wasted waiting for
 * the pipeline while it is fetching and decoding
 */
int wait_pipe(void)
{
	int ready = (stage == STAGES-1);
	if (!ready) {
		fprintf(stdout, "\t(waiting for pipeline: pc=%d, stage=%d)\n", *pc, stage);
		do_cycle();
	}
	return ready;
}


/*
 * Jumps to absolute address
 */
void jmp(u32 new_pc)
{
	*pc = new_pc;
	stage = 0;
}


/*
 * Adds 'offset' instructions to current pc
 */
void b(s32 offset)
{
	da_addr_t off = da_instr_branch_target(offset, *pc);
	jmp(off-8);
}


/*
 * Instruction being currently executed: pc-8
 */
u32 cur_inst(void)
{
	return *pc-8;
}
