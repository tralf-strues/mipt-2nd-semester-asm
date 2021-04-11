.globl _Z4findPK9HashTablePKc
.type _Z4findPK9HashTablePKc, @function
.intel_syntax noprefix

# ------------------------------------------------------------------------------
# Finds an element associated with key in hashTable.
# 
# Expects: RDI = constant pointer to a HashTable
#          RSI = constant pointer to a string (key) 
# 
# Returns: RAX = constant pointer to value (DictEntry) or nullptr if there is 
#          no element with key in hashTable.
# ------------------------------------------------------------------------------
_Z4findPK9HashTablePKc:
                push r12 # FIXME: unneeded
                push rbx 

                mov rbx, rdi                    # rbx = hashTable
                
                mov rdi, rsi
                call QWORD PTR [rbx + 24]       # rax = getHash(key)

                xor rdx, rdx
                div QWORD PTR [rbx]             # rdx = rdx:rax % hashTable->size = hash

                mov rax, QWORD PTR [rbx + 8]    # rax = buckets
                
                # sizeof(Bucket) = 24
                lea rdx, [rdx + 2 * rdx]     
                lea rdx, [rax + 8 * rdx]        # &(buckets[hash])
                
                # mov r12, QWORD PTR [rbx + 16] # r12 = cmp
                mov rbx, QWORD PTR [rdx]        # rbx = buckets[hash].data
                mov rcx, QWORD PTR [rdx + 8]    # rcx = buckets[hash].size
                
                # rcx = rbx + 64 * rcx (sizeof(Pair)) = last + 1 bucket
                lea rcx, [8 * rcx]           
                lea rcx, [rbx + 8 * rcx]     

                # ymm1 = strToYMM(key) 
                # mov	QWORD PTR [rsp + 16], 0
	            # mov	QWORD PTR [rsp + 24], 0
	            # mov	QWORD PTR [rsp + 32], 0
	            # mov	QWORD PTR [rsp + 40], 0

                # lea rdi, [rsp + 16]

                # push rbx 
                # push rcx 
                # call strcpy # changes rax, rbx, rcx
                # pop rcx 
                # pop rbx

                # vmovdqu ymm1, YMMWORD PTR [rdi] # rdi = rsp + 16

                call _Z8strToYMMPKc # ymm0 = strToYMM(key)

.LOOP_FIND_PAIR:
                cmp rbx, rcx 
                jae .NOT_FOUND_PAIR

                vpcmpeqb ymm1, ymm0, YMMWORD PTR [rbx] 
                vpmovmskb eax, ymm1

                cmp eax, -1
                je .FOUND_PAIR

                add rbx, 64
                
                jmp .LOOP_FIND_PAIR
.FOUND_PAIR:
                lea rax, [rbx + 32]
                # lea	rsp, [r12 - 16] # FIXME:
                pop rbx
                pop r12
                ret
.NOT_FOUND_PAIR:
                xor rax, rax
                # lea	rsp, [r12 - 16] # FIXME:
                pop rbx
                pop r12
                ret

    