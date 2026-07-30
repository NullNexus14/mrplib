#include "mrplib.h"
#include <string.h>

const char *mrp_spec_json(void) {
    return "{"
        "\"format\":\"mrp\","
        "\"version\":\"1.0.0\","
        "\"pipeline\":["
        "{\"step\":1,\"name\":\"caesar_shift\",\"description\":\"Caesar cipher +8\","
        "\"params\":{\"shift\":8,\"alphabet\":\"A-Z\"}},"
        "{\"step\":2,\"name\":\"morse_encode\",\"description\":\"Custom morse encoding\"},"
        "{\"step\":3,\"name\":\"bit_convert\",\"description\":\"Dot=0 Dash=1\"},"
        "{\"step\":4,\"name\":\"bit_invert\",\"description\":\"Invert all bits\"},"
        "{\"step\":5,\"name\":\"pack\",\"description\":\"3-bit length prefix + 5-bit data, 1 byte per char\"}"
        "],"
        "\"morse\":{"
        "\"A\":\".-\",\"B\":\"-...\",\"C\":\"-.-.\",\"D\":\"-..\",\"E\":\".\","
        "\"F\":\"..-.\",\"G\":\"--.\",\"H\":\"....\",\"I\":\"..\",\"J\":\".---\","
        "\"K\":\"-.-\",\"L\":\".-..\",\"M\":\"--\",\"N\":\"-.\",\"O\":\"---\","
        "\"P\":\".--.\",\"Q\":\"--.-\",\"R\":\".-.\",\"S\":\"...\",\"T\":\"-\","
        "\"U\":\"..-\",\"V\":\"...-\",\"W\":\".--\",\"X\":\"-..-\",\"Y\":\"-.--\",\"Z\":\"--..\","
        "\"0\":\"-----\",\"1\":\".----\",\"2\":\"..---\",\"3\":\"...--\",\"4\":\"....-\","
        "\"5\":\".....\",\"6\":\"-....\",\"7\":\"--...\",\"8\":\"---..\",\"9\":\"----.\","
        "\" \":\".-.-.\",\"<END>\":\"---.-\""
        "},"
        "\"magic\":{\"bytes\":[77,82,80,49],\"ascii\":\"MRP1\"},"
        "\"header\":{\"size\":5,\"magic_size\":4,\"payload_size_offset\":4},"
        "\"encoding\":{"
        "\"dot_bit\":0,\"dash_bit\":1,"
        "\"pack_bits\":5,\"length_bits\":3,"
        "\"max_morse_len\":5"
        "}"
        "}";
}

int mrp_spec_json_copy(char *buf, size_t bufsize) {
    if (!buf || bufsize == 0) return -1;
    const char *json = mrp_spec_json();
    size_t len = strlen(json) + 1;
    if (len > bufsize) {
        if (bufsize > 0) buf[0] = '\0';
        return -1;
    }
    memcpy(buf, json, len);
    return 0;
}
