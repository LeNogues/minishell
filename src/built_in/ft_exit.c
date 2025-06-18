/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seb <seb@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:50:10 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/16 16:22:15 by seb              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	exit_clean(unsigned int return_value, t_info *info, t_env *env,
		t_pipe *pipe_fd)
{
	free_all_cmd(info->cmd);
	free_tab(env->envp);
	free(env);
	close_pipe_fd(pipe_fd->old);
	free(pipe_fd);
	free(info);
	exit(return_value);
}

int	ft_exit(t_info *info, t_env *env, t_pipe *pipe_fd)
{
	int				result;
	long			exit_value_long;
	unsigned int	exit_value_int;

	result = ft_atol((*info).cmd->cmd[1], &exit_value_long);
	exit_value_int = exit_value_long;
	if (result == 1)
	{
		if (!(*info).cmd->cmd[1])
			exit_clean(exit_value_int, info, env, pipe_fd);
		else if (!(*info).cmd->cmd[2])
			exit_clean(exit_value_int, info, env, pipe_fd);
		else if ((*info).cmd->cmd[2] != 0)
			write(1, "too many arguments\n", 19);
	}
	else if (result == -1)
	{
		write(1, "only numeric argument\n", 22);
		exit_clean(2, info, env, pipe_fd);
	}
	return (1);
}
