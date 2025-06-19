/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oettaqi <oettaqi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:15:35 by taqi              #+#    #+#             */
/*   Updated: 2025/06/19 16:12:07 by oettaqi          ###   ########.fr       */
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
	if (!resu)
		return (0);
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

char	*return_string_from_quote(t_token *node, t_token **head, t_info *info)
{
	char	*value_token;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(sizeof(char) * (node->length));
	if (!value_token)
		exit_and_free_clean(head, info);
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

char	*return_string(t_token *node, t_token **head, t_info *info)
{
	char	*value_token;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(sizeof(char) * (node->length));
	if (!value_token)
		exit_and_free_clean(head, info);
	while (i < (node->length - 2))
	{
		value_token[i] = node->start[j];
		i++;
		j++;
	}
	value_token[i] = 0;
	return (value_token);
}

void	expand_string(t_token **head, t_token *node, t_info *info)
{
	char	*str;
	char	*resu;
	t_token	*sub_linked_list;
	int		size;

	(void)head;
	sub_linked_list = NULL;
	str = return_string_from_quote(node, head, info);
	init_scanner(str);
	if (!create_list_of_token(&sub_linked_list))
		exit_and_free_clean(head, info);
	free(str);
	if (!expand_one_token_sub(&sub_linked_list, info))
	{
		free_token_list(&sub_linked_list);
		exit_and_free_clean(head, info);
	}
	size = size_of_merged_string(&sub_linked_list);
	resu = merge_string(&sub_linked_list, size);
	if (!resu)
	{
		free_token_list(&sub_linked_list);
		exit_and_free_clean(head, info);
	}
	replace_node(node, resu);
	free_token_list(&sub_linked_list);
}
