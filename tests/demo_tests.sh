#!/bin/bash

# Demo test - shows key features working

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

MINISHELL="../minishell"

echo "=================================="
echo "Minishell Feature Demonstration"
echo "=================================="
echo ""

demo_test() {
    local name="$1"
    local cmd="$2"
    local expected="$3"

    echo -e "${BLUE}Test:${NC} $name"
    echo -e "${YELLOW}Command:${NC} $(echo "$cmd" | tr '\n' ';')"

    # Run command and capture output (filter out prompts)
    result=$(printf "%s\nexit\n" "$cmd" | $MINISHELL 2>&1 | grep -v "^minishell>" | grep -v "^exit$" | grep -v "^$" | tail -1)

    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✓ PASS${NC} - Output: $result"
    else
        echo -e "${RED}✗ FAIL${NC} - Expected: $expected, Got: $result"
    fi
    echo ""
}

echo "=== Basic Commands ==="
demo_test "Echo" "echo hello world" "hello world"
demo_test "PWD" "pwd" "$(cd ../; pwd)"

echo "=== Variable Expansion ==="
demo_test "Simple variable" "export VAR=test\necho \$VAR" "test"
demo_test "Exit status" "true\necho \$?" "0"
demo_test "Multiple variables" "export A=hello\nexport B=world\necho \$A \$B" "hello world"

echo "=== Quotes ==="
demo_test "Double quotes" "echo \"hello world\"" "hello world"
demo_test "Single quotes (no expansion)" "echo '\$VAR'" "\$VAR"
demo_test "Variable in double quotes" "export X=test\necho \"\$X\"" "test"

echo "=== Redirects ==="
# These need special handling
echo -e "${BLUE}Test:${NC} Output redirect"
printf "echo hello > /tmp/mstest.txt\ncat /tmp/mstest.txt\nexit\n" | $MINISHELL > /dev/null 2>&1
if [ "$(cat /tmp/mstest.txt 2>/dev/null)" = "hello" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Redirect works"
else
    echo -e "${RED}✗ FAIL${NC} - Redirect failed"
fi
rm -f /tmp/mstest.txt
echo ""

echo -e "${BLUE}Test:${NC} Append redirect"
printf "echo line1 > /tmp/mstest.txt\necho line2 >> /tmp/mstest.txt\ncat /tmp/mstest.txt\nexit\n" | $MINISHELL > /dev/null 2>&1
result=$(cat /tmp/mstest.txt 2>/dev/null | tr '\n' ' ' | sed 's/ $//')
if [ "$result" = "line1 line2" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Append works"
else
    echo -e "${RED}✗ FAIL${NC} - Expected 'line1 line2', Got '$result'"
fi
rm -f /tmp/mstest.txt
echo ""

echo "=== Pipes ==="
demo_test "Simple pipe" "echo hello | cat" "hello"
demo_test "Multiple pipes" "echo test | cat | cat" "test"

echo "=== Logical Operators ==="
demo_test "AND success" "true && echo yes" "yes"
demo_test "AND failure" "false && echo no" ""
demo_test "OR success" "true || echo no" ""
demo_test "OR failure" "false || echo yes" "yes"

echo "=== Wildcards ==="
echo -e "${BLUE}Test:${NC} Wildcard expansion"
mkdir -p /tmp/mstest_wild
touch /tmp/mstest_wild/{a,b,c}.txt
result=$(cd /tmp/mstest_wild && printf "echo *.txt\nexit\n" | $MINISHELL 2>&1 | grep -v "^minishell>" | grep -v "^exit$" | grep "txt")
rm -rf /tmp/mstest_wild
if [[ "$result" == *"a.txt"* ]] && [[ "$result" == *"b.txt"* ]] && [[ "$result" == *"c.txt"* ]]; then
    echo -e "${GREEN}✓ PASS${NC} - Wildcards expanded correctly"
else
    echo -e "${RED}✗ FAIL${NC} - Wildcard expansion failed: $result"
fi
echo ""

echo "=== Heredoc ==="
echo -e "${BLUE}Test:${NC} Heredoc"
result=$(printf "cat << EOF\nhello from heredoc\nEOF\nexit\n" | $MINISHELL 2>&1 | grep "hello from heredoc")
if [ "$result" = "hello from heredoc" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Heredoc works"
else
    echo -e "${RED}✗ FAIL${NC} - Heredoc failed"
fi
echo ""

echo -e "${BLUE}Test:${NC} Heredoc with expansion"
result=$(printf "export VAR=expanded\ncat << EOF\nValue: \$VAR\nEOF\nexit\n" | $MINISHELL 2>&1 | grep "Value:" | sed 's/^Value: //')
if [ "$result" = "expanded" ]; then
    echo -e "${GREEN}✓ PASS${NC} - Heredoc expansion works"
else
    echo -e "${RED}✗ FAIL${NC} - Expected 'expanded', Got '$result'"
fi
echo ""

echo "=== Complex Scenarios ==="
demo_test "Expansion + pipe" "export MSG=hello\necho \$MSG | cat" "hello"
demo_test "Quotes + expansion" "export X=world\necho \"hello \$X\"" "hello world"
demo_test "Logical + pipe" "echo test | cat && echo success" "success"

echo "=================================="
echo "Feature demonstration complete!"
echo "=================================="
