#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#define ENCRYPTED_LINE_LEN 249
#define LINE_LEN 209
#define BYTE_SIZE 8
#define HAMMING_LEN 32
#define ORIGINAL_LEN 26
#define ASCII_OFFSET 48

char* convert_string_to_binary(const char* str);
char* message_encryption(char* bin_message);
char* convert_bin_to_string(char* bin_str);

#endif // !ENCRYPTION_H
