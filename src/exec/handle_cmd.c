/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seb <seb@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:37:22 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/11 16:46:04 by seb              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	dup_no_fd(t_cmd *cmd, t_pipe *pipe_fd)
{
	if (cmd->pos == 1)
		dup_first(cmd, pipe_fd);
	if (cmd->pos == 2)
		dup_middle(cmd, pipe_fd);
	if (cmd->pos == 3)
		dup_last(cmd, pipe_fd);
}

static void	ctrl_back(int sig)
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

static void	ctrl_c_bis(int sig)
{
	(void)sig;
	g_state_signal = 130;
}

static void	handle_signal_bis(void)
{
	signal(SIGQUIT, ctrl_back);
	signal(SIGINT, ctrl_c_bis);
}

void	handle_cmd(t_info *info, t_pipe *pipe_fd)
{
	int	id;
	int	built;

	built = -1;
	id = -1;
	if (info->cmd->nb_cmd == 1)
		built = choice_of_builtin(info, info->env, pipe_fd);
	handle_signal_bis();
	if (built == -1)
		id = fork();
	g_state_signal = 2;
	if (id == 0 && built == -1)
	{
		signal(SIGINT, ctrl_c);
		if (!verif_file(info, pipe_fd))
			free_cmd_env_pipe(info, info->env, pipe_fd);
		dup_no_fd(info->cmd, pipe_fd);
		if (choice_of_builtin(info, info->env, pipe_fd) == -1)
			execute(info, info->env, pipe_fd);
		else
			free_cmd_env_pipe(info, info->env, pipe_fd);
	}
	info->last_pid = id;
	if (g_state_signal == 130 || g_state_signal == 131)
		info->return_value = g_state_signal;
	if (info->cmd->nb_cmd == 1)
		close_pipe_fd(pipe_fd->old);
}
