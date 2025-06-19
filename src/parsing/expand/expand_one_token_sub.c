/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_one_token_sub.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oettaqi <oettaqi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:15:39 by oettaqi           #+#    #+#             */
/*   Updated: 2025/06/19 16:28:46 by oettaqi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	expand_one_token_for_sub(t_token *token_node, t_info *info)
{
	char	*value_token;
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(token_node->length + 1);
	if (!value_token)
		return (0);
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
		return (3);
	}
	token_node->start = ft_strdup(value);
	free(value);
	return (1);
}

int	expand_one_token_return_value_for_sub(t_token *token_node, t_info *info)
{
	char	*value;

	value = ft_itoa(info->return_value);
	if (!value)
		return (0);
	free(token_node->start);
	token_node->start = ft_strdup(value);
	token_node->length = ft_strlen(value);
	free(value);
	return (1);
}

char	*return_string_from_quote_for_sub(t_token *node)
{
	char	*value_token;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(sizeof(char) * (node->length));
	if (!value_token)
		return (0);
	while (i < (node->length - 2))
	{
		value_token[i] = node->start[j];
		i++;
		j++;
	}
	value_token[i] = 0;
	if (!ft_strchr(value_token, '$'))
	{
		free(value_token);
		return (NULL);
	}
	return (value_token);
}

int	expand_one_token_spec(t_token *token_node, t_info *info)
{
	char	*value_token;
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(token_node->length + 1);
	if (!value_token)
        return (0);
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
		return (3);
	}
	token_node->start = ft_strdup(value);
	free(value);
	return (1);
}

int	expand_one_token_sub(t_token **head, t_info *info)
{
	t_token	*parcours;
	char	*str;

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
		{
			if (expand_one_token_for_sub(parcours, info) == 0)
				return(0) ;
		}
		else if (parcours->type == RETURN_COMMAND)
		{
			if (expand_one_token_return_value_for_sub(parcours, info) == 0)
				return (0);
		}
		else if (parcours->type == STRING && parcours->start[0] == 39)
		{
			str = return_string_from_quote_for_sub(parcours);
			if (!str)
				return (0);
			replace_node(parcours, str);
			if (!expand_one_token_spec(parcours, info))
				return (0);
		}
		parcours = parcours->next;
	}
	return (1);
}
