#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

extern const unsigned char *embed_offsets[];
extern unsigned int embed_lengths[];

#include "mrp-installer.h"

static void ensure_dir(const char *path) {
    char tmp[1024];
    strncpy(tmp, path, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    mkdir(tmp, 0755);
}

static int write_file(const char *dst, const unsigned char *data, unsigned int len) {
    FILE *f = fopen(dst, "wb");
    if (!f) return -1;
    size_t wrote = fwrite(data, 1, len, f);
    fclose(f);
    return (wrote == len) ? 0 : -1;
}

static int install_file(const char *prefix, int idx) {
    char dst[1024];
    const char *subpath = embed_filenames[idx];
    snprintf(dst, sizeof(dst), "%s/share/mrplib/%s", prefix, subpath);
    ensure_dir(dst);
    if (write_file(dst, embed_offsets[idx], embed_lengths[idx]) != 0) {
        fprintf(stderr, "  FAIL: %s\n", subpath);
        return -1;
    }
    printf("  %s\n", subpath);
    return 0;
}

int main(int argc, char **argv) {
    const char *prefix = "/usr/local";
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--prefix=", 9) == 0) {
            prefix = argv[i] + 9;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("============================================\n");
            printf("  mrplib \xe2\x80\x94 MRP Filetype System Installer\n");
            printf("  Version 1.0.0\n");
            printf("============================================\n");
            printf("Usage: mrp-installer [--prefix=<path>]\n\n");
            printf("Install mrplib (library, CLI tools, MIME types,\n");
            printf("icons, man pages, and filetype registration).\n\n");
            printf("Options:\n");
            printf("  --prefix=<path>  Install to custom prefix\n");
            printf("                   Default: /usr/local\n");
            printf("  -h, --help       Show this help\n");
            return 0;
        }
    }
    init_embedded_files();
    printf("Installing mrplib to %s/share/mrplib/ ...\n", prefix);
    int ok = 0;
    int count = sizeof(embed_filenames) / sizeof(embed_filenames[0]);
    for (int i = 0; i < count; i++)
        if (install_file(prefix, i) == 0) ok++;
    printf("Installed %d/%d files.\n", ok, count);
    return (ok == count) ? 0 : 1;
}
