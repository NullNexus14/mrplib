#include "mrplib.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static int tests = 0, passed = 0;

#define TEST(name) do { tests++; printf("Test: %s... ", name); } while(0)
#define PASS do { passed++; printf("PASS\n"); } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); return 1; } while(0)

int main(void) {
    printf("=== mrplib test suite ===\n\n");

    TEST("version");
    if (strcmp(mrp_version(), "1.0.0") != 0) FAIL("wrong version");
    PASS;

    TEST("caesar");
    {
        char out;
        if (mrp_caesar_shift('A', &out) != 0 || out != 'I') FAIL("A->I");
        if (mrp_caesar_shift('Z', &out) != 0 || out != 'H') FAIL("Z->H");
        if (mrp_caesar_shift('a', &out) != 0 || out != 'I') FAIL("a->I");
        if (mrp_caesar_shift('z', &out) != 0 || out != 'H') FAIL("z->H");
    }
    PASS;

    TEST("caesar unshift");
    {
        char out;
        if (mrp_caesar_unshift('I', &out) != 0 || out != 'A') FAIL("I->A");
        if (mrp_caesar_unshift('H', &out) != 0 || out != 'Z') FAIL("H->Z");
    }
    PASS;

    TEST("morse");
    {
        if (strcmp(mrp_char_to_morse('A'), ".-") != 0) FAIL("A");
        if (strcmp(mrp_char_to_morse('Z'), "--..") != 0) FAIL("Z");
        if (strcmp(mrp_char_to_morse('0'), "-----") != 0) FAIL("0");
        if (strcmp(mrp_char_to_morse('9'), "----.") != 0) FAIL("9");
        if (strcmp(mrp_char_to_morse(' '), ".-.-.") != 0) FAIL("space");
        if (mrp_char_to_morse('[') != NULL) FAIL("invalid");
    }
    PASS;

    TEST("bits");
    {
        unsigned char bits;
        int len = mrp_morse_to_bits(".-", &bits);
        if (len != 2) FAIL("len");
        if (bits != 0x01) FAIL("bits");
    }
    PASS;

    TEST("invert");
    {
        unsigned char raw = 0x01;
        unsigned char inv = mrp_invert_bits(raw);
        unsigned char masked = inv & 0x03;
        if (masked != 0x02) FAIL("invert 01 -> 10");
    }
    PASS;

    TEST("pack");
    {
        unsigned char byte;
        if (mrp_pack_bits(0x02, 2, &byte) != 0) FAIL("pack");
        if (byte != 0x42) FAIL("pack value");
        unsigned char bits_out, len_out;
        if (mrp_unpack_byte(byte, &bits_out, &len_out) != 0) FAIL("unpack");
        if (len_out != 2 || bits_out != 0x02) FAIL("unpack value");
    }
    PASS;

    TEST("encode/decode");
    {
        for (char c = 'A'; c <= 'Z'; c++) {
            unsigned char byte;
            if (mrp_encode_char(c, &byte) != 0) FAIL("encode");
            char c2; int is_end;
            if (mrp_decode_byte(byte, &c2, &is_end) != 0) FAIL("decode");
            if (is_end) FAIL("unexpected end");
            if (c2 != c) FAIL("round-trip mismatch");
        }
        for (char c = '0'; c <= '9'; c++) {
            unsigned char byte;
            if (mrp_encode_char(c, &byte) != 0) FAIL("encode digit");
            char c2; int is_end;
            if (mrp_decode_byte(byte, &c2, &is_end) != 0) FAIL("decode digit");
            if (is_end) FAIL("unexpected end digit");
            if (c2 != c) FAIL("round-trip digit mismatch");
        }
        {
            unsigned char byte;
            if (mrp_encode_char(' ', &byte) != 0) FAIL("encode space");
            char c2; int is_end;
            if (mrp_decode_byte(byte, &c2, &is_end) != 0) FAIL("decode space");
            if (is_end) FAIL("unexpected end space");
            if (c2 != ' ') FAIL("round-trip space mismatch");
        }
    }
    PASS;

    TEST("spec");
    {
        const char *json = mrp_spec_json();
        if (json == NULL) FAIL("null spec");
        if (strstr(json, "\"format\":\"mrp\"") == NULL) FAIL("missing format");
    }
    PASS;

    TEST("file I/O");
    {
        const char *tmp = "/tmp/test_mrp_basic.mrp";
        const char *text = "HELLO WORLD 123";
        if (mrp_create(tmp, text) != 0) FAIL("create");
        mrp_file f;
        if (mrp_open(tmp, &f) != 0) FAIL("open");
        if (memcmp(f.magic, "MRP1", 4) != 0) FAIL("magic");
        if (f.payload_size != strlen(text)) FAIL("payload size");
        mrp_close(&f);
        if (!mrp_validate(tmp)) FAIL("validate");
    }
    PASS;

    printf("\n=== All tests passed! ===\n");
    return 0;
}
