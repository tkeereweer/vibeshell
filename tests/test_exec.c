/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/executor.h"
#include <stdio.h>
#include <string.h>

extern char	**environ;

void	print_status(char *test, int status)
{
	printf("%-40s", test);
	if (status == 0)
		printf(" ✓ (status: %d)\n", status);
	else
		printf(" ✗ (status: %d)\n", status);
}

void	test_echo(t_exec_ctx *ctx)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	printf("\n=== Testing echo built-in ===\n");
	tokens = tokenize("echo hello world");
	ast = parse_tokens(tokens);
	printf("Expected: hello world\n");
	printf("Got:      ");
	status = execute_ast(ast, ctx);
	print_status("echo hello world", status);
	free_ast(ast);
	free_tokens(tokens);
	tokens = tokenize("echo -n test");
	ast = parse_tokens(tokens);
	printf("\nExpected: test (no newline)\n");
	printf("Got:      ");
	status = execute_ast(ast, ctx);
	printf("\n");
	print_status("echo -n test", status);
	free_ast(ast);
	free_tokens(tokens);
}

void	test_pwd(t_exec_ctx *ctx)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	printf("\n=== Testing pwd built-in ===\n");
	tokens = tokenize("pwd");
	ast = parse_tokens(tokens);
	printf("Current directory:\n");
	status = execute_ast(ast, ctx);
	print_status("pwd", status);
	free_ast(ast);
	free_tokens(tokens);
}

void	test_env(t_exec_ctx *ctx)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	printf("\n=== Testing env built-in (first 5 vars) ===\n");
	tokens = tokenize("env");
	ast = parse_tokens(tokens);
	status = execute_ast(ast, ctx);
	print_status("env", status);
	free_ast(ast);
	free_tokens(tokens);
}

void	test_external_commands(t_exec_ctx *ctx)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	printf("\n=== Testing external commands ===\n");
	tokens = tokenize("/bin/echo external command test");
	ast = parse_tokens(tokens);
	printf("Expected: external command test\n");
	printf("Got:      ");
	status = execute_ast(ast, ctx);
	print_status("/bin/echo", status);
	free_ast(ast);
	free_tokens(tokens);
	tokens = tokenize("ls -la");
	ast = parse_tokens(tokens);
	printf("\nListing directory:\n");
	status = execute_ast(ast, ctx);
	print_status("ls -la (PATH resolution)", status);
	free_ast(ast);
	free_tokens(tokens);
}

void	test_logical_operators(t_exec_ctx *ctx)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	printf("\n=== Testing logical operators ===\n");
	tokens = tokenize("echo first && echo second");
	ast = parse_tokens(tokens);
	printf("Expected: first\\nsecond\n");
	printf("Got:\n");
	status = execute_ast(ast, ctx);
	print_status("echo first && echo second", status);
	free_ast(ast);
	free_tokens(tokens);
	tokens = tokenize("/bin/false || echo fallback");
	ast = parse_tokens(tokens);
	printf("\nExpected: fallback\n");
	printf("Got:\n");
	status = execute_ast(ast, ctx);
	print_status("/bin/false || echo fallback", status);
	free_ast(ast);
	free_tokens(tokens);
}

int	main(void)
{
	t_exec_ctx	*ctx;

	printf("=== Minishell Execution Tests ===\n");
	ctx = init_exec_ctx(environ);
	if (!ctx)
	{
		fprintf(stderr, "Failed to initialize execution context\n");
		return (1);
	}
	test_echo(ctx);
	test_pwd(ctx);
	test_env(ctx);
	test_external_commands(ctx);
	test_logical_operators(ctx);
	printf("\n=== All tests complete ===\n");
	cleanup_exec_ctx(ctx);
	return (0);
}
