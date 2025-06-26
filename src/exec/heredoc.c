/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: othmaneettaqi <othmaneettaqi@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:29:05 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/26 12:15:24 by othmaneetta      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

// static void	put_line(char *line, t_pipe *pipe_fd)
// {
// 	ft_putstr_fd(line, pipe_fd->heredoc[1]);
// 	ft_putstr_fd("\n", pipe_fd->heredoc[1]);
// }

int	open_heredoc_bis(t_cmd *cmd, char *limiter, t_pipe *pipe_fd)
{
	char		*line;

	g_state_signal = 3;
	(void)cmd;
	(void)pipe_fd;
	signal(SIGQUIT, SIG_IGN);
	while (g_state_signal == 3)
	{
		line = readline("heredoc > ");
		if (!line || g_state_signal != 3)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
			return (free(line), 1);
		free(line);
	}
	return (-1);
}

int	open_heredoc(t_cmd *cmd, char *limiter, t_info *info)
{
	char	*line;
	int		pipe_hd[2];

	if (pipe(pipe_hd) == -1)
		return (perror("pipe"), 0);
	g_state_signal = 3;
	signal(SIGQUIT, SIG_IGN);
	while (g_state_signal == 3)
	{
		line = readline("heredoc > ");
		if (!line || g_state_signal != 3)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		line = expand_for_heredoc(line, info);
		write(pipe_hd[1], line, ft_strlen(line));
		write(pipe_hd[1], "\n", 1);
		free(line);
	}
	if (g_state_signal != 3)
	{
		close(pipe_hd[0]);
		close(pipe_hd[1]);
		info->return_value = 130;
		return (-1);
	}
	close(pipe_hd[1]);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = pipe_hd[0];
	return (1);
}
