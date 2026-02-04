/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 14:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

extern volatile sig_atomic_t	g_sigint_received;

void	init_signals(void);
void	ignore_signals(void);
void	default_signals(void);
void	sigint_handler(int sig);
void	sigint_heredoc_handler(int sig);
void	heredoc_signals(void);

#endif
