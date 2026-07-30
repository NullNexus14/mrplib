#include "mrplib.h"
#include <string.h>
#include <stdio.h>

static const char *morse_letters[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
    "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
    "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
    "-.--", "--.."
};

static const char *morse_digits[] = {
    "-----", ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----."
};

static const char *morse_space = ".-.-.";
static const char *morse_end = "---.-";

const char *mrp_version(void) {
    return "1.0.0";
}

int mrp_caesar_shift(char c, char *out) {
    if (c >= 'A' && c <= 'Z') {
        *out = (char)('A' + (c - 'A' + 8) % 26);
        return 0;
    }
    if (c >= 'a' && c <= 'z') {
        *out = (char)('A' + (c - 'a' + 8) % 26);
        return 0;
    }
    if ((c >= '0' && c <= '9') || c == ' ') {
        *out = c;
        return 0;
    }
    return -1;
}

int mrp_caesar_unshift(char c, char *out) {
    if (c >= 'A' && c <= 'Z') {
        *out = (char)('A' + (c - 'A' + 18) % 26);
        return 0;
    }
    if ((c >= '0' && c <= '9') || c == ' ') {
        *out = c;
        return 0;
    }
    return -1;
}

const char *mrp_char_to_morse(char c) {
    if (c >= 'A' && c <= 'Z')
        return morse_letters[c - 'A'];
    if (c >= '0' && c <= '9')
        return morse_digits[c - '0'];
    if (c == ' ')
        return morse_space;
    return NULL;
}

int mrp_morse_to_char(const char *morse, char *out) {
    if (!morse || !out) return -1;
    for (int i = 0; i < 26; i++) {
        if (strcmp(morse, morse_letters[i]) == 0) {
            *out = (char)('A' + i);
            return 0;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (strcmp(morse, morse_digits[i]) == 0) {
            *out = (char)('0' + i);
            return 0;
        }
    }
    if (strcmp(morse, morse_space) == 0) {
        *out = ' ';
        return 0;
    }
    return -1;
}

int mrp_morse_to_bits(const char *morse, unsigned char *bits_out) {
    if (!morse || !bits_out) return -1;
    unsigned char bits = 0;
    size_t len = strlen(morse);
    if (len > 8) return -1;
    for (size_t i = 0; i < len; i++) {
        if (morse[i] == '.')
            bits = (bits << 1) | 0;
        else if (morse[i] == '-')
            bits = (bits << 1) | 1;
        else
            return -1;
    }
    *bits_out = bits;
    return (int)len;
}

int mrp_bits_to_morse(unsigned char bits, int length, char *morse_out, size_t morse_size) {
    if (!morse_out || morse_size < 9 || length < 1 || length > 8) return -1;
    for (int i = 0; i < length; i++) {
        int bitpos = length - 1 - i;
        morse_out[i] = (bits & (1 << bitpos)) ? '-' : '.';
    }
    morse_out[length] = '\0';
    return length;
}

unsigned char mrp_invert_bits(unsigned char bits) {
    return ~bits;
}

int mrp_pack_bits(unsigned char bits, unsigned char length, unsigned char *byte_out) {
    if (length > 5 || !byte_out) return -1;
    *byte_out = (length << 5) | (bits & 0x1F);
    return 0;
}

int mrp_unpack_byte(unsigned char byte, unsigned char *bits_out, unsigned char *length_out) {
    if (!bits_out || !length_out) return -1;
    *length_out = byte >> 5;
    *bits_out = byte & 0x1F;
    return 0;
}

int mrp_encode_char(char c, unsigned char *byte_out) {
    if (!byte_out) return -1;
    char shifted;
    if (mrp_caesar_shift(c, &shifted) != 0)
        return -1;
    const char *morse = mrp_char_to_morse(shifted);
    if (!morse) return -1;
    unsigned char bits;
    int len = mrp_morse_to_bits(morse, &bits);
    if (len < 0) return -1;
    bits = mrp_invert_bits(bits);
    bits = bits & ((1 << len) - 1);
    return mrp_pack_bits(bits, (unsigned char)len, byte_out);
}

int mrp_decode_byte(unsigned char byte, char *c_out, int *is_end) {
    if (!c_out || !is_end) return -1;
    unsigned char raw_bits, length;
    if (mrp_unpack_byte(byte, &raw_bits, &length) != 0)
        return -1;
    unsigned char bits = mrp_invert_bits(raw_bits);
    bits = bits & ((1 << length) - 1);
    char morse[9];
    if (mrp_bits_to_morse(bits, length, morse, sizeof(morse)) < 0)
        return -1;
    if (strcmp(morse, morse_end) == 0) {
        *is_end = 1;
        *c_out = '\0';
        return 0;
    }
    *is_end = 0;
    char shifted;
    if (mrp_morse_to_char(morse, &shifted) != 0)
        return -1;
    return mrp_caesar_unshift(shifted, c_out);
}
