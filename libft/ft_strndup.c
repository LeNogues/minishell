/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oettaqi <oettaqi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:23:06 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/19 14:06:55 by oettaqi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, int size)
{
	char	*dup;
	int		i;

	i = 0;
	if (!s)
		return (0);
	dup = malloc(sizeof (char) * size + 1);
	if (!dup)
		return (NULL);
	while (s[i] && i < size)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}
