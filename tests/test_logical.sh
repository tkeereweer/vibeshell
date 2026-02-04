#!/bin/bash

echo ""
echo "=== Logical Operator Tests ==="
echo ""

run_test "and_success" \
    "true && echo success" \
    "AND operator with success"

run_test "and_failure" \
    "false && echo should_not_print" \
    "AND operator with failure"

run_test "or_success" \
    "true || echo should_not_print" \
    "OR operator with success"

run_test "or_failure" \
    "false || echo failure" \
    "OR operator with failure"

run_test "and_chain" \
    "true && true && echo success" \
    "Chained AND operators"

run_test "or_chain" \
    "false || false || echo finally" \
    "Chained OR operators"

run_test "mixed_logical" \
    "true && echo yes || echo no" \
    "Mixed AND and OR"

run_test "complex_logical" \
    "false || true && echo success" \
    "Complex logical expression"

run_test "and_with_command" \
    "echo first && echo second" \
    "AND with actual commands"

run_test "or_with_command" \
    "ls nonexistent 2>/dev/null || echo not_found" \
    "OR with failing command"

run_test "logical_with_vars" \
    "export VAR=test && true && echo \$VAR" \
    "Logical operators with variables"

run_test "logical_precedence" \
    "false && echo no || echo yes" \
    "Logical operator precedence"

run_test "and_or_and" \
    "true && false || echo rescue" \
    "AND OR AND pattern"

run_test "logical_exit_status" \
    "true && false; echo \$?" \
    "Exit status with logical operators"
