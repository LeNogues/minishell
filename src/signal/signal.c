/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:58:40 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/18 23:04:58 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	ctrl_c(int sig)
{
	(void)sig;
	if (g_state_signal != 2)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	}
	rl_on_new_line();
	rl_replace_line("", 0);
	g_state_signal = 130;
}

void	ctrl_back(int sig)
{
	(void)sig;
	if (g_state_signal != 1)
	{
		write(2, "Quit (core dumped)\n", 19);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_state_signal = 131;
	}
}

void	ctrl_back_bis(int sig)
{
	int			i;
	const char	*str;

	(void)sig;
	i = 0;
	while (i < 500000)
		i++;
	str = "\b \b\b \b\033[K";
	write(STDOUT_FILENO, str, strlen(str));
	str = "\033[1A\033[2Kheredoc > ";
	write(STDOUT_FILENO, str, strlen(str));
}

void	handle_signal(void)
{
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}
