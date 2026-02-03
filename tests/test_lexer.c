/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

static char	*token_type_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == TOKEN_REDIR_HEREDOC)
		return ("REDIR_HEREDOC");
	if (type == TOKEN_AND)
		return ("AND");
	if (type == TOKEN_OR)
		return ("OR");
	if (type == TOKEN_LPAREN)
		return ("LPAREN");
	if (type == TOKEN_RPAREN)
		return ("RPAREN");
	if (type == TOKEN_EOF)
		return ("EOF");
	return ("UNKNOWN");
}

static void	print_token(t_token *token)
{
	if (token->value)
		printf("[%s: '%s']\n", token_type_str(token->type), token->value);
	else
		printf("[%s]\n", token_type_str(token->type));
}

static void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		print_token(current);
		current = current->next;
	}
}

static void	test_case(char *input)
{
	t_token	*tokens;

	printf("\nTest: \"%s\"\n", input);
	printf("Tokens:\n");
	tokens = tokenize(input);
	print_tokens(tokens);
	token_list_free(tokens);
}

int	main(void)
{
	printf("=== Tokenizer Tests ===\n");
	test_case("echo hello world");
	test_case("ls -la | grep test");
	test_case("cat < input.txt > output.txt");
	test_case("cat file >> log.txt");
	test_case("cat << EOF");
	test_case("echo 'hello world'");
	test_case("echo \"hello $USER\"");
	test_case("cmd1 && cmd2 || cmd3");
	test_case("(echo test) | cat");
	test_case("echo a&&echo b||echo c");
	return (0);
}
