#!/bin/bash
# Comprehensive integration test for minishell

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

PASS=0
FAIL=0

test_cmd() {
    local desc="$1"
    local cmd="$2"
    local expected="$3"

    result=$(echo "$cmd" | ./minishell 2>&1 | grep -v "^exit$" | head -1)

    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✓${NC} $desc"
        ((PASS++))
    else
        echo -e "${RED}✗${NC} $desc"
        echo "  Expected: $expected"
        echo "  Got: $result"
        ((FAIL++))
    fi
}

echo "=== Minishell Integration Tests ==="
echo

echo "--- Built-in Commands ---"
test_cmd "echo" "echo hello" "hello"
test_cmd "echo -n" "echo -n test" "test"
test_cmd "pwd" "pwd" "$(pwd)"

echo
echo "--- Environment Variables ---"
test_cmd "export" "export TEST=value && env | grep TEST" "TEST=value"

echo
echo "--- External Commands ---"
test_cmd "/bin/echo" "/bin/echo test" "test"

echo
echo "--- Pipes ---"
test_cmd "pipe 2 commands" "echo test | cat" "test"
test_cmd "pipe 3 commands" "echo abc | cat | cat" "abc"

echo
echo "--- Logical Operators ---"
test_cmd "AND success" "echo first && echo second" "first"
test_cmd "OR failure" "false || echo fallback" "fallback"

echo
echo "--- Redirections ---"
# Test output redirection
echo "test" | ./minishell -c "echo hello > /tmp/minishell_test.txt" 2>/dev/null
if [ "$(cat /tmp/minishell_test.txt 2>/dev/null)" = "hello" ]; then
    echo -e "${GREEN}✓${NC} output redirection"
    ((PASS++))
else
    echo -e "${RED}✗${NC} output redirection"
    ((FAIL++))
fi
rm -f /tmp/minishell_test.txt

echo
echo "=== Summary ==="
echo "Passed: $PASS"
echo "Failed: $FAIL"

if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed${NC}"
    exit 1
fi
