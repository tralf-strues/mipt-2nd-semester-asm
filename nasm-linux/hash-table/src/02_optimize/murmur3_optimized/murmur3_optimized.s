.globl _Z16getMurmur3HashOPPKc
.type _Z16getMurmur3HashOPPKc, @function
.intel_syntax noprefix

_Z16getMurmur3HashOPPKc:
        push    rbx
        mov     rbx, rdi
        call    strlen
        mov     rdi, rax
        mov     r8, rax
        shr     rdi, 2
        je      .L7
        xor     ecx, ecx
        mov     esi, 220202
        xor     edx, edx
.L3:
        imul    eax, DWORD PTR [rbx+rdx*4], -862048943
        lea     edx, [rcx+1]
        mov     rcx, rdx
        rol     eax, 15
        imul    eax, eax, 461845907
        xor     eax, esi
        rol     eax, 13
        lea     esi, [rax-430675100+rax*4]
        cmp     rdx, rdi
        jb      .L3
.L2:
        mov     rdi, r8
        mov     rax, r8
        and     rdi, -4
        and     eax, 3
        add     rbx, rdi
        cmp     rax, 2
        je      .L4
        cmp     rax, 3
        je      .L5
        xor     edx, edx
        cmp     rax, 1
        je      .L12
.L6:
        mov     eax, esi
        pop     rbx
        xor     eax, r8d
        xor     eax, edx
        mov     esi, eax
        shr     esi, 16
        xor     eax, esi
        imul    esi, eax, -2048144789
        mov     eax, esi
        shr     eax, 13
        xor     eax, esi
        imul    eax, eax, -1028477387
        mov     edx, eax
        shr     edx, 16
        xor     eax, edx
        ret
.L4:
        movzx   eax, BYTE PTR [rbx+1]
        sal     eax, 8
        imul    eax, eax, -862048943
        rol     eax, 15
        imul    edx, eax, 461845907
        jmp     .L6
.L12:
        movzx   eax, BYTE PTR [rbx]
        imul    eax, eax, -862048943
        rol     eax, 15
        imul    edx, eax, 461845907
        jmp     .L6
.L5:
        movzx   edx, BYTE PTR [rbx+2]
        sal     edx, 16
        imul    edx, edx, -862048943
        rol     edx, 15
        imul    edx, edx, 461845907
        jmp     .L6
.L7:
        mov     esi, 220202
        jmp     .L2

#.globl _Z16getMurmur3HashOPPKc
#.type _Z16getMurmur3HashOPPKc, @function
#.intel_syntax noprefix
#
#_Z16getMurmur3HashOPPKc:
#.LFB34:
#                # push r12
#
#                # setting constants
#                mov esi,  0xCC9E2D51            # MURMUR_MUL_COEFFICIENT_1 
#                mov r9d,  0x1B873593            # MURMUR_MUL_COEFFICIENT_2 
#                mov r10b, 15                    # MURMUR_ROT_COEFFICIENT_1 
#                mov r11d, 0xE6546B64                    # MURMUR_ROT_COEFFICIENT_2 
#
#                # edx = length = strlen(string)
#                call strlen@PLT # eax = strlen(string)
#                mov edx, eax 
#
#                # eax = hash = MURMUR_SEED = 0
#                xor eax, eax
#
#                # ecx = blocksCount = length / 4
#                mov ecx, edx 
#                shr ecx, 2
#
#                test ecx, ecx
#                jz .HASH_BLOCKS_END
#.HASH_BLOCKS:
#                # r8d = cur block
#                mov r8d, DWORD PTR [rdi]
#                imul r8d, 0xCC9E2D51                   # r8d *= MURMUR_MUL_COEFFICIENT_1
#                rol  r8d, 15                    # rol r8d by MURMUR_ROT_COEFFICIENT_1
#                imul r8d, 0x1B873593                   # r8d *= MURMUR_MUL_COEFFICIENT_2
#
#                xor eax, r8d                    # hash = hash xor block
#
#                rol eax, 13                     # rol hash by MURMUR_ROT_COEFFICIENT_2
#                lea eax, [4 * eax + eax]          # eax *= 5
#                add eax, 0xE6546B64
#
#                add rdi, 4                      # move to next block
#
#                loop .HASH_BLOCKS
#.HASH_BLOCKS_END:
#
#                xor ecx, ecx                    # ecx = tailValue = 0
#                
#                mov r10d, edx                   # FIXME:
#                and r10d, 0x3
#                
#                test r10d, r10d
#                jz .REMAINDER_0
#
#                cmp r10d, 1
#                je .REMAINDER_1
#
#                cmp r10d, 2
#                je .REMAINDER_2
#
#                cmp r10d, 3
#                je .REMAINDER_3
#
#.REMAINDER_1:
#                xor cl, BYTE PTR [rdi]
#
#                jmp .REMAINDER_0
#.REMAINDER_2:
#                mov r10b, BYTE PTR [rdi + 1]
#                xor rdi, rdi
#                mov dil, r10b
#                shl rdi, 8
#
#                xor ecx, edi
#                jmp .REMAINDER_0
#.REMAINDER_3:
#                mov r10b, BYTE PTR [rdi + 2]
#                xor rdi, rdi
#                mov dil, r10b
#                shl rdi, 16
#
#                xor ecx, edi
#.REMAINDER_0:
#
#                imul ecx, 0xCC9E2D51
#                rol  ecx, 15                    # MURMUR_ROT_COEFFICIENT_1
#                imul ecx, 0x1B873593
#                xor eax, ecx                    # hash ^= tailValue
#
#                xor eax, edx                    # hash ^= length
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 16                     # hash >>= 16
#                xor eax, edx                    # hash ^= (hash >> 16)
#
#                imul eax, 0x85EBCA6B            # hash *= MURMUR_FINAL_MUL_1
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 13                     # hash >>= MURMUR_FINAL_SHIFT_RIGHT
#                xor eax, edx                    # hash ^= (hash >> 13)
#
#                imul eax, 0xC2B2AE35            # hash *= MURMUR_FINAL_MUL_2
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 16                     # hash >>= 16
#                xor eax, edx                    # hash ^= (hash >> 16)
#
#                # pop r12
#	        ret

