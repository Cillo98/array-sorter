#include "stdafx.h"

void main() {
	char msgEnterCounter[] = "Enter the desired amount of positive integers (2 to 5): ";
	char msgWrongInput[] = "\nThe entered number is not between 2 and 5. Please try again \n\n";
	char msgIntegerRules[] = "\nAll positive integers will be stored, the first negative integer will finish the input phase\n";
	char msgEnterInteger[] = "\nEnter integer number ";
	char msgLoopTimes[] = "\nInput phase terminates. The programme looped ";
	char msgIntegers[] = " times.";
	char msgOrderedArray[] = "\n\nThe entered integers from lowest to highest are: ";
	char msgTotalAmount[] = "\n\nThe sum of all the positive integers entered is: ";
	char msgIntegerToExit[] = "\n\n\nEnter any integer to close the programme ";
	char msgColon[] = ": ";
	char msgComma[] = ", ";
	char format[] = "%d"; // used to scan and print integers
	int integerArray[5]; // array containing the entered integers
	int counter; // counter for the loops to enter the positive integers into the array
	// first is the number of integer to be entered; then is the actual number of entered integers
	int nInteger = 1;
	int totalAmount = 0; // sum of all the entered positive integers
	// the following variable is only used at the end of the programme to wait for the user to input
	// an integer, so the the programme does not close immediately after entering the last number
	int waitUserInput;
	_asm {
		jmp		ENTER_INPUT			; the first run I immediately go to enterInput, the error message
									; must not be displayed
	WRONG_INPUT:					; IF THE INPUT IS NOT BETWEEN 2 AND 5, THE PROGRAMME JUMPS HERE
		lea		eax, msgWrongInput
		push	eax
		call	printf				; printing the error message
		add		esp, 4				; I clean the stack by increasing the esp stack pointer

	ENTER_INPUT:					; INPUT PHASE
		lea		eax, msgEnterCounter; preparing the message to be printed
		push	eax
		call	printf				; printing the first message
		add		esp, 4				; cleaning the stack

		lea		eax, counter		; preparing to receive the counter as input
		push	eax					; pushing the counter effective address to get its value with scanf_s
		lea		eax, format
		push	eax					; pushing the format for the scanf function
		call	scanf_s				; reading the counter
		add		esp, 8				; I used 2 spaces in the stack, so I clean both(4 + 4 bytes)

		mov		eax, counter		; preparing the entered number to be checked
		cmp		eax, 2
		jb		WRONG_INPUT			; if eax is less than 2, jumps to the wrongInput section
		cmp		eax, 5
		ja		WRONG_INPUT			; if eax is greater than 5, jumps to the wrongInput section

	INPUT_RULES:					; DISPLAY THE INPUT RULES (unused label, just keeps the code clear)
		lea		eax, msgIntegerRules; msgIntegerRules tells the user that entering a negative integer
									; will exit the loop
		push	eax
		call	printf				; printing the message with the integer rules
		add		esp, 4

		mov		ecx, counter		; moving the entered counter into the ecx registry(loop counter)
		mov		ebx, 0				; initialize the index of the array to 0

	INPUT_LOOP:						; PROMT USER INPUT - STORE THE INPUT IN THE ARRAY - EXIT IF
									; NEGATIVE ENTERED
		push	ecx					; pushing the ecx loop counter, so printf does not overwrite it(save ecx)
		lea		eax, msgEnterInteger
		push	eax
		call	printf				; promt the user to enter an integer(msg PART 1)
		add		esp, 4

		push	nInteger			; prepearing the number of the input integer to be printed
		lea		eax, format
		push	eax					; pushing the format for integer values
		call	printf				; printing the number of the input integer(msg PART 2)
		add		esp, 8				; clean the stack after printing 2 things(so 8 bytes)

		lea		eax, msgColon
		push	eax
		call	printf				; prints the colon after the input integer number(msg PART 3)
		add		esp, 4

		lea		eax, integerArray[ebx] ; load the address of the array at the index(ebx / 4)
		push	eax
		lea		eax, format
		push	eax
		call	scanf_s				; input the integer into integerArray[ebx]
		add		esp, 8

		mov		edx, integerArray[ebx]
		cmp		edx, 0
		jl		NEGATIVE_ENTERED	; if the entered integer is less than 0, exit the loop and jump to
									; next phase
		add		ebx, 4				; increase the array pointer by 4 bytes to point to the following index
		inc		nInteger			; increase the value of nInteger by 1
		pop		ecx					; retriving the loop counter that was previously saved in the stack(load ecx)
		loop	INPUT_LOOP			; decrease ecx and loop back to INPUT_LOOP if ecx > 0
		jmp		PRINT_nLOOPS

	NEGATIVE_ENTERED:
		add		esp, 4				; discard the last entered number

	PRINT_nLOOPS:					; PRINTING THE NUMBER OF LOOPS
		dec		nInteger			; the variable nInteger always stores the number of entered
		lea		eax, msgLoopTimes	; integers + 1, so I decrease it
		push	eax
		call	printf				; printing the number of loops message
		pop		eax

		push	nInteger
		lea		eax, format
		push	eax
		call	printf				; printing the number of loops
		add		esp, 8

		lea		eax, msgIntegers
		push	eax
		call	printf				; printing the word "integers" after the entered integers number
		pop		eax

		mov		eax, nInteger
		cmp		eax, 1
		jle 	SORTING_FINISHED	; if the user entered 1 or less values, skip the bubble sort

	BUBBLE_SORT:					; START THE BUBBLE SORTING ALGORITHM(unused label, just keeps the code clear)
		mov		ecx, nInteger		; preparing the loop counter
		dec		ecx					; OUTER_LOOP should run[array_length - 1] times

	OUTER_LOOP:
		push	ecx					; saving the loop counter for the OUTER_LOOP, which is decreased by 1
									; each loop
		lea		ebx, integerArray	; now the effective adress of integerArray is stored into ebx
	INNER_LOOP:
		mov		eax, [ebx]			; move the value in ebx to eax so the programme can compare it
		cmp		[ebx + 4], eax		; if the value in ebx + 4 is greater that the previous one, then
		jge		JUMPS				; those two values are ordered so the programme jumps to the end of the loop
		xchg	eax, [ebx + 4]		; if the values are not ordered(if[ebx] > [ebx + 4]), swap them
		mov		[ebx], eax			; finish swapping
	JUMPS:
		add		ebx, 4				; moving onto the next pair of values
		loop	INNER_LOOP
		pop		ecx					; retriving the loop counter saved when the programme last entered
		loop	OUTER_LOOP			; the OUTER_LOOP

	SORTING_FINISHED:				; PRINT THE ORDERED ARRAY MESSAGE(unused label, keeps the code clear)
		lea		eax, msgOrderedArray
		push	eax
		call	printf
		pop		eax

		mov		ecx, nInteger		; getting the loop counter
		cmp		ecx, 0				; if the user entered no positive integer the programme skips to
		je		PRINT_SUM			; the displaying of the sum(which is initialized to 0)
		dec		ecx					; print all the values but the last one, that must be printed without comma
		lea		ebx, integerArray	; load the address of the 0th element of the array into ebx
		cmp		ecx, 0				; if the user entered only 1 positive integer, the programme
		je		LAST_VALUE_OUTPUT	; skips to displaying only the last(and only) value
	OUTPUT_LOOP:					; PRINT THE ORDERED VALUES WITH COMMAS IN BETWEEN
		push	ecx					; save the loop counter
		push	[ebx]				; preparing the values in integerArray to be printed
		lea		eax, format
		push	eax
		call	printf				; print the number
		add		esp, 8
		add		ebx, 4				; point to the next value in integerArray
				
		lea		eax, msgComma
		push	eax
		call	printf				; print the comma between numbers
		pop		eax

		pop		ecx					; retrieve the loop counter
		loop	OUTPUT_LOOP

	LAST_VALUE_OUTPUT:				; PRINT THE LAST ORDERED VALUE WITHOUT COMMA AT THE END
		push	[ebx]
		lea		eax, format
		push	eax
		call	printf
		add		esp, 8

		mov		ecx, nInteger		; prepearing the loop counter
		lea		ebx, integerArray	; storing the address of the array into ebx
	SUM_LOOP:						; SUM ALL THE ENTERED POSITIVE INTEGERS INTO totalAmount(unused label)
		mov		eax, totalAmount	; this line and the following two contain the sum of
		add		eax, [ebx]			; [ebx] to totalAmount, which must be done through a registry
		mov		totalAmount, eax
		add		ebx, 4				; pointing to the next integer in the array
		loop	SUM_LOOP

	PRINT_SUM:						; DISPLAY THE FINAL TOTAL AMOUNT
		lea		eax, msgTotalAmount
		push	eax
		call	printf				; display the final sum message
		pop		eax

		push	totalAmount
		lea		eax, format
		push	eax
		call	printf				; display the totalAmount number
		add		esp, 8

	WAIT_EXIT:						; PROMT THE USER TO ENTER ANY INTEGER TO EXIT THE PROGRAMME(unused label)
		lea		eax, msgIntegerToExit
		push	eax
		call	printf				; print exit message
		pop		eax

		lea		eax, waitUserInput
		push	eax
		lea		eax, format
		push	eax					; wait for user input before exiting the programme, otherwise
		call	scanf_s				; the programme does not allow the user to see the result
		add		esp, 8				; because it closes immediately
	}
}