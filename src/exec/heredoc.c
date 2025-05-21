/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:29:05 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/05/21 16:15:20 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

static void	put_line(char *line, t_pipe *pipe_fd)
{
	ft_putstr_fd(line, pipe_fd->heredoc[1]);
	ft_putstr_fd("\n", pipe_fd->heredoc[1]);
}

int	open_heredoc(t_cmd *cmd, char *limiter, t_pipe *pipe_fd)
{
	char		*line;
	static int	passage = 0;

	g_state_signal = 3;
	if (passage == 0)
		pipe(pipe_fd->heredoc);
	passage++;
	signal(SIGQUIT, SIG_IGN);
	while (g_state_signal == 3)
	{
		line = readline("heredoc > ");
		if (!line || g_state_signal != 3)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			if (passage == cmd->heredoc)
				dup_heredoc(pipe_fd);
			return (free(line), 1);
		}
		if (passage == cmd->heredoc)
			put_line(line, pipe_fd);
		free(line);
	}
	close_pipe_fd(pipe_fd->heredoc);
	return (-1);
}
