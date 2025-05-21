/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:58:40 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/05/21 13:44:25 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

static void	ctrl_c(int sig)
{
	(void)sig;
	if (g_state_signal != 2)
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	else if (g_state_signal == 2)
		write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_state_signal = 130;
}

void	handle_signal(void)
{
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}
