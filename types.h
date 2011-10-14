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
	MOV,MUL,LML,SWP,LDB,LDM,B,BXC,COP,SWI,UNK,BYE,NONE
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


/*
 * Returns the 32-bit unsigned value of bits [from,from+size) of base.
 *
 * This is done by shifting right base 'from' times and then masking the bits
 * at the left of 'size'.
 */
inline u32 get_u32(u32 base, u8 from, u8 size)
{
	return (base >> from) & ~((~0) << size);
}

/*
 * Returns the 32-bit signed value of bits [from,from+size) of base.
 *
 * If the value's most significant bit is 1, sign-extend the value by OR'ing
 * with 1 all bits at the left of it. Else, call get_u32().
 */
inline s32 get_s32(u32 base, u8 from, u8 size)
{
	if (base & (1 << (from+size-1)))
		return (base >> from) | (~0) << size;
	else
		return get_u32(base, from, size);
}


#endif	/* _TYPES_H */

