/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_verif.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oettaqi <oettaqi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:39:42 by othmaneetta       #+#    #+#             */
/*   Updated: 2025/05/30 16:47:03 by oettaqi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	syntax_verif(t_token **head)
{
	t_token	*current;

	current = *head;
	if (current == NULL || current->type == PIPE)
		return (printf("syntax error\n"), 1);
	while (current)
	{
		if (is_redir_type(current->type))
		{
			if (handle_redir_syntax(&current))
				return (1);
		}
		else if (current->type == PIPE)
		{
			if (handle_pipe_syntax(&current))
				return (1);
		}
		else if (current->type == ERROR)
			return (printf("syntax error\n"), 1);
		else if (current->type == COMMAND && current->start[0] == '<')
		{
			if (find_next_significant(current)->type == HEREDOC)
				return (printf("syntax error\n"), 1);
		}
		else if (current->type == COMMAND && current->start[0] == '>')
		{
			if (find_next_significant(current)->type == APPEND)
				return (printf("syntax error\n"), 1);
		}
		else
			current = current->next;
	}
	return (0);
}
