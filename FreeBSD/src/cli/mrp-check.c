#include "mrplib.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: mrp-check <file.mrp> [<file2.mrp> ...]\n");
        return 1;
    }
    int all_ok = 1;
    for (int i = 1; i < argc; i++) {
        int valid = mrp_validate(argv[i]);
        if (valid) {
            printf("OK: '%s' is a valid .mrp file\n", argv[i]);
        } else {
            printf("FAIL: '%s'", argv[i]);
            mrp_file_info info;
            if (mrp_get_info(argv[i], &info) == 0 && !info.valid_magic)
                printf(" \xe2\x80\x94 Invalid magic header \xe2\x80\x94 not an .mrp file");
            else
                printf(" \xe2\x80\x94 Invalid or corrupted");
            printf("\n");
            all_ok = 0;
        }
    }
    return all_ok ? 0 : 1;
}
