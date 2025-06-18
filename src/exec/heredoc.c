/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:29:05 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/18 23:06:47 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

static void	put_line(char *line, t_pipe *pipe_fd)
{
	ft_putstr_fd(line, pipe_fd->heredoc[1]);
	ft_putstr_fd("\n", pipe_fd->heredoc[1]);
}

int	open_heredoc_bis(t_cmd *cmd, char *limiter, t_pipe *pipe_fd)
{
	char		*line;

	g_state_signal = 3;
	(void)cmd;
	(void)pipe_fd;
	signal(SIGQUIT, SIG_IGN);
	while (g_state_signal == 3)
	{
		line = readline("heredoc > ");
		if (!line || g_state_signal != 3)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			return (free(line), 1);
		}
		free(line);
	}
	return (-1);
}

static char	*append_char(char *s, char c)
{
	size_t	len;
	char	*new;

	if (s == NULL)
		len = 0;
	else
		len = ft_strlen(s);
	new = malloc(len + 2);
	if (new == NULL)
		return (NULL);
	if (s != NULL)
		ft_memcpy(new, s, len);
	new[len] = c;
	new[len + 1] = '\0';
	free(s);
	return (new);
}

char	*expand_for_heredoc(char *line, t_info *info)
{
	size_t	i;
	char	*out;
	char	*tmp;
	char	*name;
	char	*val;
	char	*sts;
	size_t	start;

	i = 0;
	out = ft_strdup("");
	while (line != NULL && line[i] != '\0')
	{
		if (line[i] == '$' && line[i + 1] != '\0')
		{
			if (line[i + 1] == '?')
			{
				sts = ft_itoa(info->return_value);
				if (sts == NULL)
					return (free(out), free(line), NULL);
				tmp = ft_strjoin(out, sts);
				free(out);
				free(sts);
				out = tmp;
				i += 2;
				continue ;
			}
			if (!ft_isalpha(line[i + 1]) && line[i + 1] != '_')
			{
				out = append_char(out, line[i]);
				i++;
				continue ;
			}
			i++;
			start = i;
			while (ft_isalnum(line[i]) || line[i] == '_')
				i++;
			name = ft_substr(line, start, i - start);
			val = ft_getenv(name, info->env);
			if (val == NULL)
				val = "";
			tmp = ft_strjoin(out, val);
			free(out);
			free(name);
			out = tmp;
		}
		else
		{
			out = append_char(out, line[i]);
			i++;
		}
	}
	free(line);
	return (out);
}

int	open_heredoc(t_cmd *cmd, char *limiter, t_pipe *pipe_fd, t_info *info)
{
	char		*line;
	static int	passage = 0;

	if (passage == 0)
		pipe(pipe_fd->heredoc);
	passage++;
	signal(SIGQUIT, ctrl_back_bis);
	while (g_state_signal == 3)
	{
		line = readline("heredoc > ");
		if (!line || g_state_signal != 3)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) != 0)
			line = expand_for_heredoc(line, info);
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			if (passage == cmd->heredoc)
				dup_heredoc(pipe_fd);
			return (free(line), 1);
		}
		if (passage == cmd->heredoc)
			put_line(line, pipe_fd);
		free(line);
	}
	return (close_pipe_fd(pipe_fd->heredoc), -1);
}
