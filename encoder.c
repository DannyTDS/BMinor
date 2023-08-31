#include "encoder.h"

EscapeMap escapeMap[] = {
    {"\\a", '\a'},
    {"\\b", '\b'},
    {"\\e", '\e'},
    {"\\f", '\f'},
    {"\\n", '\n'},
    {"\\r", '\r'},
    {"\\t", '\t'},
    {"\\v", '\v'},
    {"\\\\", '\\'},
    {"\\\'", '\''},
    {"\\\"", '\"'},
    {"\\0x", '\0'}
};

int string_decode(const char *es, char *s) {
	/* iterate over the encoded string */
	if (*es++ != '\"') {
		/* input string not beginning with a double quote */
        printf("Error: input string not beginning with a double quote.\n");
		return FAILURE;
	}
	/* "closed" tracks whether there is a trailing double quote at the end of string */
	int closed = 0;
	int length = 0;
	while (*es) {
		if (length > MAXSTRLEN) {
			/* decoded string has exceeded max allowed length*/
            printf("Error: decoded string has exceeded max allowed length.\n");
			return FAILURE;
		}
		if (*es == '\\') {
			/* check if escaped character is valid */
			int found = 0;
			for (size_t i=0; i<sizeof(escapeMap)/sizeof(escapeMap[0]); i++) {
				if (strncmp(es, escapeMap[i].sequence, strlen(escapeMap[i].sequence)) == 0) {
					found = 1;
					es += strlen(escapeMap[i].sequence);
					if (strcmp(escapeMap[i].sequence, "\\0x") == 0) {
						/* try to read in a 2 digit hex value */
						unsigned int byteValue;
						if (sscanf(es, "%2x", &byteValue) != 1) {
							/* failed to read 2 digit hex value following \0x */
                            printf("Error: failed to read \\0xHH hex value.\n");
							return FAILURE;
						};
						es += 2*sizeof(char);
						*s++ = (char)byteValue;
					} else {
						*s++ = escapeMap[i].character;
					}
					break;
				}
			}
			/* found will be 0 if escape char invalid or backslash not followed by char */
			if (!found) {
                printf("Error: backslash followed by invalid character.\n");
				return FAILURE;
			}
		} else if (*es == '\"') {
			if (*(es+sizeof(char)) != '\0') {
				/* middle-of-string double quote not escaped, invalid*/
                printf("Error: double quote in middle of string not escaped.\n");
				return FAILURE;
			}
			/* reached the end of string */
			closed = 1;
			goto end_of_string;
		} else {
			if ((int)*es < MINASCII || (int)*es > MAXASCII) {
				/* non-escaped character not in permitted ASCII range */
                printf("Error: non-escaped character not in permitted ASCII range.\n");
				return FAILURE;
			}
			/* append ordinary character to decoded string */
			*s++ = *es++;
		}
		length++;
	}
end_of_string:
	if (!closed) {
        printf("Error: input string not closed by double quote.\n");
		return FAILURE;
	} else {
		*s = '\0';
		return SUCCESS;
	}
};


int string_encode(const char *s, char *es) {
	*es++ = '\"';
	while (*s) {
		int found = 0;
		for (size_t i=0; i<sizeof(escapeMap)/sizeof(escapeMap[0]); i++) {
			if (*s == escapeMap[i].character) {
				found = 1;
				strcpy(es, escapeMap[i].sequence);
				s++;
				es += strlen(escapeMap[i].sequence);
				break;
			}
		}
		if (found) {
			continue;
		} else if ((int)*s < MINASCII || (int)*s > MAXASCII) {
			/* current char not printable */
			unsigned char ch = *s++;
			sprintf(es, "\\0x%2x", ch);
			es += 5*sizeof(char);
		} else {
			/* ordinary char inside printable ASCII range */
			*es++ = *s++;
		}
	}
	strcpy(es, "\"");
	return SUCCESS;
};
