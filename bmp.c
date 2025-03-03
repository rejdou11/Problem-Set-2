#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bmp.h"

char* reverse(const char* text){
    if(text == NULL){
        return NULL;
    }

    int text_len = strlen(text);
    char *reversedChar = (char *)malloc((text_len + 1) * sizeof(char));

    for(int i = 0; i < text_len; i++){
        reversedChar[i] = toupper(text[(text_len-1) - i]);
    }
    reversedChar[text_len] = '\0';
    return reversedChar;
}

bool generate_repeating_key(const char* key, char* repeating_key, int text_len, int key_len){
    for(int i = 0; i < text_len ; i++){
        if(!isalpha(key[i % key_len])){
            return false;
        }else{
            repeating_key[i] = toupper(key[i % key_len]);
        }
    }
    repeating_key[text_len] = '\0';
    return true;
}

char* vigenere_encrypt(const char* key, const char* text) {
    if(key == NULL || text == NULL){
        return NULL;
    }
    int text_len = strlen(text);
    int key_len = strlen(key);
    char repeating_key[text_len];

    if(!generate_repeating_key(key, repeating_key, text_len, key_len)){
        return NULL;
    }

    char *encrypted_key = (char *)malloc((text_len + 1) * sizeof(char));
    for(int i = 0; i < text_len; i++){
        char difference = 'C';
        if(isalpha(text[i])){
            difference = toupper(text[i]) + (repeating_key[i]-64);
        } else if(text[i] == ' '){
            difference = ' ';
        } else {
            return NULL;
        }

        if(difference > 90){
            difference -= 25;
        }
        encrypted_key[i] = difference; //65 po 90
    }
    encrypted_key[text_len] = '\0';
    return encrypted_key;
}

char* vigenere_decrypt(const char* key, const char* text){
    if(key == NULL || text == NULL){
        return NULL;
    }
    int text_len = strlen(text);
    int key_len = strlen(key);
    char repeating_key[text_len];

    if(!generate_repeating_key(key, repeating_key, text_len, key_len)){
        return NULL;
    }

    char *decrypted_key = (char *)malloc((text_len + 1) * sizeof(char));
    for(int i = 0; i < text_len; i++){
        char difference = 'C';
        if(isalpha(text[i])){
            difference = text[i] - (repeating_key[i]-64);
        } else if(text[i] == ' '){
            difference = ' ';
        } else {
            return NULL;
        }

        if(difference < 65 && difference != ' '){
            difference += 25;
        }
        decrypted_key[i] = difference;
    }
    decrypted_key[text_len] = '\0';
    return decrypted_key;
}

// 2nd program

int* ConvertCharToBits(char recievedChar){
    int *bits = (int *)malloc(8 * sizeof(int));
    for (int i = 0; i < 8; i++) {
        bits[i] = (recievedChar >> (7 - i)) & 1;
    }
    return bits;
}

void switch_bits(int* bits){
    int bit_buffer = bits[0];
    bits[0] = bits[1];
    bits[1] = bit_buffer;
    bit_buffer = bits[2];
    bits[2] = bits[3];
    bits[3] = bit_buffer;
}

void xor_bits(int *bits){
    for(int i = 0; i < 4; i++){
        if(bits[i] != bits[i+4]){
            bits[i+4] = 1;
        } else {
            bits[i+4] = 0;
        }
    }
}

unsigned char bits_to_unsigned_char(int bits[8]) {
    unsigned char result = 0;
    for (int i = 0; i < 8; i++) {
        result |= (bits[i] << (7 - i));
    }
    return result;
}

/*void print_arr(int *arr){
    for(int i = 0; i < 8; i++){
        printf("%d", arr[i]);
    }
    printf("\n");
}*/

unsigned char* bit_encrypt(const char* text){
    if(text == NULL){
        return NULL;
    }
    int text_len = strlen(text);
    unsigned char *encrypted_arr = (unsigned char *)malloc((text_len + 1) * sizeof(unsigned char));
    for (int i = 0; i < text_len; i++){
        int *bits = ConvertCharToBits(text[i]);
        switch_bits(bits);
        xor_bits(bits);
        encrypted_arr[i] = bits_to_unsigned_char(bits);
        free(bits);
    }
    encrypted_arr[text_len] = '\0';
    return encrypted_arr;
}

void reverse_xor(int *bits){
    for(int i = 0; i < 4; i++){
        if(bits[i+4] == 1){
            if(bits[i] == 1){
                bits[i+4] = 0;
            } else{
                bits[i+4] = 1;
            }
        } else {
            bits[i+4] = bits[i];
        }
    }
}

char bits_to_char(int bits[8]) {
    char result = 0;
    for (int i = 0; i < 8; i++) {
        result |= (bits[i] << (7 - i));
    }
    return result;
}

char* bit_decrypt(const unsigned char* text){
    if(text == NULL){
        return NULL;
    }
    int text_len = strlen((const char *)text);
    char *decrypted_arr = (char *)malloc((text_len + 1) * sizeof(char));
    for (int i = 0; i < text_len; i++){
        int *bits = ConvertCharToBits(text[i]);
        reverse_xor(bits);
        switch_bits(bits);
        decrypted_arr[i] = bits_to_char(bits);
        free(bits);
    }
    decrypted_arr[text_len] = '\0';
    return decrypted_arr;
}

unsigned char* bmp_encrypt(const char* key, const char* text){
    char *reversed_p = reverse(text);
    char *encrypted_vigenere_arr = vigenere_encrypt(key, reversed_p);
    free(reversed_p);
    unsigned char *bit_encrypt_arr = bit_encrypt(encrypted_vigenere_arr);
    free(encrypted_vigenere_arr);
    return bit_encrypt_arr;
}

char* bmp_decrypt(const char* key, const unsigned char* text){
    char *bit_decrypted_p = bit_decrypt(text);
    char *decrypted_vigenere_p = vigenere_decrypt(key, bit_decrypted_p);
    free(bit_decrypted_p);
    char *reversed_p = reverse(decrypted_vigenere_p);
    free(decrypted_vigenere_p);
    return reversed_p;
}
