#!/bin/bash

echo ""
echo "=== Exit Status Tests ==="
echo ""

run_test "exit_success" \
    "true && echo \$?" \
    "Exit status of true"

run_test "exit_failure" \
    "false && echo \$?" \
    "Exit status of false"

run_test "exit_command_success" \
    "echo hello && echo \$?" \
    "Exit status of successful command"

run_test "exit_command_not_found" \
    "nonexistent_command 2>/dev/null; echo \$?" \
    "Exit status of command not found"

run_test "exit_echo" \
    "echo test && echo \$?" \
    "Exit status after echo"

run_test "exit_builtin_cd_success" \
    "cd . && echo \$?" \
    "Exit status of successful cd"

run_test "exit_builtin_cd_fail" \
    "cd /nonexistent 2>/dev/null; echo \$?" \
    "Exit status of failed cd"

run_test "exit_export" \
    "export VAR=value && echo \$?" \
    "Exit status of export"

run_test "exit_unset" \
    "unset PATH && echo \$?" \
    "Exit status of unset"

run_test "exit_pipe" \
    "echo hello | cat && echo \$?" \
    "Exit status after pipe"

run_test "exit_redirect" \
    "echo hello > out.txt && echo \$?" \
    "Exit status after redirect"

run_test "exit_multiple_status" \
    "true && echo \$? && false; echo \$?" \
    "Multiple exit status checks"

run_test "exit_and_operator" \
    "true && true && echo \$?" \
    "Exit status with AND operator"

run_test "exit_or_operator" \
    "false || true && echo \$?" \
    "Exit status with OR operator"

run_test "exit_status_persistence" \
    "false; VAR=\$?; echo \$VAR" \
    "Store and display exit status"

run_test "exit_heredoc_status" \
    "cat << EOF && echo \$?
test
EOF" \
    "Exit status after heredoc"

run_test "exit_wildcard_status" \
    "echo * && echo \$?" \
    "Exit status after wildcard expansion"

run_test "exit_sequence" \
    "true; false; true; echo \$?" \
    "Exit status of last command in sequence"
