#include "utils.h"
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
        error("Decode string error: input string not beginning with a double quote.");
		return FAILURE;
	}
	/* "closed" tracks whether there is a trailing double quote at the end of string */
	int closed = 0;
	int length = 0;
	while (*es) {
		if (length > MAXSTRLEN) {
			/* decoded string has exceeded max allowed length*/
            error("Decode string error: decoded string has exceeded max allowed length.");
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
						char* endptr;
						unsigned int byteValue = strtoul(es, &endptr, 16);
						if (endptr != es+2) {
							/* failed to read 2 digit hex value following \0x */
                            error("Decode string error: failed to read \\0xHH hex value.");
							return FAILURE;
						};
						es = endptr;
						*s++ = (char)byteValue;
					} else {
						*s++ = escapeMap[i].character;
					}
					break;
				}
			}
			/* found will be 0 if escape char invalid or backslash not followed by char */
			if (!found) {
                error("Decode string error: backslash followed by invalid character.");
				return FAILURE;
			}
		} else if (*es == '\"') {
			if (*(es+sizeof(char)) != '\0') {
				/* middle-of-string double quote not escaped, invalid*/
                error("Decode string error: double quote in middle of string not escaped.");
				return FAILURE;
			}
			/* reached the end of string */
			closed = 1;
			goto end_of_string;
		} else {
			if ((int)*es < MINASCII || (int)*es > MAXASCII) {
				/* non-escaped character not in permitted ASCII range */
                error("Decode string error: non-escaped character not in permitted ASCII range.");
				return FAILURE;
			}
			/* append ordinary character to decoded string */
			*s++ = *es++;
		}
		length++;
	}
end_of_string:
	if (!closed) {
        error("Decode string error: input string not closed by double quote.");
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
			sprintf(es, "\\0x%02x", ch);
			es += 5*sizeof(char);
		} else {
			/* ordinary char inside printable ASCII range */
			*es++ = *s++;
		}
	}
	strcpy(es, "\"");
	return SUCCESS;
};
