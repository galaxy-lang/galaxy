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
  %newline_ptr = getelementptr [1 x i8], [1 x i8]* @new_line, i64 0, i64 0
  
  ; Call the write syscall again to print the newline
  %return = call i64 @write(i32 1, ptr %newline_ptr, i64 1)  ; Write the newline to stdout
  
  ret i64 %return  ; Return from the writeln function
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

define ptr @strrep(ptr %input_str, i32 %repeat_count) {
entry:
  ; Allocate space for intermediate variables
  %output_buffer = alloca ptr, align 8
  %string_ptr = alloca ptr, align 8
  %repeat_index = alloca i32, align 4
  %temp_index = alloca i32, align 4

  ; Initialize variables
  store ptr %input_str, ptr %string_ptr, align 8
  store i32 0, ptr %repeat_index, align 4
  store i32 0, ptr %temp_index, align 4

  br label %check_repeat_loop

check_repeat_loop:                                           ; Check if repetition loop is complete
  %repeat_count_val = load i32, ptr %repeat_index, align 4
  %is_repeat_done = icmp sge i32 %repeat_count_val, %repeat_count
  br i1 %is_repeat_done, label %finalize_buffer, label %copy_to_temp_buffer

copy_to_temp_buffer:                                         ; Copy characters to the buffer
  %current_str_ptr = load ptr, ptr %string_ptr, align 8
  %temp_index_val = load i32, ptr %temp_index, align 4
  %char_pos = sext i32 %temp_index_val to i64
  %char_ptr = getelementptr inbounds i8, ptr %current_str_ptr, i64 %char_pos
  %char_val = load i8, ptr %char_ptr, align 1
  %is_non_null = icmp ne i8 %char_val, 0
  br i1 %is_non_null, label %continue_copying, label %increment_repeat_index

continue_copying:                                            ; Copy single character to the temp buffer
  %temp_offset = load i32, ptr %temp_index, align 4
  %buffer_pos = getelementptr inbounds [1024 x i8], ptr @strrep.temp_buffer, i64 0, i32 %temp_offset
  store i8 %char_val, ptr %buffer_pos, align 1
  %next_index = add nsw i32 %temp_offset, 1
  store i32 %next_index, ptr %temp_index, align 4
  br label %copy_to_temp_buffer

increment_repeat_index:                                      ; Increment repeat index and reset temp index
  %next_repeat = load i32, ptr %repeat_index, align 4
  %incremented_repeat = add i32 %next_repeat, 1
  store i32 %incremented_repeat, ptr %repeat_index, align 4
  store i32 0, ptr %temp_index, align 4
  br label %check_repeat_loop

finalize_buffer:                                             ; Add null terminator and finalize buffer
  %final_size = load i32, ptr %temp_index, align 4
  %final_size64 = sext i32 %final_size to i64
  %final_ptr = getelementptr inbounds [1024 x i8], ptr @strrep.temp_buffer, i64 0, i64 %final_size64
  store i8 0, ptr %final_ptr, align 1
  store ptr @strrep.temp_buffer, ptr %output_buffer, align 8
  br label %return_buffer

return_buffer:                                               ; Return the buffer
  %final_output = load ptr, ptr %output_buffer, align 8
  ret ptr %final_output
}
