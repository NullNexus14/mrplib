#include "mrplib.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int json_mode = 0;
    const char *path = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--json") == 0)
            json_mode = 1;
        else if (argv[i][0] != '-')
            path = argv[i];
    }
    if (!path) {
        fprintf(stderr, "Usage: mrp-info [--json] <file.mrp>\n");
        return 1;
    }
    mrp_file_info info;
    if (mrp_get_info(path, &info) != 0) {
        fprintf(stderr, "Error: could not read '%s'\n", path);
        return 1;
    }
    if (json_mode) {
        printf("{\n");
        printf("  \"path\": \"%s\",\n", path);
        printf("  \"file_size\": %zu,\n", info.file_size);
        printf("  \"valid_magic\": %s,\n", info.valid_magic ? "true" : "false");
        printf("  \"payload_size\": %u,\n", info.payload_size);
        printf("  \"char_count\": %zu,\n", info.char_count);
        printf("  \"format_version\": %d\n", 1);
        printf("}\n");
    } else {
        printf("File:        %s\n", path);
        printf("Size:        %zu bytes\n", info.file_size);
        printf("Valid magic: %s\n", info.valid_magic ? "yes" : "no");
        printf("Payload:     %u bytes\n", info.payload_size);
        printf("Chars:       %zu\n", info.char_count);
        printf("Format v%s\n", mrp_version());
    }
    return 0;
}
