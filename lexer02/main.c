#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOREACH_TOKEN(TOKEN)                                                                       \
  TOKEN(Soma)                                                                                      \
  TOKEN(Subtracao)                                                                                 \
  TOKEN(Multiplicacao)                                                                             \
  TOKEN(Divisao)                                                                                   \
  TOKEN(Exponenciacao)                                                                             \
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

void addToken(HeadList *head, enum Token token, void *value, int valueSize) {
  head->size += 1;
  if (head->size == 1) {
    head->list = malloc(sizeof(TokenValue));
  } else {
    head->list = realloc(head->list, head->size * sizeof(TokenValue));
  }
  head->list[head->size - 1].token = token;
  head->list[head->size - 1].value = calloc((size_t)(valueSize + 1), sizeof(char));
  int i;
  for (i = 0; i < valueSize; i++) {
    ((char *)(head->list[head->size - 1].value))[i] = ((char *)value)[i];
  }
  ((char *)(head->list[head->size - 1].value))[i] = '\0';
}

void tokenizar(HeadList *head, char *s) {
  for (int i = 0; s[i] != '\0'; i++) {
    printf("i = %d\n", i);
    switch (s[i]) {
    case '+':
      addToken(head, Soma, s + i, 1);
      break;
    case '-':
      addToken(head, Subtracao, s + i, 1);
      break;
    case '*':
      if (s[i + 1] == '*') {
        addToken(head, Exponenciacao, s + i, 2);
        i++;
      } else {
        addToken(head, Multiplicacao, s + i, 1);
      }
      break;
    case '/':
      addToken(head, Divisao, &s[i], 1);
      break;
    default:
      if (!isspace(s[i]))
        addToken(head, Indefinido, s + i, 1);
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
    strcpy(str + *iStr, TOKEN_STRING[head->list[iList].token]);
    *iStr += strlen(TOKEN_STRING[head->list[iList].token]);

    putInString(str, iStr, '(');
    putInString(str, iStr, '\n');
    putInString(str, iStr, '\t');
    putInString(str, iStr, '\t');

    putInString(str, iStr, '\'');
    strcpy(str + *iStr, ((char *)(head->list[iList].value)));
    *iStr += strlen(((char *)(head->list[iList].value)));
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

  char *program = (char *)calloc(sizeof(char), (fileSize + 1));
  validateMalloc(program);

  size_t resultSize = fread(program, 1, fileSize, file);
  if (resultSize != fileSize)
    throwError("Reading file");

  fclose(file);

  char *str = calloc(1000, sizeof(char));
  tokenizar(head, program);
  printf("TOKENS: %s", printTokenList(head, str));

  free(program);
  freeHeadList(head);
  free(str);
  return 0;
}