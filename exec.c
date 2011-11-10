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
#include <libdisarm/disarm.h>
#include <stdio.h>
#include <unistd.h>
#include "types.h"
#include "memory.h"
#include "pipeline.h"

void exec_mov(struct instruction *inst)
{
	r[inst->args.data_imm.rd] = inst->args.data_imm.imm;
}

void exec_swi(struct instruction *inst)
{
	int nr = inst->args.swi.imm;
	if (nr == 0) {
		r[1] = syscall(r[7], r[0], r[1], r[2], r[3], r[4]);
		printf("\t--> returned %d\n", r[1]);
	} else {
		fprintf(stderr, "Syscall %x not implemented\n", nr);
	}
}

void exec_bl(struct instruction *inst)
{
//	b(get_s32(inst->val._u32, 0, 24));
	b(inst->args.bl.off);
}

void exec(struct instruction *inst)
{
	if (inst->val._u32 == 0xffffffff)
		return;
	switch (inst->group) {
	case DA_GROUP_DATA_IMM:
		exec_mov(inst);
		break;
	case DA_GROUP_SWI:
		exec_swi(inst);
		break;
	case DA_GROUP_BL:
		exec_bl(inst);
		break;
	default:
		printf("\t--> NOT EXECUTED\n");
		break;
	}

	return;
}

