section .data
DOIS dd 2

section .bss
OLD_DATA resd 1
NEW_DATA resd 1
TMP_DATA resd 1

section .text
mov ebx, 0
L1: push eax
push edx
mov eax, ebx
cdq
idiv dword [DOIS]
mov ebx, eax
pop edx
pop eax
push eax
mov eax, ebx
imul dword [DOIS]
mov ebx, eax
pop eax
sub ebx, [TMP_DATA]
add ebx, [DOIS+8]
