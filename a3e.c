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

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
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
#include "pipeline.h"


static void warning(char *s)
{
	fprintf(stderr, "WARNING: %s\n", s);
}

static void error(char *s)
{
	fprintf(stderr, "ERROR: %s\n", s);
	exit(-1);
}

static void usage(char *progname)
{
	printf("Usage: %s <file>\n", progname);
	exit(0);
}

void segfault(int signal)
{
	printf("\n\n\n********************************************************\n");
	printf("* OMG THIS IS TERRIBLE! Signal %d received, exiting... *\n", signal);
	printf("********************************************************\n");
	show_status();
	exit(EFAULT);
}

int main(int argc, char** argv)
{
	struct instruction cur;
	struct sigaction act, oact;
	int codefile;
	int instr_n = 0;
	u16 codelen;
	void *map;

	act.sa_handler = segfault;

	if (argc < 2)
		usage(argv[0]);

	/* Open source file */
	if ((codefile = open(argv[1], O_RDONLY)) == -1)
		error("file does not exist or does not have priviledges");

	/* Check length */
	if ((codelen = lseek(codefile, 0, SEEK_END)) % 4 != 0) {
		warning("file not word-aligned, filling with zeros ('\\0')");
		codelen = codelen - (codelen % 4) + 4;
	}

	printf("Memory size: %d bytes\n", MEMSIZE);
	if (MEMSIZE < codelen + 4)
		error("not enough memory");

	map = mmap(NULL, codelen, PROT_READ, MAP_SHARED, codefile, 0);
	memcpy(m, map, codelen);			/* copy code from file into memory */
	memset(m + codelen, 0xff, 4);		/* put .exit at EOF (0xffffffff) */

	munmap(map, codelen);
	close(codefile);

	sigaction(SIGSEGV, &act, &oact);

	printf("\nGo go go...\n");
	while (1) {

		/* If pipe is ready to execute an instruction, do it */
		if (pipe_ready()) {
			instr_n += 1;		/* Executing one more instruction */
			memcpy(cur.val._byte, m + cur_inst(), 4);	/* "fetch" */
			parse(&cur);	/* decode & print instruction */
			exec(&cur);			/* "execute" */
		} else {
			wait_pipe();
		}

		do_cycle();

		if (cur.val._u32 == 0xffffffff)	// exit
			break;
	}

	printf("\nExecution finished. Total instructions: %d\n", instr_n-1);
	show_status();

	return 0;
}

