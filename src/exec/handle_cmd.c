/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:37:22 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/19 12:20:56 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

static void	handle_signal_bis(void)
{
	signal(SIGQUIT, ctrl_back);
	signal(SIGINT, ctrl_c_bis);
}

int	prepare_redirections(t_info *info)
{
	int	i;
	int	result;

	info->cmd->fd_in = STDIN_FILENO;
	info->cmd->fd_out = STDOUT_FILENO;
	if (!info->cmd->name)
		return (1);
	i = 0;
	while (info->cmd->name[i])
	{
		result = open_all(info, i);
		if (result <= 0)
		{
			if (info->cmd->fd_in > STDIN_FILENO)
				close(info->cmd->fd_in);
			if (info->cmd->fd_out > STDOUT_FILENO)
				close(info->cmd->fd_out);
			return (0);
		}
		i++;
	}
	return (1);
}

void	close_redirection_fds(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
}

void	open_and_execute(t_info *info, t_pipe *pipe_fd)
{
	signal(SIGINT, ctrl_c);
	dup_no_fd(info->cmd, pipe_fd);
	close_redirection_fds(info->cmd);
	if (choice_of_builtin(info, info->env, pipe_fd) == -1)
		execute(info, info->env, pipe_fd);
	else
		free_cmd_env_pipe_bis(info, info->env, pipe_fd);
}

void	handle_cmd(t_info *info, t_pipe *pipe_fd)
{
	int	id;
	int	built;

	built = -1;
	id = -1;
	if (!prepare_redirections(info))
	{
		if (info->return_value != 130)
			info->return_value = 1;
		return ;
	}
	if (info->cmd->nb_cmd == 1)
		built = choice_of_builtin(info, info->env, pipe_fd);
	handle_signal_bis();
	if (built == -1)
		id = fork();
	g_state_signal = 2;
	if (id == 0 && built == -1)
		open_and_execute(info, pipe_fd);
	info->last_pid = id;
	if (g_state_signal == 130 || g_state_signal == 131)
		info->return_value = g_state_signal;
	close_redirection_fds(info->cmd);
	if (info->cmd->nb_cmd == 1)
		close_pipe_fd(pipe_fd->old);
}
