#!/bin/bash

# Comprehensive Minishell Test Runner
# Compares minishell output with bash output

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MINISHELL="$SCRIPT_DIR/../minishell"
PASSED=0
FAILED=0
SKIPPED=0

# Create test directories
mkdir -p "$SCRIPT_DIR/tmp"
mkdir -p "$SCRIPT_DIR/results"

# Cleanup function
cleanup() {
    rm -rf "$SCRIPT_DIR/tmp"/*
    rm -f "$SCRIPT_DIR/results"/bash_out "$SCRIPT_DIR/results"/minishell_out
    rm -f "$SCRIPT_DIR/results"/bash_err "$SCRIPT_DIR/results"/minishell_err
}

# Setup test environment
setup() {
    cleanup
    cd "$SCRIPT_DIR/tmp"
    # Create test files
    echo "test content" > testfile
    echo "line1" > file1
    echo "line2" > file2
    echo "line3" > file3
    touch empty
    echo "abc" > a.txt
    echo "def" > b.txt
    echo "ghi" > c.txt
    mkdir -p subdir
    echo "nested" > subdir/nested.txt
    cd "$SCRIPT_DIR/.."
}

# Run test function
run_test() {
    local test_name="$1"
    local test_cmd="$2"
    local description="$3"

    echo -e "${BLUE}Testing:${NC} $test_name"
    echo -e "${YELLOW}Command:${NC} $test_cmd"
    echo -e "${YELLOW}Description:${NC} $description"

    # Run in bash
    (cd "$SCRIPT_DIR/tmp" && bash -c "$test_cmd" > "$SCRIPT_DIR/results/bash_out" 2> "$SCRIPT_DIR/results/bash_err"; echo $? > "$SCRIPT_DIR/results/bash_status")

    # Run in minishell
    (cd "$SCRIPT_DIR/tmp" && echo "$test_cmd" | "$MINISHELL" > "$SCRIPT_DIR/results/minishell_out" 2> "$SCRIPT_DIR/results/minishell_err"; echo $? > "$SCRIPT_DIR/results/minishell_status")

    # Compare outputs
    bash_out=$(cat "$SCRIPT_DIR/results/bash_out")
    minishell_out=$(cat "$SCRIPT_DIR/results/minishell_out")
    bash_status=$(cat "$SCRIPT_DIR/results/bash_status")
    minishell_status=$(cat "$SCRIPT_DIR/results/minishell_status")

    if [ "$bash_out" = "$minishell_out" ] && [ "$bash_status" = "$minishell_status" ]; then
        echo -e "${GREEN}✓ PASSED${NC}\n"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAILED${NC}"
        echo -e "${YELLOW}Expected (bash):${NC}"
        cat "$SCRIPT_DIR/results/bash_out"
        echo -e "${YELLOW}Got (minishell):${NC}"
        cat "$SCRIPT_DIR/results/minishell_out"
        echo -e "${YELLOW}Bash status: $bash_status, Minishell status: $minishell_status${NC}\n"
        ((FAILED++))
    fi
}

# Print summary
print_summary() {
    echo ""
    echo "=================================="
    echo "Test Summary"
    echo "=================================="
    echo -e "${GREEN}Passed: $PASSED${NC}"
    echo -e "${RED}Failed: $FAILED${NC}"
    echo -e "${YELLOW}Skipped: $SKIPPED${NC}"
    echo "=================================="

    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed!${NC}"
        exit 1
    fi
}

# Main
echo "=================================="
echo "Minishell Comprehensive Test Suite"
echo "=================================="
echo ""

if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: minishell not found. Please run 'make' first.${NC}"
    exit 1
fi

setup

# Source all test modules
source "$SCRIPT_DIR/test_basic.sh"
source "$SCRIPT_DIR/test_redirects.sh"
source "$SCRIPT_DIR/test_expansion.sh"
source "$SCRIPT_DIR/test_heredoc.sh"
source "$SCRIPT_DIR/test_wildcards.sh"
source "$SCRIPT_DIR/test_quotes.sh"
source "$SCRIPT_DIR/test_pipes.sh"
source "$SCRIPT_DIR/test_logical.sh"
source "$SCRIPT_DIR/test_exit_status.sh"
source "$SCRIPT_DIR/test_edge_cases.sh"

cleanup
print_summary
