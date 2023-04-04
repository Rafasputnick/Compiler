#include <ctype.h>
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
  TOKEN(Exponenciacao)                                                                                                                                                                                 \
  TOKEN(Inteiros)                                                                                                                                                                                      \
  TOKEN(Reais)                                                                                                                                                                                         \
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

void addToken(HeadList *head, enum Token token, void *value, size_t valueSize) {
  head->size += 1;
  if (head->size == 1) {
    head->list = malloc(sizeof(TokenValue));
  } else {
    head->list = realloc(head->list, head->size * sizeof(TokenValue));
  }
  head->list[head->size - 1].token = token;
  head->list[head->size - 1].value = calloc(valueSize + 1, sizeof(char));
  size_t i;
  for (i = 0; i < valueSize; i++) {
    ((char *)(head->list[head->size - 1].value))[i] = ((char *)value)[i];
  }
}

void tokenizar(HeadList *head, char *s) {
  for (int i = 0; s[i] != '\0'; i++) {
    if (isdigit(s[i])) {
      bool point = false;
      bool jump = false;
      char *numStr = calloc(sizeof(char), 25);
      int iStr = 0;
      numStr[iStr] = s[i];
      iStr++;
      while (isdigit(s[i + 1]) || s[i + 1] == '.') {
        i++;
        numStr[iStr] = s[i];
        iStr++;
        if (s[i] == '.') {
          if (point) {
            jump = true;
          } else {
            point = true;
          }
        }
      }
      if (!jump) {
        if (point) {
          // Reais
          float num = atof(numStr);
          addToken(head, Reais, &num, sizeof(float));
        } else {
          // Inteiros
          int num = atoi(numStr);
          addToken(head, Inteiros, &num, sizeof(int));
        }
      } else {
        addToken(head, Indefinido, numStr, strlen(numStr));
      }
      free(numStr);
    } else {
      switch (s[i]) {
      case '+':
        addToken(head, Soma, s + i, sizeof(char));
        break;
      case '-':
        addToken(head, Subtracao, s + i, sizeof(char));
        break;
      case '*':
        if (s[i + 1] == '*') {
          // **
          addToken(head, Exponenciacao, s + i, sizeof(char) * 2);
          i++;
        } else {
          // *
          addToken(head, Multiplicacao, s + i, sizeof(char));
        }
        break;
      case '/':
        addToken(head, Divisao, s + i, sizeof(char));
        break;
      default:
        if (!isspace(s[i]))
          addToken(head, Indefinido, s + i, sizeof(char));
        break;
      }
    }
  }
}

void putInString(char *target, int *iTarget, char *source) {
  for(size_t i = 0; i < strlen(source); i++){
	target[*iTarget] = source[i];
  	(*iTarget)++;	  
  }
}

bool isNumberToken(HeadList *head, int index){
	return head->list[index].token == Inteiros || head->list[index].token == Reais;
}

char *printTokenList(HeadList *head, char *str) {
  uint64_t iList;
  int *iStr = malloc(sizeof(int));
  *iStr = 0;
  putInString(str, iStr, "[\n");
  bool isNumber = false;
  for (iList = 0; iList < head->size; iList++) {
    isNumber = isNumberToken(head, iList);
    
	putInString(str, iStr, "\t");
	strcpy(str + *iStr, TOKEN_STRING[head->list[iList].token]);
    *iStr += strlen(TOKEN_STRING[head->list[iList].token]);

    putInString(str, iStr, "(\n\t\t");
    
    if (isNumber) {
      char *numStr = calloc(sizeof(char), 25);
      if (head->list[iList].token == Inteiros) {
        sprintf(numStr, "%d", *((int *)(head->list[iList].value)));
      } else {
        sprintf(numStr, "%f", *((float *)(head->list[iList].value)));
      }
      strcpy(str + *iStr, numStr);
      *iStr += strlen(numStr);
      free(numStr);
    } else {
      putInString(str, iStr, "\'");
      strcpy(str + *iStr, ((char *)(head->list[iList].value)));
      *iStr += strlen(((char *)(head->list[iList].value)));
      putInString(str, iStr, "\'");
    }

    putInString(str, iStr, ",\n\t),\n");
  }
  putInString(str, iStr, "]\n");

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

float number(HeadList *head, int index){
	if (isNumberToken(head,index)){
		if(head->list[index].token == Inteiros){
			return *((int *)(head->list[index].value)) * 1.0;
		}
		return *((float *)(head->list[index].value));
	}
	throwError("Invalid number");
	return 0;
}

float term(HeadList *head, int index, float res){
	if(head->size == 1) return *((float *)(head->list[0].value));
	if(head->size == 2) throwError("Invalid code");
	res += number(head, index);
	res += number(head, index + 2);
	return res;
}

float expression(HeadList *head, int index){
	float res = 0;
	if(head->size == 0) return res;
	if(head->size > 3) throwError("Invalid code");
	return term(head, index, res);
}

float parser(HeadList *head){
	float res = 0;
	res += expression(head, 0);
	return res;
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
	printf("%f\n", parser(head));

  free(program);
  freeHeadList(head);
  free(str);
  return 0;
}