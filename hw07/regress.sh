#!/bin/sh
# Regression tests for ./balance
set -u
cd "$(dirname "$0")" || exit 1

fail=0
pass=0

for in_file in inputs/*; do
    name=$(basename "$in_file")
    exp_out="expected/${name}.out"
    exp_rc="expected/${name}.rc"

    actual_out=$(./balance < "$in_file")
    actual_rc=$?

    exp_text=$(cat "$exp_out" 2>/dev/null)
    exp_status=$(cat "$exp_rc" 2>/dev/null)

    if [ "$actual_out" = "$exp_text" ] && [ "$actual_rc" -eq "$exp_status" ]; then
        echo "PASS: $name"
        pass=$((pass + 1))
    else
        echo "FAIL: $name"
        echo "  expected rc=$exp_status output: $exp_text"
        echo "  actual   rc=$actual_rc output: $actual_out"
        fail=$((fail + 1))
    fi
done

echo "$pass passed, $fail failed"
[ "$fail" -eq 0 ]