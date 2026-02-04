#!/bin/bash

echo ""
echo "=== Redirection Tests ==="
echo ""

run_test "redirect_output" \
    "echo hello > out.txt && cat out.txt" \
    "Redirect output to file"

run_test "redirect_append" \
    "echo line1 > out.txt && echo line2 >> out.txt && cat out.txt" \
    "Append to file"

run_test "redirect_input" \
    "cat < testfile" \
    "Redirect input from file"

run_test "redirect_both" \
    "cat < testfile > out.txt && cat out.txt" \
    "Redirect both input and output"

run_test "multiple_redirects" \
    "echo hello > out1.txt > out2.txt > out3.txt && cat out3.txt" \
    "Multiple output redirects (last wins)"

run_test "redirect_order" \
    "> out.txt echo hello && cat out.txt" \
    "Redirect before command"

run_test "redirect_mixed_order" \
    "echo hello > out.txt world && cat out.txt" \
    "Redirect in middle of arguments"

run_test "redirect_overwrite" \
    "echo old > out.txt && echo new > out.txt && cat out.txt" \
    "Overwrite file"

run_test "redirect_empty" \
    "> empty_out.txt && cat empty_out.txt" \
    "Create empty file with redirect"

run_test "many_redirects" \
    "echo test > a > b > c > d > e && cat e" \
    "Many output redirects"

run_test "redirect_with_spaces" \
    "echo hello >    out.txt && cat out.txt" \
    "Redirect with multiple spaces"

run_test "input_output_combo" \
    "cat < file1 > out.txt && cat out.txt" \
    "Input and output redirect combined"

run_test "append_multiple" \
    "echo a >> out.txt && echo b >> out.txt && echo c >> out.txt && cat out.txt" \
    "Multiple appends to same file"

run_test "redirect_nonexistent" \
    "cat < nonexistent.txt 2>&1" \
    "Redirect from nonexistent file"
