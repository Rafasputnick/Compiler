#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Token { OPERADOR, INDEFINIDO };

typedef struct TokensValues {
	enum Token token;
	void *value;
} TokenValue;

typedef struct HeadsLists {
	uint64_t size;
	TokenValue *list;
} HeadList;

bool isOperator(char c) { return c == '+' || c == '-'; }

void addToken(HeadList *head, enum Token token, void *value) {
	head->size += 1;
	if (head->size == 1) {
		head->list = malloc(sizeof(TokenValue));
	} else {
		head->list = realloc(head->list, head->size * sizeof(TokenValue));
	}

	if (token == OPERADOR) {
		head->list[head->size - 1].token = OPERADOR;
		head->list[head->size - 1].value = calloc(3, sizeof(char));
		((char *)(head->list[head->size - 1].value))[0] = ((char *)value)[0];
		((char *)(head->list[head->size - 1].value))[1] = '\0';

	} else {
		head->list[head->size - 1].token = INDEFINIDO;
		head->list[head->size - 1].value = calloc(3, sizeof(char));
		((char *)(head->list[head->size - 1].value))[0] = ((char *)value)[0];
		((char *)(head->list[head->size - 1].value))[1] = '\0';
	}
}

void tokenizar(HeadList *head, char *s) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (isOperator(s[i])) {
			addToken(head, OPERADOR, &s[i]);
		} else {
			addToken(head, INDEFINIDO, &s[i]);
		}
	}
}

void getTokenStr(enum Token token, char *str) {
	switch (token) {
	case OPERADOR:
		strcpy(str, "Operador");
		break;
	case INDEFINIDO:
		strcpy(str, "Indefinido");
		break;
	default:
		break;
	}
}

void putInString(char *str, int *i, char value) {
	str[*i] = value;
	(*i)++;
}

char *printTokenList(HeadList *head, char *str) {
	uint64_t iList;
	char *tokenStr = calloc(25, sizeof(char));
	int *iStr = malloc(sizeof(int));
	*iStr = 0;
	putInString(str, iStr, '[');
	putInString(str, iStr, '\n');

	for (iList = 0; iList < head->size; iList++) {
		getTokenStr(head->list[iList].token, tokenStr);
		putInString(str, iStr, '\t');

		strcpy(str + *iStr, tokenStr);
		*iStr += strlen(tokenStr);

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

	free(tokenStr);
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

int main(void) {
	HeadList *head = createHeadList();

	char *str = calloc(1000, sizeof(char));
	tokenizar(head, "++- +");
	printf("TOKENS: %s", printTokenList(head, str));

	freeHeadList(head);
	free(str);
	return 0;
}