/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:46 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/04/23 11:57:11 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	execute(t_cmd *cmd, t_env *env, t_pipe *pipe_fd, t_cmd *cmd_origin)
{
	cmd->full_path = verif_arg(cmd, cmd_origin, pipe_fd, env);
	if (cmd->full_path)
		execve(cmd->full_path, cmd->cmd, env->envp);
	free_cmd_env_pipe(cmd_origin, env, pipe_fd);
}
