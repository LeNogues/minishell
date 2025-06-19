/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oettaqi <oettaqi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:43:22 by othmaneetta       #+#    #+#             */
/*   Updated: 2025/06/19 15:56:13 by oettaqi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	expand_one_token_return_value(t_token *token_node, t_info *info, t_token **head)
{
	char	*value;

	value = ft_itoa(info->return_value);
	if (!value)
		exit_and_free_clean(head, info);
	free(token_node->start);
	token_node->start = ft_strdup(value);
	token_node->length = ft_strlen(value);
	free(value);
}

void	expand_one_token(t_token *token_node, t_info *info, t_token **head)
{
	char	*value_token;
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(token_node->length + 1);
	if (!value_token)
		exit_and_free_clean(head, info);
	while (i < (token_node->length - 1))
		value_token[i++] = token_node->start[j++];
	value_token[i++] = '=';
	value_token[i] = 0;
	value = ft_getenv(value_token, info->env);
	free(value_token);
	free(token_node->start);
	token_node->length = ft_strlen(value);
	if (!value)
	{
		token_node->start = ft_strdup("");
		return ;
	}
	token_node->start = ft_strdup(value);
	free(value);
}

int	will_expand(t_token *node, t_token **head, t_info *info)
{
	char	*test;

	test = ft_strndup(node->start, node->length);
	if (!test)
		exit_and_free_clean(head, info);
	if (node->start[0] == '"' && ft_strchr(test, '$'))
	{
		free(test);
		return (1);
	}
	else
	{
		free(test);
		return (0);
	}
}

void	without_quote(t_token **head, t_token *node, t_info *info)
{
	char	*str;

	str = return_string(node, head, info);
	replace_node(node, str);
}

void	expand_token(t_token **head, t_info *info)
{
	t_token	*parcours;

	parcours = *head;
	while (parcours)
	{
		if (parcours->type == EXPAND && is_only_dollars(parcours))
		{
			if (parcours->next == NULL)
				break ;
			parcours = parcours->next;
		}
		else if (parcours->type == EXPAND)
			expand_one_token(parcours, info, head);
		else if (parcours->type == STRING && will_expand(parcours, head, info))
			expand_string(head, parcours, info);
		else if (parcours->type == STRING)
			without_quote(head, parcours, info);
		else if (parcours->type == HEREDOC && parcours->next != NULL)
		{
			parcours = parcours->next;
			while (parcours->type == WHITESPACE)
				parcours = parcours->next;
		}
		else if (parcours->type == RETURN_COMMAND)
			expand_one_token_return_value(parcours, info, head);
		parcours = parcours->next;
	}
}
