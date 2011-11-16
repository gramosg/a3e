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
#include <getopt.h>
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
#include "pipeline.h"

#define LIM_I		0
#define LIM_C		1


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
	printf("Usage: %s [-v|--verbose] [-c|--max-cycles] [-i|--max-inst] <file>\n", progname);
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
	int codefile, arg, cur_pos;
	int instrn = 0, cycles = 0;
	int verbose = 0;
	int codelen;
	void *map;
	int limits[] = {-1, -1};

	int opt = 0;
	struct option options[] = {
		{"help", no_argument, NULL, 'h'},
		{"verbose", no_argument, NULL, 'v'},
		{"max-cycles", required_argument, NULL, 'c'},
		{"max-inst", required_argument, NULL, 'i'},
//		{"memsize", required_argument, NULL, 'm'},
		{0, 0, 0, 0}
	};

	while (opt != -1) {
		switch (opt = getopt_long(argc, argv, "hvc:i:", options, NULL)) {
		case -1:
			break;
		case 'i':
			if ((arg = atoi(optarg)) >= 0)
				limits[LIM_I] = arg;
			break;
		case 'c':
			if ((arg = atoi(optarg)) >= 0)
				limits[LIM_C] = arg;
			break;
		case 'v':
			verbose = 1;
			break;
//		case 'm':
//			if ((arg = atoi(optarg)) > 0)
//				memsize = arg;
//			break;
		case 'h':
		case '?':
			usage(argv[0]);
		}
	}

	// There must be only one parameter not parsed by getopt: the file name
	if (optind != argc-1)
		usage(argv[0]);

	act.sa_handler = segfault;

	/* Open source file */
	if ((codefile = open(argv[argc-1], O_RDONLY)) == -1)
		error("file does not exist or does not have priviledges");

	/* Check length */
	if ((codelen = lseek(codefile, 0, SEEK_END)) % 4 != 0) {
		warning("file not word-aligned, filling with zeros ('\\0')");
		codelen = codelen - (codelen % 4) + 4;
	}

	if (verbose)
		printf("Memory size: %d bytes\n", MEMSIZE);
	if (MEMSIZE < codelen + 4)
		error("not enough memory");

	map = mmap(NULL, codelen, PROT_READ, MAP_SHARED, codefile, 0);
	memcpy(m, map, codelen);			/* copy code from file into memory */
	memset(m + codelen, 0xff, 4);		/* put .exit at EOF (0xffffffff) */

	munmap(map, codelen);
	close(codefile);

	sigaction(SIGSEGV, &act, &oact);

	if (verbose)
		printf("Beginning execution...\n-------------------------------\n\n");

	while (1) {

		if (limits[LIM_C] >= 0 && cycles >= limits[LIM_C])
			break;
		if (limits[LIM_I] >= 0 && instrn >= limits[LIM_I])
			break;
		cycles++;

		/* If pipe is ready to execute an instruction, do it */
		if ((cur_pos = fetch(verbose)) != -1) {
			memcpy(cur.val._byte, m + cur_pos, 4);	/* "fetch" */
			exec(&cur);		/* "execute" */
			instrn++;		/* Executing one more instruction */
		}

	}

	printf("\nExecution finished OK. Instructions: %d, cycles: %d\n", instrn, cycles);
	if (verbose)
		show_status();

	return 0;
}

