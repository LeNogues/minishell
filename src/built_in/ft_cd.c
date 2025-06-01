/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seb <seb@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:43:35 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/01 13:59:12 by seb              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

static void	free_things(t_env *env, char *path, char *tmp_path, int i)
{
	free(env->envp[i]);
	free(path);
	free(tmp_path);
}

static void	change_pwd(t_env *env, char *path)
{
	char	*new_pwd;
	int		i;
	char	*tmp_path;

	i = 0;
	tmp_path = ft_strdup(path);
	if (!env || !env->envp)
		return ;
	while (env->envp[i])
	{
		if (ft_strncmp("PWD=", env->envp[i], 4) == 0)
			break ;
		i++;
	}
	if (path[0] != '/')
		path = create_new_path(env, path);
	if (ft_strncmp(tmp_path, "..", 3) == 0)
		new_pwd = get_parent(env);
	else
		new_pwd = ft_strjoin("PWD=", path);
	if (!new_pwd)
		return (free(path), free(tmp_path));
	get_rid_slash(new_pwd);
	free_things(env, path, tmp_path, i);
	env->envp[i] = new_pwd;
}

static void	change_old_pwd(t_env *env)
{
	char	*cwd;
	char	*old_pwd;
	int		i;

	i = 0;
	if (!env || !env->envp)
		return ;
	while (env->envp[i])
	{
		if (ft_strncmp("OLDPWD=", env->envp[i], 7) == 0)
			break ;
		i++;
	}
	if (!env->envp[i])
		if (!create_env_cd("export OLDPWD=", env))
			return ;
	cwd = ft_getenv("PWD=", env);
	if (!cwd)
		return (perror("ft_getenv"));
	old_pwd = ft_strjoin("OLDPWD=", cwd);
	if (!old_pwd)
		return (perror("ft_strjoin"));
	free(env->envp[i]);
	free(cwd);
	env->envp[i] = old_pwd;
}

int	ft_cd(char **cmd, t_env *env)
{
	char		*path;
	const int	size_tab = ft_tablen(cmd);

	path = NULL;
	if (size_tab == 1)
	{
		if (!cd_home(env, &path))
			return (0);
	}
	else if (size_tab > 2)
		return (perror("too many arguments"), 0);
	else
	{
		if (path)
			free(path);
		path = ft_strdup(cmd[1]);
	}
	if (chdir(path) == -1)
		return (perror(path), free(path), 0);
	change_old_pwd(env);
	change_pwd(env, path);
	return (1);
}
