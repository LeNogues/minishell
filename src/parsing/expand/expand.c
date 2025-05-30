/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oettaqi <oettaqi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:43:22 by othmaneetta       #+#    #+#             */
/*   Updated: 2025/05/30 16:03:37 by oettaqi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	expand_one_token_return_value(t_token *token_node, t_info *info)
{
	char	*value;

	//printf ("RETURN VALUE = %d \n", info->return_value);
	value = ft_itoa(info->return_value);
	//printf ("quand je passe a itoa ca donne %s \n", value);
	free(token_node->start);
	token_node->start = ft_strdup(value);
	token_node->length = ft_strlen(value);
	free(value);
	// int	i = 0;
	// printf(" le token est : ");
	// while (i < token_node->length)
	// {
	// 	printf("%c" ,token_node->start[i]);
	// 	i++;
	// }
	// printf("\n");
}

void	expand_one_token(t_token *token_node, t_info *info)
{
	char	*value_token;
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 1;
	value_token = malloc(token_node->length + 1);
	if (!value_token)
		return ;
	while (i < (token_node->length - 1))
		value_token[i++] = token_node->start[j++];
	value_token[i++] = '=';
	value_token[i] = 0;
	value = ft_getenv(value_token, info->env);
	free(value_token);
	if (!value)
	{
		free(token_node->start);
		token_node->start = ft_strdup("");
		token_node->length = ft_strlen(value);
		return ;
	}
	free(token_node->start);
	token_node->start = ft_strdup(value);
	token_node->length = ft_strlen(value);
	free(value);
}

int	will_expand(t_token *node)
{
	char	*test;

	test = ft_strndup(node->start, node->length);
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

void	without_quote(t_token **head, t_token *node)
{
	char	*str;

	(void)head;
	str = return_string(node);
	replace_node(node, str);
}

void	expand_token(t_token **head, t_info *info)
{
	t_token	*parcours;

	parcours = *head;
	while (parcours)
	{
		if (parcours->type == EXPAND)
			expand_one_token(parcours, info);
		else if (parcours->type == STRING && will_expand(parcours))
			expand_string(head, parcours, info);
		else if (parcours->type == STRING)
			without_quote(head, parcours);
		else if (parcours->type == HEREDOC && parcours->next != NULL)
		{
			parcours = parcours->next;
			while (parcours->type == WHITESPACE)
				parcours = parcours->next;
		}
		else if (parcours->type == RETURN_COMMAND)
			expand_one_token_return_value(parcours, info);
		parcours = parcours->next;
	}
}
