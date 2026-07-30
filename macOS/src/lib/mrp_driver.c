#include "mrplib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int mrp_create(const char *path, const char *text) {
    if (!path || !text) return -1;
    unsigned char magic[] = MRPLIB_MAGIC_BYTES;
    size_t text_len = strlen(text);
    if (text_len > 255) return -1;
    unsigned char buf[5 + 255];
    memcpy(buf, magic, 4);
    buf[4] = (unsigned char)text_len;
    for (size_t i = 0; i < text_len; i++) {
        if (mrp_encode_char(text[i], &buf[5 + i]) != 0)
            return -1;
    }
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    size_t written = fwrite(buf, 1, 5 + text_len, f);
    fclose(f);
    return (written == 5 + text_len) ? 0 : -1;
}

int mrp_open(const char *path, mrp_file *file) {
    if (!path || !file) return -1;
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    unsigned char header[MRPLIB_HEADER_SIZE];
    if (fread(header, 1, MRPLIB_HEADER_SIZE, f) != MRPLIB_HEADER_SIZE) {
        fclose(f);
        return -1;
    }
    unsigned char expected_magic[] = MRPLIB_MAGIC_BYTES;
    if (memcmp(header, expected_magic, MRPLIB_MAGIC_SIZE) != 0) {
        fclose(f);
        return -1;
    }
    memcpy(file->magic, header, 4);
    file->payload_size = header[4];
    file->data = NULL;
    if (file->payload_size > 0) {
        file->data = (unsigned char *)malloc(file->payload_size);
        if (!file->data) { fclose(f); return -1; }
        if (fread(file->data, 1, file->payload_size, f) != file->payload_size) {
            fclose(f);
            free(file->data);
            file->data = NULL;
            return -1;
        }
    }
    fclose(f);
    return 0;
}

void mrp_close(mrp_file *file) {
    if (file) {
        free(file->data);
        file->data = NULL;
    }
}

int mrp_get_info(const char *path, mrp_file_info *info) {
    if (!path || !info) return -1;
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    info->file_size = (size_t)ftell(f);
    rewind(f);
    unsigned char header[MRPLIB_HEADER_SIZE];
    if (fread(header, 1, MRPLIB_HEADER_SIZE, f) != MRPLIB_HEADER_SIZE) {
        fclose(f);
        return -1;
    }
    fclose(f);
    unsigned char expected_magic[] = MRPLIB_MAGIC_BYTES;
    info->valid_magic = (memcmp(header, expected_magic, MRPLIB_MAGIC_SIZE) == 0);
    info->payload_size = header[4];
    info->char_count = info->payload_size;
    info->format_version = mrp_version();
    return 0;
}

int mrp_validate(const char *path) {
    mrp_file_info info;
    if (mrp_get_info(path, &info) != 0) return 0;
    if (!info.valid_magic) return 0;
    if (info.file_size < (size_t)(5 + info.payload_size)) return 0;
    mrp_file file;
    if (mrp_open(path, &file) != 0) return 0;
    int ret = 1;
    char c;
    int is_end;
    for (int i = 0; i < file.payload_size; i++) {
        if (mrp_decode_byte(file.data[i], &c, &is_end) != 0) {
            ret = 0; break;
        }
        if (is_end && i != file.payload_size - 1) {
            ret = 0; break;
        }
    }
    mrp_close(&file);
    return ret;
}

int mrp_register_filetype(void) {
    return 0;
}

int mrp_unregister_filetype(void) {
    return 0;
}
