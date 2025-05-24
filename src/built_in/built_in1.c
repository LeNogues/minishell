/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:12:07 by seb               #+#    #+#             */
/*   Updated: 2025/05/24 12:49:47 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	choice_of_builtin(t_info *info, t_env *env, t_pipe *pipe_fd)
{
	if (!info->cmd->cmd)
		return (0);
	if (ft_strncmp(info->cmd->cmd[0], "cd", 3) == 0)
		return (ft_cd(info->cmd->cmd, env), 1);
	else if (ft_strncmp(info->cmd->cmd[0], "pwd", 4) == 0)
		return (ft_pwd(env), 1);
	else if (ft_strncmp(info->cmd->cmd[0], "echo", 5) == 0)
		return (ft_echo(info->cmd), 1);
	else if (ft_strncmp(info->cmd->cmd[0], "env", 4) == 0)
		return (ft_env(env), 1);
	else if (ft_strncmp(info->cmd->cmd[0], "unset", 6) == 0)
		return (ft_unset(info->cmd->cmd, env), 1);
	else if (ft_strncmp(info->cmd->cmd[0], "export", 7) == 0)
		return (ft_export(info->cmd->cmd, env), 1);
	if (ft_strncmp(info->cmd->cmd[0], "exit", 5) == 0)
		return (ft_exit(info, env, pipe_fd));
	return (0);
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
	}
}
