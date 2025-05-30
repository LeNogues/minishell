/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seb <seb@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:37:22 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/05/27 11:05:27 by seb              ###   ########.fr       */
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

void	handle_cmd(t_info *info, t_pipe *pipe_fd)
{
	int	id;
	int	built;

	built = -1;
	id = -1;
	if (info->cmd->nb_cmd == 1)
		built = choice_of_builtin(info, info->env, pipe_fd);
	signal(SIGQUIT, ctrl_back);
	signal(SIGINT, SIG_IGN);
	if (built == -1)
		id = fork();
	if (id == 0 && built == -1)
	{
		g_state_signal = 2;
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
	g_state_signal = 2;
	if (info->cmd->nb_cmd == 1)
		close_pipe_fd(pipe_fd->old);
}
