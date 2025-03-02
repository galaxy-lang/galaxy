@new_line = constant [1 x i8] c"\0A"  ; Define a constant representing the newline character (0x0A)

declare i64 @write(i32, ptr, i64) ; Declare the write syscall, which takes a file descriptor (i32), a pointer to data (ptr), and the length of the data (i64)

define i64 @writeln(ptr %str) {
entry:
  %len = call i64 @strlen(ptr %str)             ; Calcula o comprimento da string
  call i64 @write(i32 1, ptr %str, i64 %len)    ; Escreve a string no stdout
  %newline_ptr = getelementptr [1 x i8], ptr @new_line, i64 0, i64 0
  call i64 @write(i32 1, ptr %newline_ptr, i64 1) ; Escreve a nova linha
  ret i64 0
}

define i64 @strlen(ptr %input_str) {
entry:
  %start_ptr = ptrtoint ptr %input_str to i64  ; Converte o ponteiro inicial para um inteiro
  br label %loop_head

loop_head:
  %current_ptr = phi ptr [ %input_str, %entry ], [ %next_ptr, %loop_body ]  ; Ponteiro atual
  %char = load i8, ptr %current_ptr, align 1  ; Carrega o caractere atual
  %is_null = icmp eq i8 %char, 0              ; Verifica se é o caractere nulo
  br i1 %is_null, label %done, label %loop_body

loop_body:
  %next_ptr = getelementptr inbounds i8, ptr %current_ptr, i64 1  ; Avança para o próximo caractere
  br label %loop_head

done:
  %final_ptr = ptrtoint ptr %current_ptr to i64  ; Converte o ponteiro final para um inteiro
  %length = sub i64 %final_ptr, %start_ptr       ; Calcula o comprimento da string
  ret i64 %length
}

@strrep.temp_buffer = internal global [1024 x i8] zeroinitializer, align 16
; Global buffer to store the repeated string, with a capacity of 1024 bytes.

