#!/bin/bash

echo ""
echo "=== Basic Command Tests ==="
echo ""

run_test "basic_echo" \
    "echo hello world" \
    "Simple echo command"

run_test "echo_no_newline" \
    "echo -n hello" \
    "Echo without newline"

run_test "echo_multiple_flags" \
    "echo -n -n -n hello" \
    "Echo with multiple -n flags"

run_test "pwd" \
    "pwd" \
    "Print working directory"

run_test "env_display" \
    "export TEST=value && env | grep TEST" \
    "Display environment variables"

run_test "export_var" \
    "export FOO=bar && echo \$FOO" \
    "Export and display variable"

run_test "export_append" \
    "export PATH=\$PATH:/new/path && echo \$PATH | grep /new/path" \
    "Append to PATH"

run_test "unset_var" \
    "export FOO=bar && unset FOO && echo \$FOO" \
    "Unset variable"

run_test "cd_relative" \
    "cd subdir && pwd" \
    "Change directory (relative)"

run_test "cd_back" \
    "cd subdir && cd .. && pwd" \
    "Change directory back"

run_test "external_command" \
    "ls -la testfile" \
    "Execute external command"

run_test "command_with_args" \
    "cat testfile" \
    "Command with file argument"

run_test "empty_command" \
    "" \
    "Empty command line"

run_test "whitespace_only" \
    "   " \
    "Whitespace only"
