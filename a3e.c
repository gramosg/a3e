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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "types.h"
#include "memory.h"
#include "exec.h"
#include "parser.h"


static void usage(char *progname)
{
	printf("Usage: %s <file>\n", progname);
	exit(0);
}

int main(int argc, char** argv)
{
	int codefile;
	u16 codelen;

	void *map;
	struct instruction cur;

	if (argc < 2)
		usage(argv[0]);

	codefile = open(argv[1], O_RDONLY);

	if ((codelen = lseek(codefile, 0, SEEK_END)) % 4 != 0) {
		printf("WARNING: file not word-aligned, filling with zeros ('\\0')\n");
		codelen = codelen - (codelen % 4) + 4;
	}

	printf("Memory size: %d bytes\n", MEMSIZE);
	if (MEMSIZE < codelen + 4) {
		fprintf(stderr, "ERROR: Not enough memory\n");
		return -1;
	}

	map = (char *)mmap(NULL, codelen, PROT_READ, MAP_SHARED, codefile, 0);
	memcpy(m, map, codelen);			/* copy file into memory */
	memset(m + codelen, 0xff, 4);		/* put .exit at EOF (0xffffffff) */

	munmap(map, codelen);
	close(codefile);

	printf("\nGo go go...\n");
	while (1) {
		memcpy(cur.val._byte, m+(*pc), 4);

		cur.cond = get_u32(cur.val._u32, 28, 4);

		if (get_u32(cur.val._u32, 0, 32) == 0xffffffff) {
			cur.type = BYE;
			printi(&cur);
			show_status();
			return 0;
		}

		if (get_u32(cur.val._u32, 25, 3) == 0x5)
			cur.type = B;
		else if (get_u32(cur.val._u32, 24, 4) == 0xf)
			cur.type = SWI;
		else if (get_u32(cur.val._u32, 26, 2) == 0x1)
			cur.type = LDB;
		else if (get_u32(cur.val._u32, 22, 6) == 0x0)
			cur.type = MUL;
		else if (get_u32(cur.val._u32, 23, 5) == 0x1)
			cur.type = LML;
		else if (get_u32(cur.val._u32, 23, 5) == 0x2)
			cur.type = SWP;
		else if (get_u32(cur.val._u32, 25, 3) == 0x4)
			cur.type = LDM;
		else if (get_u32(cur.val._u32, 24, 4) == 0x1)
			cur.type = BXC;
		else if (get_u32(cur.val._u32, 26, 2) == 0x0)
			cur.type = MOV;
		else if (get_u32(cur.val._u32, 24, 4) == 0xe)
			cur.type = COP;
		else
			cur.type = UNK;

		// Show instruction info
//		parse(&cur);
		printi(&cur);

		// Execute instruction
		switch (cur.type) {
		case MOV:
			exec_mov(&cur);
			break;
		case SWI:
			exec_swi(&cur);
			break;
		case B:
//			*pc += ...;
			break;
		case BYE:
		case UNK:
			break;
		case MUL:
		case LML:
		case SWP:
		case LDB:
		case LDM:
		case BXC:
		case COP:
			break;
		}

		*pc += 4;
	}

	show_status();

	return 0;
}

