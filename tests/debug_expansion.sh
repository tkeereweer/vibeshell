#!/bin/bash

# Debug script to test variable expansion

MINISHELL="../minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=================================="
echo "Variable Expansion Debug Tests"
echo "=================================="
echo ""

test_expansion() {
    local name="$1"
    local cmd="$2"
    local expected="$3"

    echo -e "${YELLOW}Test: $name${NC}"
    echo "Command: $cmd"

    result=$(printf "%s\nexit\n" "$cmd" | $MINISHELL 2>&1 | grep -v "^minishell>" | grep -v "^exit$" | grep -v "^$")

    echo "Expected: [$expected]"
    echo "Got: [$result]"

    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
    else
        echo -e "${RED}✗ FAIL${NC}"
    fi
    echo ""
}

# Test 1: USER variable (inherited from environment)
test_expansion "USER variable" "echo \$USER" "$USER"

# Test 2: Exported variable
test_expansion "Exported variable" "export TEST=hello\necho \$TEST" "hello"

# Test 3: Exit status
test_expansion "Exit status after true" "true\necho \$?" "0"

# Test 4: Exit status after false
test_expansion "Exit status after false" "false\necho \$?" "1"

# Test 5: Undefined variable
test_expansion "Undefined variable" "echo \$UNDEFINED" ""

# Test 6: Variable in double quotes
test_expansion "Variable in double quotes" "export VAR=world\necho \"Hello \$VAR\"" "Hello world"

# Test 7: Variable in single quotes (should NOT expand)
test_expansion "Variable in single quotes" "echo '\$VAR'" "\$VAR"

# Test 8: Multiple variables
test_expansion "Multiple variables" "export A=hello B=world\necho \$A \$B" "hello world"

# Test 9: HOME variable
test_expansion "HOME variable" "echo \$HOME" "$HOME"

# Test 10: PATH variable
test_expansion "PATH exists" "echo \$PATH | grep -q / && echo found" "found"

echo "=================================="
echo "Debug complete!"
echo "=================================="
