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
#include <stdlib.h>
#include <libdisarm/disarm.h>
#include "types.h"

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

