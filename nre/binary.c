#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM 26

int main() {
  char buffer[TAM];
  // Testar todas funcoes
  char matrix[TAM][4] = {"42", "1", "0",  "16", "0",   "32",  "0",  "48", "0",
                         "64", "0", "80", "0",  "96",  "128", "14", "3",  "144",
                         "17", "0", "32", "1",  "160", "22",  "0",  "240"};

  for (int i = 0; i < TAM; i++) {
    buffer[i] = (uint8_t)atoi(matrix[i]);
  }
  FILE *arq = fopen("entrada.bin", "wb");
  fwrite(buffer, 1, TAM, arq);
  return 0;
}