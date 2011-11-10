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

#ifndef _PIPELINE_H
#define _PIPELINE_H


#include "types.h"

extern void wait_pipe(void);
extern void jmp(u32 new_pc);
extern void b(u32 offset);
//extern void next_inst(void);
extern void stage_up(void);
extern u32 cur_inst(void);
extern int pipe_ready(void);


#endif	/* _PIPELINE_H */