define ptr @strrep(ptr %string_input, i32 %repeat_count) {
entry:
  %buffer_ptr = alloca ptr, align 8
  ; Pointer to the buffer that will hold the resulting repeated string.

  %input_ptr = alloca ptr, align 8
  ; Pointer to the input string.

  %repeat_val = alloca i32, align 4
  ; The number of repetitions requested.

  %index_i = alloca i32, align 4
  ; Index to control the outer repetition loop.

  %index_j = alloca i32, align 4
  ; Index to iterate over characters of the input string.

  %current_index = alloca i32, align 4
  ; Index to track the current position in the output buffer.

  %output_index = alloca i32, align 4
  ; Index to count the number of characters in the input string.

  store ptr %string_input, ptr %input_ptr, align 8
  ; Store the input string pointer.

  store i32 %repeat_count, ptr %repeat_val, align 4
  ; Store the requested repeat count.

  store i32 0, ptr %current_index, align 4
  ; Initialize the current output buffer index to 0.

  store i32 0, ptr %output_index, align 4
  ; Initialize the input string character counter to 0.

  br label %loop_check
  ; Jump to the loop that counts the input string length.

loop_check: ; preds = %repeat_increment, %entry
  %string_start = load ptr, ptr %input_ptr, align 8
  ; Load the pointer to the start of the input string.

  %current_char_index = load i32, ptr %output_index, align 4
  ; Load the current index in the input string.

  %current_char_offset = sext i32 %current_char_index to i64
  ; Extend the character index to 64 bits for pointer arithmetic.

  %current_char_ptr = getelementptr inbounds i8, ptr %string_start, i64 %current_char_offset
  ; Compute the pointer to the current character in the input string.

  %current_char = load i8, ptr %current_char_ptr, align 1
  ; Load the current character.

  %char_val = sext i8 %current_char to i32
  ; Extend the character to 32 bits for comparison.

  %is_not_null = icmp ne i32 %char_val, 0
  ; Check if the current character is not null.

  br i1 %is_not_null, label %repeat_increment, label %size_check
  ; If the character is not null, continue counting; otherwise, check the size.

repeat_increment: ; preds = %loop_check
  %increment_index = load i32, ptr %output_index, align 4
  ; Load the current input string index.

  %incremented_index = add nsw i32 %increment_index, 1
  ; Increment the index.

  store i32 %incremented_index, ptr %output_index, align 4
  ; Store the updated index.

  br label %loop_check
  ; Repeat the loop to check the next character.

size_check: ; preds = %loop_check
  %num_repeats = load i32, ptr %repeat_val, align 4
  ; Load the number of repetitions.

  %string_length = load i32, ptr %output_index, align 4
  ; Load the length of the input string.

  %total_size = mul nsw i32 %num_repeats, %string_length
  ; Calculate the total size required for the repeated string.

  %exceeds_limit = icmp sge i32 %total_size, 1024
  ; Check if the total size exceeds the buffer limit.

  br i1 %exceeds_limit, label %abort, label %copy_init
  ; If the limit is exceeded, abort; otherwise, start copying.

abort: ; preds = %size_check
  store ptr null, ptr %buffer_ptr, align 8
  ; Store null in the buffer pointer to indicate failure.

  br label %exit
  ; Exit the function.

copy_init: ; preds = %size_check
  store i32 0, ptr %index_i, align 4
  ; Initialize the outer loop index to 0.

  br label %repeat_loop
  ; Begin the outer loop to repeat the string.

repeat_loop: ; preds = %copy_end, %copy_init
  %current_repeat = load i32, ptr %index_i, align 4
  ; Load the current repetition count.

  %repeats_left = load i32, ptr %repeat_val, align 4
  ; Load the total number of repetitions.

  %has_more_repeats = icmp slt i32 %current_repeat, %repeats_left
  ; Check if there are more repetitions to perform.

  br i1 %has_more_repeats, label %inner_copy_init, label %finalize
  ; If more repetitions are needed, begin the inner copy; otherwise, finalize.

inner_copy_init: ; preds = %repeat_loop
  store i32 0, ptr %index_j, align 4
  ; Initialize the character index for the inner loop.

  br label %inner_copy
  ; Start copying characters from the input string.

inner_copy: ; preds = %inner_copy_increment, %inner_copy_init
  %copy_index = load i32, ptr %index_j, align 4
  ; Load the current character index in the input string.

  %string_len = load i32, ptr %output_index, align 4
  ; Load the input string length.

  %has_chars_left = icmp slt i32 %copy_index, %string_len
  ; Check if there are more characters to copy.

  br i1 %has_chars_left, label %copy_char, label %inner_copy_end
  ; If more characters remain, copy the next one; otherwise, end the inner loop.

copy_char: ; preds = %inner_copy
  %source_string = load ptr, ptr %input_ptr, align 8
  ; Load the pointer to the input string.

  %char_index = load i32, ptr %index_j, align 4
  ; Load the current character index.

  %char_offset = sext i32 %char_index to i64
  ; Extend the index to 64 bits for pointer arithmetic.

  %source_char_ptr = getelementptr inbounds i8, ptr %source_string, i64 %char_offset
  ; Get the pointer to the current character in the input string.

  %source_char = load i8, ptr %source_char_ptr, align 1
  ; Load the current character from the input string.

  %output_pos = load i32, ptr %current_index, align 4
  ; Load the current position in the output buffer.

  %next_output_pos = add nsw i32 %output_pos, 1
  ; Calculate the next position in the output buffer.

  store i32 %next_output_pos, ptr %current_index, align 4
  ; Update the current index for the output buffer.

  %output_offset = sext i32 %output_pos to i64
  ; Extend the output buffer index for pointer arithmetic.

  %output_char_ptr = getelementptr inbounds [1024 x i8], ptr @strrep.temp_buffer, i64 0, i64 %output_offset
  ; Get the pointer to the output buffer's current position.

  store i8 %source_char, ptr %output_char_ptr, align 1
  ; Store the current character in the output buffer.

  br label %inner_copy_increment
  ; Increment the inner loop index.

inner_copy_increment: ; preds = %copy_char
  %next_copy_index = load i32, ptr %index_j, align 4
  ; Load the current index of the inner loop.

  %incremented_copy_index = add nsw i32 %next_copy_index, 1
  ; Increment the inner loop index.

  store i32 %incremented_copy_index, ptr %index_j, align 4
  ; Store the updated inner loop index.

  br label %inner_copy
  ; Repeat the inner loop.

inner_copy_end: ; preds = %inner_copy
  br label %copy_end
  ; Exit the inner loop.

copy_end: ; preds = %inner_copy_end
  %increment_repeat = load i32, ptr %index_i, align 4
  ; Load the current repetition index.

  %next_repeat = add nsw i32 %increment_repeat, 1
  ; Increment the repetition index.

  store i32 %next_repeat, ptr %index_i, align 4
  ; Store the updated repetition index.

  br label %repeat_loop
  ; Repeat the outer loop.

finalize: ; preds = %repeat_loop
  %final_index = load i32, ptr %current_index, align 4
  ; Load the final position in the output buffer.

  %final_offset = sext i32 %final_index to i64
  ; Extend the final index for pointer arithmetic.

  %null_terminator_ptr = getelementptr inbounds [1024 x i8], ptr @strrep.temp_buffer, i64 0, i64 %final_offset
  ; Get the pointer to the position for the null terminator.

  store i8 0, ptr %null_terminator_ptr, align 1
  ; Store the null terminator in the output buffer.

  store ptr @strrep.temp_buffer, ptr %buffer_ptr, align 8
  ; Store the buffer pointer in the return variable.

  br label %exit
  ; Exit the function.

exit: ; preds = %finalize, %abort
  %result_ptr = load ptr, ptr %buffer_ptr, align 8
  ; Load the result pointer.

  ret ptr %result_ptr
  ; Return the pointer to the repeated string (or null if aborted).
}

declare ptr @malloc(i64)

declare void @free(ptr)

declare i32 @sprintf(ptr %buffer, ptr %format, ...) #1

define ptr @itos(i32 %x) {
entry:
  ; Aloca espaço para o buffer de resultado no heap
  %buffer = call ptr @malloc(i64 128)
  
  ; Define a constante local para o formato "%d"
  %format = alloca [4 x i8], align 1
  store [3 x i8] c"%d\00", ptr %format

  ; Chama o sprintf com a string de formato
  call i32 @sprintf(ptr %buffer, ptr %format, i32 %x)

  ; Retorna o resultado final (buffer) com a string
  ret ptr %buffer
}

define ptr @ftos(double %x) {
entry:
  ; Aloca espaço para o buffer de resultado no heap
  %buffer = call ptr @malloc(i64 128)
  
  ; Define a constante local para o formato "%f"
  %format = alloca [4 x i8], align 1
  store [3 x i8] c"%f\00", ptr %format

  ; Chama o sprintf com a string de formato
  call i32 @sprintf(ptr %buffer, ptr %format, double %x)

  ; Retorna o resultado final (buffer) com a string
  ret ptr %buffer
}