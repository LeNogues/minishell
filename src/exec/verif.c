/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:28:19 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/05/21 12:25:43 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	verif_file(t_info *info, t_pipe *pipe_fd)
{
	int	i;
	int	result;

	if (!info->cmd->name)
		return (1);
	i = 0;
	while (info->cmd->name[i])
	{
		if (info->cmd->in_or_out[i] == INPUT)
			result = open_in(info->cmd, info->cmd->name[i]);
		else if (info->cmd->in_or_out[i] == OUTPUT_APPEND
			|| info->cmd->in_or_out[i] == OUTPUT_TRUNC)
			result = open_out(info->cmd, info->cmd->name[i],
					info->cmd->in_or_out[i]);
		else
			result = open_heredoc(info->cmd, info->cmd->name[i], pipe_fd);
		if (!result)
		{
			write(2, "could not execute file\n", 24);
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
