#!/bin/bash

# Quick sanity test - runs a few basic tests to verify minishell is working

MINISHELL="../minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=================================="
echo "Quick Minishell Sanity Test"
echo "=================================="
echo ""

if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: minishell not found. Run 'make' first.${NC}"
    exit 1
fi

# Test 1: Basic echo
echo -n "Test 1: Basic echo... "
result=$(echo "echo hello" | $MINISHELL 2>/dev/null | grep hello)
if [ "$result" = "hello" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 2: Variable expansion
echo -n "Test 2: Variable expansion... "
result=$(echo "export VAR=test" | $MINISHELL 2>/dev/null)
result=$(echo "export VAR=test; echo \$VAR" | $MINISHELL 2>/dev/null | grep test)
if [ "$result" = "test" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 3: Exit status
echo -n "Test 3: Exit status... "
result=$(echo "true; echo \$?" | $MINISHELL 2>/dev/null | tail -1)
if [ "$result" = "0" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 4: Pipe
echo -n "Test 4: Pipes... "
result=$(echo "echo hello | cat" | $MINISHELL 2>/dev/null | grep hello)
if [ "$result" = "hello" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 5: Redirect
echo -n "Test 5: Redirects... "
echo "echo test > /tmp/ms_test; cat /tmp/ms_test" | $MINISHELL > /dev/null 2>&1
if [ -f /tmp/ms_test ] && [ "$(cat /tmp/ms_test)" = "test" ]; then
    echo -e "${GREEN}✓${NC}"
    rm -f /tmp/ms_test
else
    echo -e "${RED}✗${NC}"
fi

# Test 6: AND operator
echo -n "Test 6: AND operator... "
result=$(echo "true && echo success" | $MINISHELL 2>/dev/null | grep success)
if [ "$result" = "success" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 7: OR operator
echo -n "Test 7: OR operator... "
result=$(echo "false || echo success" | $MINISHELL 2>/dev/null | grep success)
if [ "$result" = "success" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 8: Quotes
echo -n "Test 8: Double quotes... "
result=$(echo 'echo "hello world"' | $MINISHELL 2>/dev/null)
if [ "$result" = "hello world" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 9: Single quotes (no expansion)
echo -n "Test 9: Single quotes... "
result=$(echo "echo '\$VAR'" | $MINISHELL 2>/dev/null)
if [ "$result" = "\$VAR" ]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

# Test 10: Wildcard
echo -n "Test 10: Wildcards... "
mkdir -p /tmp/ms_wildcard_test
touch /tmp/ms_wildcard_test/a.txt /tmp/ms_wildcard_test/b.txt
cd /tmp/ms_wildcard_test
result=$(echo "echo *.txt" | $MINISHELL 2>/dev/null)
cd - > /dev/null
rm -rf /tmp/ms_wildcard_test
if [[ "$result" == *"a.txt"* ]] && [[ "$result" == *"b.txt"* ]]; then
    echo -e "${GREEN}✓${NC}"
else
    echo -e "${RED}✗${NC}"
fi

echo ""
echo "=================================="
echo "Quick test complete!"
echo "Run ./tests/test_runner.sh for comprehensive tests"
echo "=================================="
