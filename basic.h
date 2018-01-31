
#pragma once
#ifndef NIGHTINGALE_BASIC_H
#define NIGHTINGALE_BASIC_H

#define static_assert _Static_assert

// Integer types

//
// Some may scream "But muh compatailiby" - I make the following case:
// Have you ever actually tested your code on a platform without standard integers?
// something with 9 bit char's?
//
// As we all know, untested code does not work, period, so unless you regularly test these
// very edge-cases, you're setting yourself up for dissaspointment later.
//
// Therefore, I make a contract that in my code a u8 is 8 bits, and a u32 is 32 bits, and
// if you want to go in and port to a platform where the chars are 12 bits and the pointers
// are 17 bits across, you can do that, and you won't be chasing runtime errors to do with
// pointer arithmetic and all the overflow you were so proud of doesn't work anymore.
//
// My way, you do the same amount of effort, but you can at least see what needs to change.
// Grep the code for u8, hack it to work with (not 8) bits. Done.
//

static_assert(__CHAR_BIT__ == 8, "Bytes must be 8 bits (How did you even do this?)");
static_assert(sizeof(short int) == 2, "Short must be 2 bytes");
static_assert(sizeof(int) == 4, "Int must be 4 bytes");
static_assert(sizeof(long int) == 8, "Long must be 8 bytes");

// I'm less married to pointers, but there could still be issues, and the porting load
// is literally changing this line and the ones that define isize and usize below.
static_assert(sizeof(void *) == 8, "Pointer must be 8 bytes (Are you using a 32 bit compiler?)");

// Look in to other (better) ways to get these exact values
// What compilers define __u8 and friends? __uint8_t?

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;
typedef unsigned long int usize;

typedef signed char i8;
typedef signed short int i16;
typedef signed int i32;
typedef signed long int i64;
typedef signed long int isize;

#ifdef NIGHTINGALE_BASIC_NOSTDINC
// In an application C program, I am happy to include standard C headers for these things

// Boolean type

typedef _Bool bool;
#define true 1
#define false 0
// Or would I prefer enum { false = 0, true = 1 };


// General stuff
#define NULL (void *)0

#endif // NIGHTINGALE_BASIC_NOSTDINC

#endif // NIGHTINGALE_BASIC_H

