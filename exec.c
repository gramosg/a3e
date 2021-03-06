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

void parse(struct instruction *inst)
{
	da_instr_t i;

	if (inst->val._u32 == 0xffffffff) {
		printf(".exit\n");
		return;
	}

	da_instr_parse(&i, inst->val._u32, 0);
	da_instr_parse_args(&inst->args, &i);
	printf("%x\t", inst->val._u32);
	da_instr_fprint(stdout, &i, &inst->args, 0);

	inst->group = i.group;
	inst->cond = da_instr_get_cond(&i);

	putchar('\n');
}

inline void not_implemented(char *where, da_data_op_t op)
{
	fprintf(stdout, "\tOp #%d not implemented in inst. type %s\n", op, where);
}

void exec_data_imm(da_args_data_imm_t *args)
{
	if (args->op == DA_DATA_OP_MOV || args->op == DA_DATA_OP_MVN)
		r[args->rd] = args->imm;
	else
		not_implemented("data_imm", args->op);
}

void exec_data_imm_sh(da_args_data_imm_sh_t *args)
{
//	printf("rd:%d rn:%d rm:%d sh:%d sha:%d\n", args->rd, args->rn, args->rm,
//			args->sh, args->sha);
	if (args->op == DA_DATA_OP_ADD)
		r[args->rd] = r[args->rn] + r[args->rm];
	else if (args->op == DA_DATA_OP_SUB)
		r[args->rd] = r[args->rn] - r[args->rm];
	else
		not_implemented("data_imm_sh", args->op);
}

void exec_swi(da_args_swi_t *args)
{
	int nr = args->imm;
	if (nr == 0) {
		r[1] = syscall(r[7], r[0], r[1], r[2], r[3], r[4]);
		printf("\t--> returned %d\n", r[1]);
	} else {
		fprintf(stderr, "Software interrupt #%x not implemented\n", nr);
	}
}

void exec_bl(da_args_bl_t *args)
{
	b(args->off - 1);
}

int exec(struct instruction *inst)
{
	da_instr_args_t args;

	parse(inst);
	args = inst->args;

	if (inst->val._u32 == 0xffffffff)	{ // exit
		return -1;
	}

	switch (inst->group) {
	case DA_GROUP_DATA_IMM:
//		printf("Entering data_imm\n");
		exec_data_imm(&args.data_imm);
		break;
	case DA_GROUP_SWI:
//		printf("Entering swi\n");
		exec_swi(&args.swi);
		break;
	case DA_GROUP_BL:
//		printf("Entering group_bl\n");
		exec_bl(&args.bl);
		break;;
	case DA_GROUP_DATA_IMM_SH:
//		printf("Entering data_imm_sh\n");
		exec_data_imm_sh(&args.data_imm_sh);
		break;
	default:
		printf("\t--> NOT EXECUTED\n");
		break;
	}

	return 0;
}

