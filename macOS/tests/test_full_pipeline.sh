#!/usr/bin/env bash
set -euo pipefail

PASS=0; FAIL=0
pass() { echo "PASS: $1"; PASS=$((PASS + 1)); }
fail() { echo "FAIL: $1"; FAIL=$((FAIL + 1)); }

SRCDIR="$(cd "$(dirname "$0")" && pwd)"
PROJDIR="$(cd "$SRCDIR/.." && pwd)"

cleanup() {
    rm -f /tmp/test_pipeline.mrp /tmp/test_mrp_bad.mrp /tmp/test_basic /tmp/mrp_create_test
}
trap cleanup EXIT

echo "=== mrplib Full Pipeline Test ==="
echo ""

# Step 1: Build and run unit tests
gcc -Wall -std=c11 -I"$PROJDIR/src/lib/include" -o /tmp/test_basic "$SRCDIR/test_basic.c" \
    -L"$PROJDIR" -lmrplib -Wl,-rpath,"$PROJDIR" 2>&1 && \
    (LD_LIBRARY_PATH="$PROJDIR" /tmp/test_basic > /dev/null 2>&1 && \
    pass "unit tests") || fail "unit tests"

# Step 2: Create a .mrp file
echo "Testing mrp_create..."
cat > /tmp/mrp_create_test.c << 'CEOF'
#include "mrplib.h"
#include <stdio.h>
#include <string.h>
int main(void) {
    const char *text = "HELLO WORLD 123";
    if (mrp_create("/tmp/test_pipeline.mrp", text) != 0) {
        fprintf(stderr, "mrp_create failed\n"); return 1;
    }
    return 0;
}
CEOF
gcc -std=c11 -I"$PROJDIR/src/lib/include" -o /tmp/mrp_create_test /tmp/mrp_create_test.c \
    -L"$PROJDIR" -lmrplib -Wl,-rpath,"$PROJDIR" && \
    LD_LIBRARY_PATH="$PROJDIR" /tmp/mrp_create_test && \
    pass "mrp_create writes .mrp file" || fail "mrp_create"

# Step 3: Validate with mrp-check
echo "Testing mrp-check..."
"$PROJDIR/mrp-check" /tmp/test_pipeline.mrp 2>&1 && \
    pass "mrp-check: valid file" || fail "mrp-check: valid file"

# Step 4: Get info with mrp-info
echo "Testing mrp-info..."
"$PROJDIR/mrp-info" /tmp/test_pipeline.mrp > /dev/null 2>&1 && \
    pass "mrp-info: reads file" || fail "mrp-info: reads file"
JSON_OUT=$("$PROJDIR/mrp-info" --json /tmp/test_pipeline.mrp 2>&1)
echo "$JSON_OUT" | python3 -m json.tool > /dev/null 2>&1 && \
    pass "mrp-info --json: valid JSON" || fail "mrp-info --json: invalid JSON"

# Step 5: Reject invalid file
echo "Testing invalid file rejection..."
echo "bad data" > /tmp/test_mrp_bad.mrp
! "$PROJDIR/mrp-check" /tmp/test_mrp_bad.mrp 2>&1 && \
    pass "mrp-check: rejects invalid" || fail "mrp-check: rejects invalid"

# Step 6: Validate mrp-spec.json
echo "Testing mrp-spec.json..."
python3 -m json.tool "$PROJDIR/files/rules/mrp-spec.json" > /dev/null 2>&1 && \
    pass "mrp-spec.json is valid JSON" || fail "mrp-spec.json is not valid JSON"

echo ""
echo "=== Results: $PASS passed, $FAIL failed ==="
exit $(( FAIL > 0 ? 1 : 0 ))
