#include "bmp.h"
#include "playfair.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){
    char *reverse_char = reverse("Dilino velky si!");
    printf("%s\n", reverse_char);
    free(reverse_char);

    char *encrypted = vigenere_encrypt("Key", "DanoDrevo");
    printf("%s\n", encrypted);
    char *decrypted = vigenere_decrypt("Key", encrypted);
    printf("%s\n", decrypted);
    free(encrypted);
    free(decrypted);

    unsigned char* encryptedChar = bit_encrypt("Hello World!");
    //for(int i = 0; i < 12; i++){
    //    printf("%x\n", encryptedChar[i]);
    //}
    char* decryptedChar = bit_decrypt(encryptedChar);
    printf("%s\n", decryptedChar);
    free(decryptedChar);
    free(encryptedChar);

    playfair_encrypt("key", "text");
    playfair_decrypt("key", "text");

    return 0;
}
