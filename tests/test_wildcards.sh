#!/bin/bash

echo ""
echo "=== Wildcard Tests ==="
echo ""

run_test "wildcard_star_all" \
    "echo *" \
    "Wildcard matches all files"

run_test "wildcard_extension" \
    "echo *.txt" \
    "Wildcard matches by extension"

run_test "wildcard_prefix" \
    "echo file*" \
    "Wildcard matches by prefix"

run_test "wildcard_no_match" \
    "echo *.nonexistent" \
    "Wildcard with no matches (literal)"

run_test "wildcard_in_command" \
    "cat *.txt" \
    "Wildcard in command argument"

run_test "wildcard_mixed" \
    "echo *.txt file1" \
    "Wildcard mixed with literal"

run_test "wildcard_multiple" \
    "echo * *" \
    "Multiple wildcards"

run_test "wildcard_hidden_exclude" \
    "touch .hidden && echo *" \
    "Wildcard excludes hidden files"

run_test "wildcard_with_path" \
    "echo subdir/*" \
    "Wildcard in subdirectory"

run_test "wildcard_empty_dir" \
    "mkdir emptydir && echo emptydir/*" \
    "Wildcard in empty directory"

run_test "wildcard_middle" \
    "echo f*1" \
    "Wildcard in middle of pattern"

run_test "wildcard_multiple_stars" \
    "echo *.*" \
    "Multiple stars in pattern"

run_test "wildcard_ls" \
    "ls *.txt" \
    "Wildcard with ls command"

run_test "wildcard_cat" \
    "cat file*" \
    "Wildcard with cat command"
