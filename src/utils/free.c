/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: othmaneettaqi <othmaneettaqi@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:45:04 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/25 16:31:51 by othmaneetta      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	free_all_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->cmd)
			free_tab(cmd->cmd);
		if (cmd->name)
			free_tab(cmd->name);
		if (cmd->in_or_out)
			free(cmd->in_or_out);
		if (cmd->full_path)
			free(cmd->full_path);
		free(cmd);
		cmd = tmp;
	}
}

void	free_all_cmd_bis(t_cmd **cmd)
{
	t_cmd	*tmp;


	while (*cmd)
	{
		tmp = (*cmd)->next;
		if ((*cmd)->cmd)
			free_tab((*cmd)->cmd);
		if ((*cmd)->name)
			free_tab((*cmd)->name);
		if ((*cmd)->in_or_out)
			free((*cmd)->in_or_out);
		if ((*cmd)->full_path)
			free((*cmd)->full_path);
		free(*cmd);
		*cmd = tmp;
	}
	*cmd = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->cmd)
		free_tab(cmd->cmd);
	if (cmd->name)
		free_tab(cmd->name);
	if (cmd->in_or_out)
		free(cmd->in_or_out);
	if (cmd->full_path)
		free(cmd->full_path);
	free(cmd);
}

void	free_cmd_env_pipe(t_info *info, t_env *env, t_pipe *pipe_fd)
{
	free_all_cmd(info->cmd_origin);
	free_tab(env->envp);
	free(pipe_fd);
	free(env);
	free(info);
	exit(127);
}

void	free_cmd_env_pipe_bis(t_info *info, t_env *env, t_pipe *pipe_fd)
{
	free_all_cmd(info->cmd_origin);
	free_tab(env->envp);
	free(pipe_fd);
	free(env);
	free(info);
	exit(1);
}
