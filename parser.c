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

void printi(struct instruction *inst)
{
	printf("%02x %02x %02x %02x\t\t", inst->val._byte[0], inst->val._byte[1],
			inst->val._byte[2], inst->val._byte[3]);
	switch (inst->type) {
	case MOV:
		printf("mov\tr%02x, #%x\n", get_u32(inst->val._u32, 8, 8),
				get_u32(inst->val._u32, 0, 8));
		break;
	case SWI:
		printf("swi\t%x\n", get_s32(inst->val._u32, 0, 24));
		break;
	case B:
		printf("b\t[pc + #%d]\n", get_s32(inst->val._u32, 0, 24));
		break;
	case BYE:
		printf(".exit\n");
		break;
	case UNK:
		printf("UNKNOWN\n");
		break;
	case MUL:
	case LML:
	case SWP:
	case LDB:
	case LDM:
	case BXC:
	case COP:
	case NONE:
		printf("Instruction (%d) not implemented\n", inst->type);
		break;
	}
}

void parse_type(struct instruction *inst)
{
		if (inst->val._u32 == 0xffffffff)
			inst->type = BYE;
		else if (get_u32(inst->val._u32, 25, 3) == 0x5)
			inst->type = B;
		else if (get_u32(inst->val._u32, 24, 4) == 0xf)
			inst->type = SWI;
		else if (get_u32(inst->val._u32, 26, 2) == 0x1)
			inst->type = LDB;
		else if (get_u32(inst->val._u32, 22, 6) == 0x0)
			inst->type = MUL;
		else if (get_u32(inst->val._u32, 23, 5) == 0x1)
			inst->type = LML;
		else if (get_u32(inst->val._u32, 23, 5) == 0x2)
			inst->type = SWP;
		else if (get_u32(inst->val._u32, 25, 3) == 0x4)
			inst->type = LDM;
		else if (get_u32(inst->val._u32, 24, 4) == 0x1)
			inst->type = BXC;
		else if (get_u32(inst->val._u32, 26, 2) == 0x0)
			inst->type = MOV;
		else if (get_u32(inst->val._u32, 24, 4) == 0xe)
			inst->type = COP;
		else
			inst->type = UNK;
}

void parse_cond(struct instruction *inst)
{
	/* In every instruction, condition is coded in the last 4 bits */
	inst->cond = get_u32(inst->val._u32, 28, 4);
}

void parse(struct instruction *inst)
{

	parse_cond(inst);
	parse_type(inst);
}

