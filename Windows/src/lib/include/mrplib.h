#ifndef MRPLIB_H
#define MRPLIB_H

#include <stddef.h>
#include <stdint.h>

#define MRPLIB_VERSION_MAJOR 1
#define MRPLIB_VERSION_MINOR 0
#define MRPLIB_VERSION_PATCH 0
#define MRPLIB_MAGIC_SIZE 4
#define MRPLIB_MAGIC_BYTES {'M','R','P','1'}
#define MRPLIB_HEADER_SIZE 5
#define MRPLIB_PAYLOAD_OFFSET 5

typedef struct {
    char magic[4];
    unsigned char payload_size;
    unsigned char *data;
} mrp_file;

typedef struct {
    char original;
    char shifted;
    const char *morse;
    unsigned char bits;
    unsigned char packed_byte;
} mrp_char_info;

typedef struct {
    int valid_magic;
    size_t file_size;
    unsigned char payload_size;
    size_t char_count;
    const char *format_version;
} mrp_file_info;

const char *mrp_version(void);
int mrp_caesar_shift(char c, char *out);
int mrp_caesar_unshift(char c, char *out);
const char *mrp_char_to_morse(char c);
int mrp_morse_to_char(const char *morse, char *out);
int mrp_morse_to_bits(const char *morse, unsigned char *bits_out);
int mrp_bits_to_morse(unsigned char bits, int length, char *morse_out, size_t morse_size);
unsigned char mrp_invert_bits(unsigned char bits);
int mrp_pack_bits(unsigned char bits, unsigned char length, unsigned char *byte_out);
int mrp_unpack_byte(unsigned char byte, unsigned char *bits_out, unsigned char *length_out);
int mrp_encode_char(char c, unsigned char *byte_out);
int mrp_decode_byte(unsigned char byte, char *c_out, int *is_end);
int mrp_create(const char *path, const char *text);
int mrp_open(const char *path, mrp_file *file);
void mrp_close(mrp_file *file);
int mrp_get_info(const char *path, mrp_file_info *info);
int mrp_validate(const char *path);
int mrp_register_filetype(void);
int mrp_unregister_filetype(void);
const char *mrp_spec_json(void);
int mrp_spec_json_copy(char *buf, size_t bufsize);

#endif