#.globl _Z16getMurmur3HashOPPKc
#.type _Z16getMurmur3HashOPPKc, @function
#.intel_syntax noprefix
#
#_Z16getMurmur3HashOPPKc:
#.LFB34:
#                # push r12
#
#                # setting constants
#                mov esi,  0xCC9E2D51            # MURMUR_MUL_COEFFICIENT_1 
#                mov r9d,  0x1B873593            # MURMUR_MUL_COEFFICIENT_2 
#                #mov r10d, 15                    # MURMUR_ROT_COEFFICIENT_1 
#                mov r11d, 0xE6546B64                    # MURMUR_ROT_COEFFICIENT_2 
#
#
#                # edx = length = strlen(string)
#                call strlen # eax = strlen(string)
#                mov edx, eax 
#
#                # eax = hash = MURMUR_SEED = 0
#                xor eax, eax
#
#                # ecx = blocksCount = length / 4
#                mov ecx, edx 
#                shr ecx, 2
#
#                test ecx, ecx
#                jz .HASH_BLOCKS_END
#.HASH_BLOCKS:
#                # r8d = cur block
#                mov r8d, DWORD PTR [rdi]
#                imul r8d, esi                   # r8d *= MURMUR_MUL_COEFFICIENT_1
#                rol  r8d, 15                    # rol r8d by MURMUR_ROT_COEFFICIENT_1
#                imul r8d, r9d                   # r8d *= MURMUR_MUL_COEFFICIENT_2
#
#                xor eax, r8d                    # hash = hash xor block
#
#                rol eax, 13                     # rol hash by MURMUR_ROT_COEFFICIENT_2
#                lea eax, [4 * eax + eax]          # eax *= 5
#                add eax, r11d
#
#                add rdi, 4                      # move to next block
#
#                loop .HASH_BLOCKS
#.HASH_BLOCKS_END:
#
#                xor ecx, ecx                    # ecx = tailValue = 0
#                
#                mov r10d, edx                   # FIXME:
#                and r10d, 0x3
#                xor rcx, rcx 
#                jmp QWORD PTR [SWITCH_TABLE + 8 * r10d]
#
#SWITCH_TABLE:
#                .quad .REMAINDER_0
#                .quad .REMAINDER_1
#                .quad .REMAINDER_2
#                .quad .REMAINDER_3
#
#.REMAINDER_1:
#                xor cl, BYTE PTR [rdi]
#
#                jmp .REMAINDER_0
#.REMAINDER_2:
#                mov r10b, BYTE PTR [rdi + 1]
#                xor rdi, rdi
#                mov dil, r10b
#                shl rdi, 8
#
#                xor ecx, edi
#                jmp .REMAINDER_0
#.REMAINDER_3:
#                mov r10b, BYTE PTR [rdi + 2]
#                xor rdi, rdi
#                mov dil, r10b
#                shl rdi, 16
#
#                xor ecx, edi
#                jmp .REMAINDER_0
#.REMAINDER_0:
#
#                imul ecx, esi
#                rol  ecx, 15                    # MURMUR_ROT_COEFFICIENT_1
#                imul ecx, r9d
#                xor eax, ecx                    # hash ^= tailValue
#
#                xor eax, edx                    # hash ^= length
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 16                     # hash >>= 16
#                xor eax, edx                    # hash ^= (hash >> 16)
#
#                imul eax, 0x85EBCA6B            # hash *= MURMUR_FINAL_MUL_1
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 13                     # hash >>= MURMUR_FINAL_SHIFT_RIGHT
#                xor eax, edx                    # hash ^= (hash >> 13)
#
#                imul eax, 0xC2B2AE35            # hash *= MURMUR_FINAL_MUL_2
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 16                     # hash >>= 16
#                xor eax, edx                    # hash ^= (hash >> 16)
#
#                # pop r12
#	        ret
#
#

