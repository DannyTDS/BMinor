#include "utils.h"
#include "encoder.h"
#include "helper.h"

void usage(char* prog_name, int status) {
    printf("Usage: %s [options] <input.file>\n", prog_name);
    printf("Options:\n");
    printf("\t--encode\n");
    printf("\t--scan\n");
    exit(status);
}

int invoke_encode(char* file) {
    /* decode and encode string */
    /* read input string */
    char input[BUFSIZ];
    FILE* f = fopen(file, "r");
    if (!f) {
        printf("File \"%s\" not found.\n", file);
        return(FAILURE);
    }
    if(!fgets(input, BUFSIZ, f)) {
        /* failed to read file */
        printf("Error: failed to read input files.\n");
        return(FAILURE);
    };
    /* remove trailing newline if one presents */
    if (input[strlen(input)-1] == '\n') {
        input[strlen(input)-1] = '\0';
    }
    fclose(f);
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

int invoke_scan(char* file) {
    FILE* f = fopen(file, "r");
    if (!f) {
        error("File \"%s\" not found.", file);
        return(FAILURE);
    }
    int status = scan(f);
    fclose(f);
    return status;
}

int invoke_parse() {
    int status = SUCCESS;
    return status;
}

int main(int argc, char* argv[]) {
    /* for the Encoder assignment. */
    if (argc != 3) {
        usage(argv[0], EXIT_FAILURE);
    }
    /* parse input flag */
    if (streq(argv[1], "--encode")) {
        return invoke_encode(argv[2]);
    } else if (streq(argv[1], "--scan")) {
        return invoke_scan(argv[2]);
    } else {
        usage(argv[0], EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}