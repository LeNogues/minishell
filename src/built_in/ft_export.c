/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:12:07 by seb               #+#    #+#             */
/*   Updated: 2025/05/25 14:29:33 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

static int	env_exist(t_env *env, char *cmd)
{
	int	i;

	if (!env->envp || !env->envp[0] || !cmd)
		return (0);
	i = 0;
	while (env->envp[i])
	{
		if (ft_strncmp(env->envp[i], cmd, ft_strlen(cmd) + 1) == -61)
		{
			env->envp[i] = ft_strdup(cmd);
			if (!env->envp[i])
				return (0);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	add_arguments(int i, int size, char **cmd, t_env *new_env)
{
	int	j;

	j = 1;
	while (i < size - 1 && cmd[j])
	{
		if (is_valid(cmd[j]))
		{
			if (!env_exist(new_env, cmd[j]))
			{
				new_env->envp[i] = ft_strdup(cmd[j]);
				if (!new_env->envp[i])
				{
					free_tab(new_env->envp);
					free(new_env);
					return ;
				}
			}	
				i++;
		}
		j++;
	}
	new_env->envp[i] = NULL;
}

int	ft_export(char **cmd, t_env *env)
{
	int		size;
	int		i;
	t_env	*new_env;

	if (!env || !env->envp)
		return (0);
	i = 0;
	size = ft_tablen(env->envp) + ft_tablen(cmd) + 1;
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (0);
	new_env->envp = ft_calloc(sizeof(char *), size);
	if (!new_env->envp)
		return (0);
	while (env->envp[i] != 0)
	{
		new_env->envp[i] = ft_strdup(env->envp[i]);
		if (!new_env->envp[i])
			return (free_tab(new_env->envp), free(new_env), 0);
		i++;
	}
	new_env->envp[i] = NULL;
	add_arguments(i, size, cmd, new_env);
	if (!new_env)
		return (free_tab(env->envp), 0);
	free_tab(env->envp);
	set_environment(env, new_env->envp);
	free_tab(new_env->envp);
	free(new_env);
	return (0);
}