#.globl _Z16getMurmur3HashOPPKc
#.type _Z16getMurmur3HashOPPKc, @function
#.intel_syntax noprefix
#
#_Z16getMurmur3HashOPPKc:
#.LFB34:
#                # .cfi_startproc
#	        # push rbx
#	        # .cfi_def_cfa_offset 16
#	        # .cfi_offset 3, -16
#                #push rbp
#                #mov rbp, rsp
#
#	        # push rbx
#                push r12
#
#                # setting constants
#                mov esi,  0xCC9E2D51            # MURMUR_MUL_COEFFICIENT_1 
#                mov r9d,  0x1B873593            # MURMUR_MUL_COEFFICIENT_2 
#                mov r10d, 15                    # MURMUR_ROT_COEFFICIENT_1 
#                mov r11d, 13                    # MURMUR_ROT_COEFFICIENT_2 
#
#                # edx = length = strlen(string)
#                call strlen # eax = strlen(string)
#                mov edx, eax 
#
#                # eax = hash = MURMUR_SEED = 0
#                xor eax, eax
#
#                # ecx = blocksCount = length / 4
#                mov ecx, edx 
#                shr ecx, 2
#
#                test ecx, ecx
#                jz .HASH_BLOCKS_END
#.HASH_BLOCKS:
#                # r8d = cur block
#                mov r8d, DWORD PTR [rdi]
#                imul r8d, esi                   # r8d *= MURMUR_MUL_COEFFICIENT_1
#                rol  r8d, 15                    # rol r8d by MURMUR_ROT_COEFFICIENT_1
#                imul r8d, r9d                   # r8d *= MURMUR_MUL_COEFFICIENT_2
#
#                xor eax, r8d                    # hash = hash xor block
#
#                rol eax, 13                     # rol hash by MURMUR_ROT_COEFFICIENT_2
#                lea eax, [4*eax + eax]          # eax *= 5
#                add eax, 0xE6546B64
#
#                add rdi, 4                      # move to next block
#
#                loop .HASH_BLOCKS
#.HASH_BLOCKS_END:
#
#                xor ecx, ecx                    # ecx = tailValue = 0
#                
#                mov r12d, edx                   # FIXME:
#                and r12d, 0x3
#                xor rcx, rcx 
#                jmp QWORD PTR [SWITCH_TABLE + 8 * r12d]
#
#SWITCH_TABLE:
#                .quad .REMAINDER_0
#                .quad .REMAINDER_1
#                .quad .REMAINDER_2
#                .quad .REMAINDER_3
#
#.REMAINDER_1:
#                xor cl, BYTE PTR [rdi]
#
#                jmp .REMAINDER_0
#.REMAINDER_2:
#                mov r12b, BYTE PTR [rdi + 1]
#                xor rdi, rdi
#                mov dil, r12b
#                shl rdi, 8
#
#                xor ecx, edi
#                jmp .REMAINDER_0
#.REMAINDER_3:
#                mov r12b, BYTE PTR [rdi + 2]
#                xor rdi, rdi
#                mov dil, r12b
#                shl rdi, 16
#
#                xor ecx, edi
#                jmp .REMAINDER_0
#.REMAINDER_0:
#
#                imul ecx, esi
#                rol  ecx, 15                    # MURMUR_ROT_COEFFICIENT_1
#                imul ecx, r9d
#                xor eax, ecx                    # hash ^= tailValue
#
#                xor eax, edx                    # hash ^= length
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 16                     # hash >>= 16
#                xor eax, edx                    # hash ^= (hash >> 16)
#
#                imul eax, 0x85EBCA6B            # hash *= MURMUR_FINAL_MUL_1
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 13                     # hash >>= MURMUR_FINAL_SHIFT_RIGHT
#                xor eax, edx                    # hash ^= (hash >> 13)
#
#                imul eax, 0xC2B2AE35            # hash *= MURMUR_FINAL_MUL_2
#
#                mov edx, eax                    # edx is used for shifts of hash
#                shr edx, 16                     # hash >>= 16
#                xor eax, edx                    # hash ^= (hash >> 16)
#
#                # pop rbx
#	        # .cfi_remember_state
#	        # .cfi_def_cfa_offset 8
#
#                pop r12
#                # pop rbx
#                # pop rbp
#	        ret
#
#                #.cfi_endproc
#
#
#; .set MURMUR_MUL_COEFFICIENT_1, 0xCC9E2D51
#; .set MURMUR_MUL_COEFFICIENT_2, 0x1B873593
#; .set MURMUR_ROT_COEFFICIENT_1, 15
#; .set MURMUR_ROT_COEFFICIENT_2, 13
#
#; .set MURMUR_MUL_COEFFICIENT_3, 5
#; .set MURMUR_ADD_COEFFICIENT,   0xE6546B64
#
#; .set MURMUR_FINAL_MUL_1,       0x85EBCA6B
#; .set MURMUR_FINAL_MUL_2,       0xC2B2AE35
#; .set MURMUR_FINAL_SHIFT_RIGHT, 13
