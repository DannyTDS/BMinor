#include "utils.h"
#include "encoder.h"
#include "helper.h"

void usage(char* prog_name) {
    fprintf(stderr, "Usage: %s [options] <input.file>\n", prog_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t--encode\n");
    fprintf(stderr, "\t--scan\n");
    fprintf(stderr, "\t--parse\n");
    exit(FAILURE);
}

int invoke_encode(FILE *f) {
    /* decode and encode string */
    /* read input string */
    char input[BUFSIZ];
    if(!fgets(input, BUFSIZ, f)) {
        /* failed to read file */
        printf("Error: failed to read input files.\n");
        return(FAILURE);
    };
    /* remove trailing newline if one presents */
    if (input[strlen(input)-1] == '\n') {
        input[strlen(input)-1] = '\0';
    }
    /* excise the functions */
    char decoded[strlen(input)+1];
    if (string_decode(input, decoded) != SUCCESS) {
        printf("Cannot decode string %s.\n", input);
        return(FAILURE);
    } else {
        char encoded[strlen(input)+1];
        string_encode(decoded, encoded);
        printf("Successfully decoded and encoded string.\n");
        printf("Input  : %s\n", input);
        printf("Decoded: %s\n", decoded);
        printf("Encoded: %s\n", encoded);
    }
    /* return success upon completion */
    return SUCCESS;
}

int invoke_scan() {
    return scan();
}

int invoke_parse() {
    if (yyparse() != SUCCESS) {
        error("Parse failed.");
        return FAILURE;
    } else {
        info("Parse successful!");
        return SUCCESS;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        usage(argv[0]);
    }
    /* prepare yyin */
    FILE* f = fopen(argv[2], "r");
    if (!f) {
        error("File \"%s\" not found.", argv[2]);
        return FAILURE;
    }
    yyin = f;
    /* parse input flag */
    if (streq(argv[1], "--encode")) {
        return invoke_encode(f);
    } else if (streq(argv[1], "--scan")) {
        return invoke_scan();
    } else if (streq(argv[1], "--parse")) {
        return invoke_parse();
    } else {
        usage(argv[0]);
    }
    /* Clean up */
    fclose(f);
    return SUCCESS;
}