#include "utils.h"
#include "encoder.h"

void usage(char* prog_name, int status) {
    printf("Usage: %s --encode <input.file>\n", prog_name);
    exit(status);
}

void invoke_encode(char* file) {
    /* decode and encode string */
    /* read input string */
    char input[BUFSIZ];
    FILE* f = fopen(file, "r");
    if (!f) {
        printf("File \"%s\" not found.\n", file);
        exit(EXIT_FAILURE);
    }
    if(!fgets(input, BUFSIZ, f)) {
        /* failed to read file */
        printf("Error: failed to read input files.\n");
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    } else {
        char encoded[strlen(input)+1];
        string_encode(decoded, encoded);
        printf("Successfully decoded and encoded string.\n");
        printf("Input  : %s\n", input);
        printf("Decoded: %s\n", decoded);
        printf("Encoded: %s\n", encoded);
    }
}

int main(int argc, char* argv[]) {
    /* for the Encoder assignment. */
    if (argc != 3) {
        usage(argv[0], EXIT_FAILURE);
    }
    /* parse input flag */
    if (strcmp(argv[1], "--encode") == 0) {
        invoke_encode(argv[2]);
    } else {
        usage(argv[0], EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}