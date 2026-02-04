#!/bin/bash

echo ""
echo "=== Pipe Tests ==="
echo ""

run_test "pipe_simple" \
    "echo hello | cat" \
    "Simple pipe"

run_test "pipe_multiple" \
    "echo hello | cat | cat | cat" \
    "Multiple pipes"

run_test "pipe_with_args" \
    "echo hello world | grep hello" \
    "Pipe with command arguments"

run_test "pipe_cat_grep" \
    "cat testfile | grep test" \
    "Cat piped to grep"

run_test "pipe_echo_wc" \
    "echo hello world | wc -w" \
    "Echo piped to word count"

run_test "pipe_ls_grep" \
    "ls | grep testfile" \
    "Ls piped to grep"

run_test "pipe_three_commands" \
    "echo abc | cat | cat" \
    "Three commands in pipeline"

run_test "pipe_with_redirect" \
    "echo hello | cat > pipe_out.txt && cat pipe_out.txt" \
    "Pipe with output redirect"

run_test "pipe_empty" \
    "echo | cat" \
    "Pipe empty output"

run_test "pipe_multi_line" \
    "printf 'line1\nline2\nline3' | grep line2" \
    "Pipe multi-line output"

run_test "pipe_with_vars" \
    "export VAR=hello && echo \$VAR | cat" \
    "Pipe with variable expansion"

run_test "pipe_failure" \
    "ls nonexistent 2>&1 | grep nonexistent" \
    "Pipe with failing command"

run_test "pipe_to_head" \
    "printf 'a\nb\nc\nd' | head -2" \
    "Pipe to head command"

run_test "pipe_sort" \
    "printf 'c\na\nb' | sort" \
    "Pipe to sort"
