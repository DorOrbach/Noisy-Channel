#ifndef NOISE_H
#define NOISE_H

char* generate_noise(char* rec_message, char* args[], int* errors);
void create_random_noise(char* data, int probabillity, int seed, int* errors);
int reverse_bits(char* data, int next_bit_to_reverse);


#endif // NOISE_H
