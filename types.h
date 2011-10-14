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

#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

enum inst_type {
	MOV,MUL,LML,SWP,LDB,LDM,B,BXC,COP,SWI,UNK,BYE
};

struct instruction {
	union {
		u8 _byte[4];
		u32 _u32;
	} val;

	enum inst_type type;
	u8 cond;
};

enum mov_type {
	add,adc,sub,sbc,rsb,rsc,cmp,cmn,tst,teq,and,eor,orr,bic,mov,mvn,lsl,lsr,
	ror,rol,rrx
};

struct mov_info {
	enum mov_type op;
};

inline u32 get_u32(u32 word, u8 from, u8 size)
{
	return (word >> from) & ~((~0) << size);
}

inline s32 get_s32(u32 val, u8 from, u8 size)
{
	s32 ret = val >> from;
	if (ret & (1 << (size-1)))
		ret |= ((~0) << size);
	else
		ret &= ~((~0) << size);

	return ret;
}


#endif	/* _TYPES_H */

