#include "utils.h"
#include "encoder.h"
#include "helper.h"

void usage(char* prog_name) {
    fprintf(stderr, "Usage: %s [options] <input.file> [<output.file>]\n", prog_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t--encode\n");
    fprintf(stderr, "\t--scan\n");
    fprintf(stderr, "\t--parse\n");
    fprintf(stderr, "\t--print\n");
    fprintf(stderr, "\t--resolve\n");
    fprintf(stderr, "\t--typecheck\n");
    fprintf(stderr, "\t--codegen\tMUST supply an output file\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        usage(argv[0]);
        return FAILURE;
    }
    /* prepare yyin */
    FILE* source = fopen(argv[2], "r");
    if (!source) {
        error("File \"%s\" not found.", argv[2]);
        return FAILURE;
    }
    yyin = source;
    /* prepare output file */
    if (argc > 3) {
        output = fopen(argv[3], "w");
    }
    /* parse input flag */
    int status;
    if (streq(argv[1], "--encode")) {
        status =  encode(source);
    } else if (streq(argv[1], "--scan")) {
        status = scan("verbose");
    } else if (streq(argv[1], "--parse")) {
        status = parse("verbose");
    } else if (streq(argv[1], "--print")) {
        status = pprint();
    } else if (streq(argv[1], "--resolve")) {
        status = resolve("verbose");
    } else if (streq(argv[1], "--typecheck")) {
        status = typecheck("verbose");
    } else if (streq(argv[1], "--codegen")) {
        if (!output) {
            error("For codegen, an output file must be specified");
            return FAILURE;
        }
        status = codegen();
    } else {
        usage(argv[0]);
        status = FAILURE;
    }
    /* Clean up */
    fclose(source);
    return status;
}