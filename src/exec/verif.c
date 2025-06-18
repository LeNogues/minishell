/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:28:19 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/18 23:07:35 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

static int	open_all(t_info *info, t_pipe *pipe_fd, int i)
{
	if (info->cmd->in_or_out[i] == INPUT)
		return (open_in(info->cmd, info->cmd->name[i]));
	else if (info->cmd->in_or_out[i] == OUTPUT_APPEND
		|| info->cmd->in_or_out[i] == OUTPUT_TRUNC)
		return (open_out(info->cmd, info->cmd->name[i],
				info->cmd->in_or_out[i]));
	else
	{
		g_state_signal = 3;
		return (open_heredoc(info->cmd, info->cmd->name[i], pipe_fd, info));
	}
}

static int	open_not_all(t_info *info, t_pipe *pipe_fd, int i)
{
	if (info->cmd->in_or_out[i] == INPUT)
		return (1);
	else if (info->cmd->in_or_out[i] == OUTPUT_APPEND
		|| info->cmd->in_or_out[i] == OUTPUT_TRUNC)
		return (open_out(info->cmd, info->cmd->name[i],
				info->cmd->in_or_out[i]));
	else
		return (open_heredoc_bis(info->cmd, info->cmd->name[i], pipe_fd));
}

int	verif_file_builtin(t_info *info, t_pipe *pipe_fd)
{
	int	i;
	int	result;

	if (!info->cmd->name)
		return (1);
	i = 0;
	if (!dup2(STDOUT_FILENO, info->cmd->fd_stdout))
	{
		close_pipe_fd(pipe_fd->old);
		return (0);
	}
	while (info->cmd->name[i])
	{
		result = open_not_all(info, pipe_fd, i);
		if (!result)
		{
			write(2, "could not open file\n", 20);
			dup2(info->cmd->fd_stdout, STDOUT_FILENO);
			return (close_pipe_fd(pipe_fd->old), 0);
		}
		if (result == -1)
			return (close_pipe_fd(pipe_fd->old), 0);
		i++;
	}
	return (1);
}

int	verif_file(t_info *info, t_pipe *pipe_fd)
{
	int	i;
	int	result;

	if (!info->cmd->name)
		return (1);
	i = 0;
	while (info->cmd->name[i])
	{
		result = open_all(info, pipe_fd, i);
		if (!result)
		{
			write(2, "could not open file\n", 20);
			close_pipe_fd(pipe_fd->old);
			return (0);
		}
		if (result == -1)
		{
			close_pipe_fd(pipe_fd->old);
			return (0);
		}
		i++;
	}
	return (1);
}
