/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oettaqi <oettaqi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:15:35 by taqi              #+#    #+#             */
/*   Updated: 2025/05/30 17:22:30 by oettaqi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

char	*merge_string(t_token **head, int size)
{
	char	*resu;
	t_token	*parcours;
	int		i;
	int		j;

	resu = malloc(sizeof(char) * (size + 1));
	i = 0;
	parcours = *head;
	while (parcours)
	{
		j = 0;
		while (j < parcours->length)
		{
			resu[i] = parcours->start[j];
			i++;
			j++;
		}
		parcours = parcours->next;
	}
	resu[i] = 0;
	return (resu);
}

char	*return_string_from_quote(t_token *node)
{
	char	*value_token;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(sizeof(char) * (node->length));
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

char	*return_string(t_token *node)
{
	char	*value_token;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(sizeof(char) * (node->length));
	while (i < (node->length - 2))
	{
		value_token[i] = node->start[j];
		i++;
		j++;
	}
	value_token[i] = 0;
	return (value_token);
}

char	*str_for_single_quote_in_double_quote(t_token *node)
{
	char	*resu;
	char	*str;
	int		len;

	len = node->length;
	str = ft_strndup(node->start, node->length);
	resu = malloc(sizeof(char) * (len + 3));
	if (resu == NULL)
		return (NULL);
	resu[0] = '\'';
	strcpy(resu + 1, str);
	resu[len + 1] = '\'';
	resu[len + 2] = '\0';
	return (resu);
}

void	expand_one_token_sub(t_token **head, t_info *info)
{
	t_token	*parcours;
	char	*str;
	char	*final_str;

	parcours = *head;
	while (parcours)
	{
		if (parcours->type == EXPAND)
			expand_one_token(parcours, info);
		else if (parcours->type == RETURN_COMMAND)
			expand_one_token_return_value (parcours, info);
		else if (parcours->type == STRING && parcours->start[0] == 39)
		{
			str = return_string_from_quote(parcours);
			replace_node(parcours, str);
			expand_one_token(parcours, info);
			final_str = str_for_single_quote_in_double_quote(parcours);
			replace_node(parcours, final_str);
		}
		parcours = parcours->next;
	}
}

void	expand_string(t_token **head, t_token *node, t_info *info)
{
	char	*str;
	char	*resu;
	t_token	*sub_linked_list;
	int		size;

	(void)head;
	sub_linked_list = NULL;
	str = return_string_from_quote(node);
	init_scanner(str);
	create_list_of_token(&sub_linked_list);
	free(str);
	expand_one_token_sub(&sub_linked_list, info);
	size = size_of_merged_string(&sub_linked_list);
	resu = merge_string(&sub_linked_list, size);
	replace_node(node, resu);
	free_token_list(&sub_linked_list);
}
