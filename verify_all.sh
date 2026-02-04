#!/bin/bash

# Quick verification script - demonstrates all features working

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=========================================="
echo "Minishell Feature Verification"
echo "=========================================="
echo ""

cd "$(dirname "$0")"

if [ ! -f ./minishell ]; then
    echo -e "${RED}Error: minishell not found. Run 'make' first.${NC}"
    exit 1
fi

echo -e "${BLUE}1. Testing Variable Expansion${NC}"
echo "Command: echo \$USER"
result=$(printf 'echo $USER\nexit\n' | ./minishell 2>&1 | grep -v "^exit" | head -1)
echo "Result: $result"
if [ ! -z "$result" ]; then
    echo -e "${GREEN}✓ Variable expansion working${NC}"
else
    echo -e "${RED}✗ Variable expansion failed${NC}"
fi
echo ""

echo -e "${BLUE}2. Testing Export + Expansion${NC}"
echo "Commands: export TEST=hello; echo \$TEST"
result=$(printf 'export TEST=hello\necho $TEST\nexit\n' | ./minishell 2>&1 | grep -v "^exit" | grep "hello")
if [ "$result" = "hello" ]; then
    echo -e "${GREEN}✓ Export and expansion working${NC}"
else
    echo -e "${RED}✗ Export failed. Got: $result${NC}"
fi
echo ""

echo -e "${BLUE}3. Testing Exit Status${NC}"
echo "Commands: true; echo \$?"
result=$(printf 'true\necho $?\nexit\n' | ./minishell 2>&1 | grep -v "^exit" | tail -1)
if [ "$result" = "0" ]; then
    echo -e "${GREEN}✓ Exit status working${NC}"
else
    echo -e "${RED}✗ Exit status failed. Got: $result${NC}"
fi
echo ""

echo -e "${BLUE}4. Testing Quote Removal${NC}"
echo 'Command: echo "hello world"'
result=$(printf 'echo "hello world"\nexit\n' | ./minishell 2>&1 | grep -v "^exit" | head -1)
if [ "$result" = "hello world" ]; then
    echo -e "${GREEN}✓ Quote removal working${NC}"
else
    echo -e "${RED}✗ Quote removal failed. Got: [$result]${NC}"
fi
echo ""

echo -e "${BLUE}5. Testing Single Quotes (No Expansion)${NC}"
echo "Command: echo '\$VAR'"
result=$(printf "echo '\$VAR'\nexit\n" | ./minishell 2>&1 | grep -v "^exit" | head -1)
if [ "$result" = "\$VAR" ]; then
    echo -e "${GREEN}✓ Single quote behavior correct${NC}"
else
    echo -e "${RED}✗ Single quotes failed. Got: $result${NC}"
fi
echo ""

echo -e "${BLUE}6. Testing Pipes${NC}"
echo "Command: echo test | cat"
result=$(printf 'echo test | cat\nexit\n' | ./minishell 2>&1 | grep -v "^exit" | head -1)
if [ "$result" = "test" ]; then
    echo -e "${GREEN}✓ Pipes working${NC}"
else
    echo -e "${RED}✗ Pipes failed. Got: $result${NC}"
fi
echo ""

echo -e "${BLUE}7. Testing Redirects${NC}"
echo "Command: echo test > /tmp/ms_test.txt; cat /tmp/ms_test.txt"
result=$(printf 'echo test > /tmp/ms_test.txt\ncat /tmp/ms_test.txt\nexit\n' | ./minishell 2>&1 | grep "test" | head -1)
rm -f /tmp/ms_test.txt
if [ "$result" = "test" ]; then
    echo -e "${GREEN}✓ Redirects working${NC}"
else
    echo -e "${RED}✗ Redirects failed${NC}"
fi
echo ""

echo -e "${BLUE}8. Testing Logical Operators${NC}"
echo "Command: true && echo yes"
result=$(printf 'true && echo yes\nexit\n' | ./minishell 2>&1 | grep "yes")
if [ "$result" = "yes" ]; then
    echo -e "${GREEN}✓ Logical operators working${NC}"
else
    echo -e "${RED}✗ Logical operators failed${NC}"
fi
echo ""

echo -e "${BLUE}9. Testing Heredoc${NC}"
echo "Command: cat << EOF"
result=$(printf 'cat << EOF\nhello\nEOF\nexit\n' | ./minishell 2>&1 | grep "hello")
if [ "$result" = "hello" ]; then
    echo -e "${GREEN}✓ Heredoc working${NC}"
else
    echo -e "${RED}✗ Heredoc failed${NC}"
fi
echo ""

echo -e "${BLUE}10. Testing Wildcards${NC}"
mkdir -p /tmp/ms_wildcard_test
touch /tmp/ms_wildcard_test/{a,b,c}.txt
cd /tmp/ms_wildcard_test
result=$(printf 'echo *.txt\nexit\n' | "$(dirname $0)/minishell" 2>&1 | grep "a.txt")
cd - > /dev/null
rm -rf /tmp/ms_wildcard_test
if [[ "$result" == *"a.txt"* ]]; then
    echo -e "${GREEN}✓ Wildcards working${NC}"
else
    echo -e "${RED}✗ Wildcards failed${NC}"
fi
echo ""

echo "=========================================="
echo -e "${GREEN}All automated tests complete!${NC}"
echo ""
echo "For interactive testing, read:"
echo "  - INTERACTIVE_TEST_GUIDE.md"
echo ""
echo "For signal testing (manual):"
echo "  1. Run: ./minishell"
echo "  2. Press Ctrl+C (should show new prompt)"
echo "  3. Press Ctrl+\\ (should be ignored)"
echo "  4. Type 'exit' to quit"
echo "=========================================="
