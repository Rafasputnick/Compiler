#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOREACH_TOKEN(TOKEN)                                                                                                                                                                           \
  TOKEN(Soma)                                                                                                                                                                                          \
  TOKEN(Subtracao)                                                                                                                                                                                     \
  TOKEN(Multiplicacao)                                                                                                                                                                                 \
  TOKEN(Divisao)                                                                                                                                                                                       \
  TOKEN(Indefinido)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum Token { FOREACH_TOKEN(GENERATE_ENUM) };

static const char *TOKEN_STRING[] = {FOREACH_TOKEN(GENERATE_STRING)};

typedef struct TokensValues {
  enum Token token;
  void *value;
} TokenValue;

typedef struct HeadsLists {
  uint64_t size;
  TokenValue *list;
} HeadList;

void addToken(HeadList *head, enum Token token, void *value) {
  head->size += 1;
  if (head->size == 1) {
    head->list = malloc(sizeof(TokenValue));
  } else {
    head->list = realloc(head->list, head->size * sizeof(TokenValue));
  }

  head->list[head->size - 1].token = token;
  head->list[head->size - 1].value = calloc(2, sizeof(char));
  ((char *)(head->list[head->size - 1].value))[0] = ((char *)value)[0];
  ((char *)(head->list[head->size - 1].value))[1] = '\0';
}

void tokenizar(HeadList *head, char *s) {
  for (int i = 0; s[i] != '\0'; i++) {
    switch (s[i]) {
    case '+':
      addToken(head, Soma, &s[i]);
      break;
    case '-':
      addToken(head, Subtracao, &s[i]);
      break;
    case '*':
      addToken(head, Multiplicacao, &s[i]);
      break;
    case '/':
      addToken(head, Divisao, &s[i]);
      break;
    default:
      addToken(head, Indefinido, &s[i]);
      break;
    }
  }
}

void putInString(char *str, int *i, char value) {
  str[*i] = value;
  (*i)++;
}

char *printTokenList(HeadList *head, char *str) {
  uint64_t iList;
  int *iStr = malloc(sizeof(int));
  *iStr = 0;
  putInString(str, iStr, '[');
  putInString(str, iStr, '\n');

  for (iList = 0; iList < head->size; iList++) {

    putInString(str, iStr, '\t');

    strcpy(str + *iStr, TOKEN_STRING[head->list[iList].token]);
    *iStr += strlen(TOKEN_STRING[head->list[iList].token]);

    putInString(str, iStr, '(');
    putInString(str, iStr, '\n');
    putInString(str, iStr, '\t');
    putInString(str, iStr, '\t');

    putInString(str, iStr, '\'');
    putInString(str, iStr, ((char *)(head->list[iList].value))[0]);
    putInString(str, iStr, '\'');

    putInString(str, iStr, ',');
    putInString(str, iStr, '\n');
    putInString(str, iStr, '\t');
    putInString(str, iStr, ')');

    putInString(str, iStr, ',');
    putInString(str, iStr, '\n');
  }
  putInString(str, iStr, ']');
  putInString(str, iStr, '\n');

  free(iStr);
  return str;
}

HeadList *createHeadList() {
  HeadList *head = malloc(sizeof(HeadList));
  head->size = 0;
  return head;
}

void freeHeadList(HeadList *head) {
  for (uint64_t i = 0; i < head->size; i++) {
    free(head->list[i].value);
  }
  free(head->list);
  free(head);
}

void throwError(char *message) {
  if (message == NULL || message[0] == '\0') {
    printf("Error: Unspecified\n");
  } else {
    printf("Error: %s\n", message);
  }
  exit(1);
}

void validateMalloc(void *pointer) {
  if (pointer == NULL) {
    throwError("Malloc return null");
  }
}

int main(int argc, char **argv) {
  if (argc < 2)
    throwError("Not found file path");

  HeadList *head = createHeadList();

  FILE *file = fopen(argv[1], "r");

  if (file == NULL)
    throwError("Opening file");

  fseek(file, 0, SEEK_END);
  uint16_t fileSize = ftell(file);
  rewind(file);

  char *program = (char *)malloc(sizeof(char) * fileSize);
  validateMalloc(program);

  size_t resultSize = fread(program, 1, fileSize, file);
  if (resultSize != fileSize)
    throwError("Reading file");

  fclose(file);

  char *str = calloc(1000, sizeof(char));
  tokenizar(head, program);
  printf("TOKENS: %s", printTokenList(head, str));

  freeHeadList(head);
  free(str);
  return 0;
}