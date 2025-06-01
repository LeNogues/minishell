/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seb <seb@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:12:07 by seb               #+#    #+#             */
/*   Updated: 2025/06/01 19:08:46 by seb              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	is_valid(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (!strchr(str, '='))
		return (0);
	while (str[i])
	{
		if (str[i] != '_' && !ft_isalnum(str[i]) && str[i] != '=' && str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}
