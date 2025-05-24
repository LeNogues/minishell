/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:46 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/05/24 14:00:29 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	execute(t_info *info, t_env *env, t_pipe *pipe_fd)
{
	info->cmd->full_path = verif_arg(info, pipe_fd, env);
	if (info->cmd->full_path)
		execve(info->cmd->full_path, info->cmd->cmd, env->envp);
	free_cmd_env_pipe(info, env, pipe_fd);
}
