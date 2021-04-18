# Overview 
This repository comprises of several projects written in assembly and C ranging from simple ones to quite complex.

# DOS :floppy_disk:   
In order to get used to writing in assembly and don't feel overencumbered by the nuances and the security mechanisms of modern operating systems.

## Drawing an animated frame
**Difficulty**: :gun::gun::gun:

This is just a one-evening program that for me to get acquainted with assembly. Nothing special - it draws an animated frame and inside of it you can convert numbers into several numeral base systems.

## Interruption interception
**Difficulty**: :gun::gun:

Here I used the idea of intercepting system interruptions in order to implement the application that draws current contents of general-use registers.

## Pair hacking
**Difficulty**: :crossed_swords:
> Huge thanks to [vssense](https://github.com/vssense) for being my opponent in this project and just being a good friend of mine, whom I could always rely on, in case I want to hear how stupid I am :clown_face:.    

In pairs we had to write simple password-filling programs and then exchange only executables with each other. The idea was to understand how the opponent's program works using nothing but disassembled code. Then ~~hacking~~ began... Then our job was to do the following:

1. Find several wrong inputs (not the correct password), which however pass as correct. This can be achieved by overflowing the buffer that contains correct/user-entered password.
2. Write a *key-gen* with GUI that changes the executable so that any input passes as correct password.     

# Linux and NASM :computer:
Here we step into the restraining modern world of Linux. 

## Printf
**Difficulty**: :crossed_swords:

Simple implementation of C' `printf()` (see its [documentation](https://en.cppreference.com/w/c/io/fprintf) on cppreference) function, written in NASM, though lacking multiple options. My function's documentation:
```asm
;------------------------------------------------------------------------------
; Writes formatted output to stdout.
;
; Expects: [RBP + 16] = formatted string
;          [RBP + 24] = arg1
;          [RBP + 32] = arg2
;               ...
;
; Returns: RAX = number of successfully written characters or -1 if an error 
;                occurred.
; Changes: RAX, RBX, RCX, RDX, RDI, RSI, R8, R9, R10, R12
; Details: You can use either of these argument specifications
; 
;            Symb  Bytes            Description 
;          +------+-----+----------------------------------+
;          |  %c  |  1  |  ASCII character                 |
;          +------+-----+----------------------------------+
;          |  %s  |  8  |  null-terminated string          |
;          +------+-----+----------------------------------+
;          |  %d  |  4  |  decimal     integer (signed)    |
;          +------+-----+----------------------------------+
;          |  %u  |  4  |  decimal     integer (unsigned)  |
;          +------+-----+----------------------------------+
;          |  %x  |  4  |  hexadecimal integer (unsigned)  |
;          +------+-----+----------------------------------+
;          |  %o  |  4  |  octal       integer (unsigned)  |
;          +------+-----+----------------------------------+
;          |  %b  |  4  |  binary      integer (unsigned)  |
;          +------+-----+----------------------------------+
;           
;          To write symbol '%' you should pass '%%'.
;------------------------------------------------------------------------------
writeFormatted:
...
```

Even though it states that the function gets parameters only through stack, you can use this function from C/C++ code (the "callable" version is `_writeFormatted()`)! This has been done by pushing first six parameters (*rdi, rsi, rdx, rcx, r8, r9*) into stack.

## Mandelbrot set visualization using AVX2
**Difficulty**: :crossed_swords:

Probably the most visually apealling project here. It's based on one unique set of complex numbers, which was defined by mathematician [Benoit Mandelbrot](https://en.wikipedia.org/wiki/Benoit_Mandelbrot) and called Mandelbrot set. In order to boost the performance of computing colors of each pixel, [AVX2 instructions](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Advanced_Vector_Extensions_2) have been used. With them, 8 pixels can be processed simultaneously!   

## Hash table optimization
**Difficulty**: :crossed_swords::crossed_swords:

See my [full text](nasm-linux/hash-table) on it.