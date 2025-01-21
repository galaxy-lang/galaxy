@new_line = constant [1 x i8] c"\0A"  ; Define a constant representing the newline character (0x0A)

declare i64 @write(i32, ptr, i64) ; Declare the write syscall, which takes a file descriptor (i32), a pointer to data (ptr), and the length of the data (i64)

; Implementation of writeln
define i64 @writeln(ptr %str) {
entry:
  ; Calculate the length of the string by calling strlen
  %len = call i64 @strlen(ptr %str)
  
  ; Call the write syscall to print the string
  call i64 @write(i32 1, ptr %str, i64 %len)  ; Write the string to stdout (file descriptor 1)
  
  ; Get the pointer to the newline character
  %newline_ptr = getelementptr [1 x i8], ptr @new_line, i64 0, i64 0
  
  ; Call the write syscall again to print the newline
  %return = call i64 @write(i32 1, ptr %newline_ptr, i64 1)  ; Write the newline to stdout
  
  ret i64 0
}

define i64 @strlen(ptr %input_str) {
entry:
  ; Allocate memory to store the starting pointer of the string
  %start_ptr = alloca ptr, align 8
  ; Allocate memory to store the current pointer for iteration
  %current_ptr = alloca ptr, align 8

  ; Store the input string pointer in start_ptr
  store ptr %input_str, ptr %start_ptr, align 8
  ; Load the initial pointer and store it in current_ptr for iteration
  %loaded_start = load ptr, ptr %start_ptr, align 8
  store ptr %loaded_start, ptr %current_ptr, align 8

  ; Jump to the loop head to start processing the string
  br label %loop_head

loop_head:                                         ; preds = %loop_body, %entry
  ; Load the current pointer to get the character it points to
  %current_char_ptr = load ptr, ptr %current_ptr, align 8
  ; Load the character at the current pointer
  %current_char = load i8, ptr %current_char_ptr, align 1
  ; Check if the character is not the null terminator (0)
  %current_char_as_int = sext i8 %current_char to i32

  %is_not_null = icmp ne i32 %current_char_as_int, 0
  ; If not null, continue the loop; otherwise, exit
  br i1 %is_not_null, label %loop_body, label %done

loop_body:                                         ; preds = %loop_head
  ; Load the current pointer
  %current_ptr_loaded = load ptr, ptr %current_ptr, align 8
  ; Get the pointer to the next character in the string
  %next_char_ptr = getelementptr inbounds i8, ptr %current_ptr_loaded, i32 1
  ; Store the updated pointer back to current_ptr
  store ptr %next_char_ptr, ptr %current_ptr, align 8
  ; Go back to the loop head to process the next character
  br label %loop_head

done:                                              ; preds = %loop_head
  ; Load the final pointer after reaching the null terminator
  %final_ptr = load ptr, ptr %current_ptr, align 8
  ; Load the initial pointer stored in start_ptr
  %initial_ptr = load ptr, ptr %start_ptr, align 8
  ; Convert the final pointer to an integer
  %final_ptr_as_int = ptrtoint ptr %final_ptr to i64
  ; Convert the initial pointer to an integer
  %initial_ptr_as_int = ptrtoint ptr %initial_ptr to i64
  ; Subtract the initial pointer value from the final pointer value to get the string length
  %string_length = sub i64 %final_ptr_as_int, %initial_ptr_as_int
  ; Return the calculated string length
  ret i64 %string_length
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

define ptr @itos(i64 %num) {
entry:
    ; Alocação de variáveis
    %is_negative = alloca i1
    %num_ptr = alloca i64
    %buffer = alloca ptr
    %i = alloca i64

    ; Inicializa variáveis
    store i64 %num, ptr %num_ptr
    store i1 0, ptr %is_negative

    ; Aloca buffer dinâmico para a string (máximo 12 bytes: "-2147483648" + '\0')
    %buf = call ptr @malloc(i64 12)
    store ptr %buf, ptr %buffer
    store i64 0, ptr %i

    ; Verifica se o número é negativo
    %num_val = load i64, ptr %num_ptr
    %is_neg = icmp slt i64 %num_val, 0
    br i1 %is_neg, label %negative, label %positive

negative: ; Bloco para números negativos
    store i1 1, ptr %is_negative
    %neg_num = sub i64 0, %num_val
    store i64 %neg_num, ptr %num_ptr
    br label %extract_digits

positive: ; Continua o processamento
    br label %extract_digits

extract_digits: ; Extrai os dígitos
    %num_val2 = load i64, ptr %num_ptr
    %digit = urem i64 %num_val2, 10
    %char_digit = add i64 %digit, 48 ; Converte para caractere
    %i_val = load i64, ptr %i
    %buf_ptr = load ptr, ptr %buffer
    %str_loc = getelementptr i8, ptr %buf_ptr, i64 %i_val
    store i64 %char_digit, ptr %str_loc

    ; Atualiza número e índice
    %next_num = sdiv i64 %num_val2, 10
    store i64 %next_num, ptr %num_ptr
    %next_i = add i64 %i_val, 1
    store i64 %next_i, ptr %i

    ; Verifica se o número é maior que zero
    %is_not_zero = icmp ne i64 %next_num, 0
    br i1 %is_not_zero, label %extract_digits, label %check_negative

check_negative: ; Adiciona o sinal de menos se necessário
    %is_neg2 = load i1, ptr %is_negative
    br i1 %is_neg2, label %add_minus, label %reverse_buffer

add_minus: ; Adiciona o sinal de menos
    %i_val2 = load i64, ptr %i
    %buf_ptr2 = load ptr, ptr %buffer
    %str_loc2 = getelementptr i8, ptr %buf_ptr2, i64 %i_val2
    store i8 45, ptr %str_loc2 ; '-' é 45 na tabela ASCII
    %next_i2 = add i64 %i_val2, 1
    store i64 %next_i2, ptr %i
    br label %reverse_buffer

reverse_buffer: ; Reverte o buffer
    %buf_ptr3 = load ptr, ptr %buffer
    %len = load i64, ptr %i

    ; Índices para reversão
    %start = alloca i64
    %end = alloca i64
    store i64 0, ptr %start
    %end_idx = sub i64 %len, 1
    store i64 %end_idx, ptr %end

    br label %reverse_loop

reverse_loop: ; Loop para inverter os caracteres
    %start_idx = load i64, ptr %start
    %end_idx_initial = load i64, ptr %end
    %cmp = icmp slt i64 %start_idx, %end_idx_initial
    br i1 %cmp, label %swap_chars, label %finalize_string

swap_chars: ; Troca os caracteres
    %buf_ptr4 = load ptr, ptr %buffer

    ; Posições dos caracteres
    %start_char_ptr = getelementptr i8, ptr %buf_ptr4, i64 %start_idx
    %end_char_ptr = getelementptr i8, ptr %buf_ptr4, i64 %end_idx

    ; Carrega os valores
    %start_char = load i8, ptr %start_char_ptr
    %end_char = load i8, ptr %end_char_ptr

    ; Troca os valores
    store i8 %end_char, ptr %start_char_ptr
    store i8 %start_char, ptr %end_char_ptr

    ; Atualiza os índices
    %next_start = add i64 %start_idx, 1
    %prev_end = sub i64 %end_idx, 1
    store i64 %next_start, ptr %start
    store i64 %prev_end, ptr %end

    br label %reverse_loop

finalize_string: ; Finaliza a string
    %i_val3 = load i64, ptr %i
    %buf_ptr5 = load ptr, ptr %buffer
    %str_loc3 = getelementptr i8, ptr %buf_ptr5, i64 %i_val3
    store i8 0, ptr %str_loc3 ; Terminador nulo
    %final_buf = load ptr, ptr %buffer

    ; Libera a memória alocada
    call void @free(ptr %final_buf)

    ret ptr %final_buf
}

declare i32 @sprintf(ptr %buffer, ptr %format, ...) #1

define ptr @ftos(double %x) {
entry:
  ; Aloca espaço para o buffer de resultado
  %buffer = alloca [128 x i8], align 1
  
  ; Define a constante local para o formato "%f"
  %format = alloca [4 x i8], align 1
  store [3 x i8] c"%f\00", ptr %format

  ; Chama o sprintf com a string de formato
  call i32 @sprintf(ptr %buffer, ptr %format, double %x)

  ; Retorna o resultado final (buffer) com a string
  ret ptr %buffer
}
