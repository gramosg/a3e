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
void stage_up(void)
{
	*pc += 4;
	if (stage < STAGES-1)
		stage++;
}


/*
 * This auxiliar function is executed every time a cycle is wasted waiting for
 * the pipeline while it is fetching and decoding
 */
void wait_pipe(void)
{
	fprintf(stdout, "\t(waiting for pipeline: current stage %d)\n", stage);
	stage_up();
}


/*
 * Set what instruction is pointing each of the pipeline stages
 */
//void update_pipe(void)
//{
//	executing = *pc-8;
//}


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
void b(u32 offset)
{
	jmp(*pc + 4*offset);
}


/*
 * Add 4 to pc and update pipeline in consequence
 */
//void next_inst(void)
//{
//	*pc += 4;
//	stage_up();
//}


/*
 * Instruction being currently executed: pc-8
 */
u32 cur_inst(void)
{
	return *pc-8;
}


/*
 * Check wether the pipe has fetched and decoded the current instruction
 */
int pipe_ready(void)
{
	return stage == STAGES-1;
}

