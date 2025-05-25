/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:12:07 by seb               #+#    #+#             */
/*   Updated: 2025/05/25 15:07:53 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	choice_of_builtin(t_info *info, t_env *env, t_pipe *pipe_fd)
{
	int result;
	int fd_dup;
	int fd_in;

	result = 0;
	fd_dup = 0;
	fd_in = 0;
	if (info->cmd->nb_cmd == 1)
	{
		dup2(STDOUT_FILENO, fd_dup);// protege
		dup2(STDIN_FILENO, fd_in);// protege
		if (!verif_file_builtin(info, pipe_fd))
			return (1);
	}
	if (!info->cmd->cmd)
		return (0);
	if (ft_strncmp(info->cmd->cmd[0], "cd", 3) == 0)
		result = ft_cd(info->cmd->cmd, env);
	else if (ft_strncmp(info->cmd->cmd[0], "pwd", 4) == 0)
		result = ft_pwd(env);
	else if (ft_strncmp(info->cmd->cmd[0], "echo", 5) == 0)
		result = ft_echo(info->cmd);
	else if (ft_strncmp(info->cmd->cmd[0], "env", 4) == 0)
		result = ft_env(env);
	else if (ft_strncmp(info->cmd->cmd[0], "unset", 6) == 0)
		result = ft_unset(info->cmd->cmd, env);
	else if (ft_strncmp(info->cmd->cmd[0], "export", 7) == 0)
		result = ft_export(info->cmd->cmd, env);
	if (ft_strncmp(info->cmd->cmd[0], "exit", 5) == 0)
		result = ft_exit(info, env, pipe_fd);
	if (info->cmd->nb_cmd == 1)
	{
		if (info->cmd->fd_out)
		{
			
			close(info->cmd->fd_out);
			dup2(fd_dup, STDOUT_FILENO);
		}
		close_pipe_fd(info->pipe->heredoc);
		dup2(fd_in, STDIN_FILENO);
	}
	
	return (result);
}

void	hub(t_info *info)
{
	char	*line;

	info->return_value = 0;
	while (1)
	{
		g_state_signal = 1;
		line = readline("\001\e[32m\002Minishell : \001\e[0m\002");
		if (!line)
			return ;
		signal(SIGQUIT, SIG_IGN);
		if (line[0] != 0)
		{
			add_history(line);
			info->cmd = merge(info, line);
			if (info->cmd)
			{
				info->cmd_origin = info->cmd;
				exec(info);
				free_all_cmd(info->cmd_origin);
			}
		}
		free(line);
		signal(SIGINT, ctrl_c);
	}
}
