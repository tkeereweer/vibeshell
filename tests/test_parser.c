/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

static void	print_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
	{
		printf(" %s", argv[i]);
		i++;
	}
}

static void	print_redirects(t_redirect *redir)
{
	char	*type_str;

	while (redir)
	{
		if (redir->type == REDIR_IN)
			type_str = "<";
		else if (redir->type == REDIR_OUT)
			type_str = ">";
		else if (redir->type == REDIR_APPEND)
			type_str = ">>";
		else
			type_str = "<<";
		printf(" [%s %s]", type_str, redir->file);
		redir = redir->next;
	}
}

static void	print_ast_rec(t_ast_node *node, int depth);

static void	print_cmd_node(t_ast_node *node, int depth)
{
	print_indent(depth);
	printf("COMMAND:");
	print_argv(node->data.cmd.argv);
	print_redirects(node->data.cmd.redirects);
	printf("\n");
}

static void	print_binop_node(t_ast_node *node, int depth)
{
	char	*op;

	if (node->type == NODE_PIPE)
		op = "PIPE";
	else if (node->type == NODE_AND)
		op = "AND";
	else
		op = "OR";
	print_indent(depth);
	printf("%s\n", op);
	print_ast_rec(node->data.binop.left, depth + 1);
	print_ast_rec(node->data.binop.right, depth + 1);
}

static void	print_subshell_node(t_ast_node *node, int depth)
{
	print_indent(depth);
	printf("SUBSHELL");
	print_redirects(node->data.subshell.redirects);
	printf("\n");
	print_ast_rec(node->data.subshell.child, depth + 1);
}

static void	print_ast_rec(t_ast_node *node, int depth)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
		print_cmd_node(node, depth);
	else if (node->type == NODE_SUBSHELL)
		print_subshell_node(node, depth);
	else
		print_binop_node(node, depth);
}

static void	print_ast(t_ast_node *node)
{
	printf("AST:\n");
	print_ast_rec(node, 0);
}

static void	test_case(char *input)
{
	t_token		*tokens;
	t_ast_node	*ast;

	printf("\n========================================\n");
	printf("Input: \"%s\"\n", input);
	tokens = tokenize(input);
	if (!tokens)
	{
		printf("Error: tokenization failed\n");
		return ;
	}
	ast = parse_tokens(tokens);
	if (!ast)
		printf("Error: parsing failed\n");
	else
	{
		print_ast(ast);
		free_ast(ast);
	}
	token_list_free(tokens);
}

int	main(void)
{
	printf("=== Parser Tests ===\n");
	test_case("echo hello world");
	test_case("ls -la | grep test");
	test_case("cat < in.txt > out.txt");
	test_case("echo test >> log.txt");
	test_case("cat << EOF");
	test_case("cmd1 && cmd2");
	test_case("cmd1 || cmd2");
	test_case("cmd1 && cmd2 || cmd3");
	test_case("cmd1 | cmd2 | cmd3");
	test_case("(echo test) | cat");
	test_case("echo a | cat && ls");
	return (0);
}
