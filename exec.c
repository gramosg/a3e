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

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include "types.h"
#include "memory.h"

void exec_mov(struct instruction *inst)
{
	r[get_u32(inst->val._u32, 8, 8)] = get_u32(inst->val._u32, 0, 8);
}

void exec_swi(struct instruction *inst)
{
	int nr = get_s32(inst->val._u32, 0, 24);
	if (nr == 0) {
		r[1] = syscall(r[7], r[0], r[1], r[2], r[3], r[4]);
		printf("\t--> returned %d\n", r[1]);
	} else {
		fprintf(stderr, "Syscall %x not implemented\n", nr);
	}
}

