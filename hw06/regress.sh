#!/bin/sh
# regress.sh: HW06 regression test. Runs ./replay on every scenario in
# scenarios/ and compares its trace with expected/<name>.trace.
# Usage, in hw06/ after make:   sh regress.sh
# Exits with 1 if any trace differs, so that make test fails.
fail=0
for scenario in scenarios/*.txt; do
    name=$(basename "$scenario" .txt)
    if ./replay "$scenario" | diff - "expected/$name.trace" > /dev/null; then
        echo "PASS  regression $name"
    else
        echo "FAIL  regression $name"
        fail=1
    fi
done
exit $fail