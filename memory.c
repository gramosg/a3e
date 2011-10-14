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

#define REG_N	16	/* Size of register bank */

u8 m[MEMSIZE];		/* Memory map */
u32 r[REG_N];		/* Registers */
u32 *pc = &r[15];	/* Program counter (r15) */

void reg_dump(void)
{
	int i;

	for (i = 0; i < REG_N; i++) {
		printf("\tr%02d=0x%08x", i, r[i]);
		if ((i+1) % 4 == 0)
			putchar('\n');
	}
}

void mem_dump(int offset, int len)
{
	int i;

	printf("\t/--------\\\n");
	for (i = 0; i < len; i+=4)
		printf("\t|%08x|\t0x%08x\n", *(u32 *)(m+offset+i), (offset+i));
	printf("\t\\--------/\n");
}

inline void show_status(void)
{

	printf("\n[REGISTERS]\n");
	reg_dump();
	printf("[MEMORY]\n");
	mem_dump(0, 40);
}

