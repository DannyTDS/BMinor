#include "utils.h"
#include "encoder.h"
#include "helper.h"

void usage(char* prog_name) {
    fprintf(stderr, "Usage: %s [options] <input.file>\n", prog_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t--encode\n");
    fprintf(stderr, "\t--scan\n");
    fprintf(stderr, "\t--parse\n");
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
    int status;
    /* parse input flag */
    if (streq(argv[1], "--encode")) {
        status =  encode(f);
    } else if (streq(argv[1], "--scan")) {
        status = scan("verbose");
    } else if (streq(argv[1], "--parse")) {
        status = parse();
    } else {
        usage(argv[0]);
        status = FAILURE;
    }
    /* Clean up */
    fclose(f);
    return status;
}