/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-nogu <sle-nogu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:03:48 by sle-nogu          #+#    #+#             */
/*   Updated: 2025/06/18 23:14:21 by sle-nogu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// declaration des macros///////////////////////////////////////////////////////
# ifndef INPUT
#  define INPUT 1

# endif

# ifndef OUTPUT_TRUNC
#  define OUTPUT_TRUNC 2

# endif

# ifndef OUTPUT_APPEND
#  define OUTPUT_APPEND 3

# endif

# ifndef ISHEREDOC
#  define ISHEREDOC 4

# endif
///////////////////////////////////////////////////////////////////////////////

# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern int			g_state_signal;

typedef struct s_cmd
{
	char			**cmd;
	char			**name;
	char			*full_path;
	int				*in_or_out;
	int				heredoc;
	int				pos;
	int				fd_in;
	int				fd_out;
	int				nb_cmd;
	int				fd_stdin;
	int				fd_stdout;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_pipe
{
	int				old[2];
	int				new[2];
	int				heredoc[2];
}					t_pipe;

typedef struct s_env
{
	char			**envp;
	int				sz;
}					t_env;

typedef struct s_info
{
	t_env	*env;
	t_pipe	*pipe;
	t_cmd	*cmd;
	t_cmd	*cmd_origin;
	int		return_value;
	int		last_pid;
}					t_info;

// built_in1.c*****************************************************************
void				hub(t_info *info);
int					choice_of_builtin(t_info *info, t_env *env,
						t_pipe *pipe_fd);
int					choice_of_builtin(t_info *info, t_env *env,
						t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// ft_pwd.c********************************************************************
int					ft_pwd(t_env *env);
///////////////////////////////////////////////////////////////////////////////

// ft_cd.c*********************************************************************
int					ft_cd(char **path, t_env *env);
///////////////////////////////////////////////////////////////////////////////

// ft_cd_utils.c***************************************************************
int					cd_home(t_env *env, char **path);
char				*get_parent(t_env *env);
///////////////////////////////////////////////////////////////////////////////

//ft_cd_utils2.c
int					create_env_cd(char *env_to_create, t_env *env);
void				get_rid_slash(char *cwd);
char				*create_new_path(t_env *env, char *path);
///////////////////////////////////////////////////////////////////////////////

// ft_echo.c*******************************************************************
int					ft_echo(t_cmd *cmd);
///////////////////////////////////////////////////////////////////////////////

// ft_exit.c*******************************************************************
int					ft_exit(t_info *info, t_env *env, t_pipe *pipe_fd);

void				exit_clean(unsigned int return_value, t_info *info,
						t_env *env, t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// ft_env.c********************************************************************
int					ft_env(t_env *env);
///////////////////////////////////////////////////////////////////////////////

// ft_unset.c******************************************************************
int					ft_unset(char **cmd, t_env *env);
///////////////////////////////////////////////////////////////////////////////

// ft_export.c*****************************************************************
int					ft_export(char **cmd, t_env *env);
///////////////////////////////////////////////////////////////////////////////

// ft_export_utils.c***********************************************************
int					is_valid(char *str);
///////////////////////////////////////////////////////////////////////////////

// main.c**********************************************************************
int					main(int argc, char **argv, char **envp);
///////////////////////////////////////////////////////////////////////////////

// ft_getenv.c*****************************************************************
char				*ft_getenv(char *value_name, t_env *env);
char				*ft_getenv_bis(char *value_name, t_env *env);
///////////////////////////////////////////////////////////////////////////////

//add_histo_and_exec.c
void				add_histo_and_exec(t_info *info, char *line);
///////////////////////////////////////////////////////////////////////////////

// signal.c********************************************************************
void				ctrl_back(int sig);
void				ctrl_back_bis(int sig);
void				handle_signal(void);
///////////////////////////////////////////////////////////////////////////////

// exec.c**********************************************************************
void				exec(t_info *info);
int					open_in(t_cmd *cmd, char *name);
int					open_out(t_cmd *cmd, char *name, int type);
///////////////////////////////////////////////////////////////////////////////

// exec_loop.c*****************************************************************
int					loop_on_middle(t_info *info, t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// parsing.c*******************************************************************
char				*verif_arg(t_info *info, t_pipe *pipe_fd, t_env *env);
///////////////////////////////////////////////////////////////////////////////

// path.c**********************************************************************
char				*get_path(t_env *env);
///////////////////////////////////////////////////////////////////////////////

// free.c**********************************************************************
void				free_all_cmd(t_cmd *cmd);
void				free_cmd(t_cmd *cmd);
void				free_cmd_env_pipe(t_info *info, t_env *env,
						t_pipe *pipe_fd);
void				free_cmd_env_pipe_bis(t_info *info, t_env *env,
						t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// verif.c*********************************************************************
int					verif_file(t_info *info, t_pipe *pipe_fd);
int					verif_file_builtin(t_info *info, t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// execute.c*******************************************************************
void				execute(t_info *info, t_env *env, t_pipe *pipe_fd);
void				execute_middle(t_cmd *cmd, char *full_path, t_env *env,
						t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// handle_cmd.c****************************************************************
void				handle_cmd(t_info *info, t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// close_pipe_fd.c*************************************************************
void				close_pipe_fd(int pipe_fd[2]);
///////////////////////////////////////////////////////////////////////////////

// dup.c************************************************************************
int					dup_first(t_cmd *cmd, t_pipe *pipe_fd);
int					dup_last(t_cmd *cmd, t_pipe *pipe_fd);
int					dup_middle(t_cmd *cmd, t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// heredoc.c********************************************************************
int					open_heredoc(t_cmd *cmd, char *limiter, t_pipe *pipe_fd,
						t_info *info);
int					open_heredoc_bis(t_cmd *cmd,
						char *limiter, t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// set_env.c********************************************************************
int					set_environment(t_env *env, char **envp);
int					create_env(t_env *env);
///////////////////////////////////////////////////////////////////////////////

// first_cmd.c******************************************************************
int					first_cmd(t_cmd **cmd_address, t_info *info,
						t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// do_cmd.c*********************************************************************
int					do_cmd(t_info *info, t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

// mini_dup.c*******************************************************************
int					dup_fd_out(t_cmd *cmd);
int					dup_fd_in(t_cmd *cmd);
int					dup_heredoc(t_pipe *pipe_fd);
///////////////////////////////////////////////////////////////////////////////

//signal.c
void				ctrl_c(int sig);
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum e_token_type
{
	COMMAND,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	EMPTY_STRING,
	EXPAND,
	STRING,
	WHITESPACE,
	NONE,
	RETURN_COMMAND,
	ERROR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*start;
	int				length;
	struct s_token	*next;
}	t_token;

typedef struct s_scanner
{
	char	*start;
	char	*current;
}	t_scanner;

// expand.c
void				expand_one_token(t_token *token_node, t_info *info);
void				expand_token(t_token **head, t_info *info);
void				expand_string(t_token **head, t_token *node, t_info *info);
void				expand_one_token_return_value(t_token *token_node,
						t_info *info);

// expand_string.c
int					size_of_merged_string(t_token **sub_linked);
char				*merge_string(t_token **head, int size);
char				*return_string_from_quote(t_token *node);
void				expand_one_token_sub(t_token **head, t_info *info);
char				*return_string(t_token *node);

// expand_utils.c
char				*ft_strchr(const char *s, int c);
void				free_token_list(t_token **head);
void				replace_node(t_token *node, char *resu);
int					is_only_dollars(t_token *node);

// main_function.c
void				fusion(t_token **head);

// fusion.c
int					is_mergeable(t_token *node);
int					size_new_string(t_token *parcours,
						t_token *end_of_sequence);
char				*create_new_string(t_token *parcours,
						t_token *end_of_sequence, int i);
void				delete_tokens(t_token *parcours, t_token *end_of_sequence);
void				merge_tokens(t_token *parcours, t_token *end_of_sequence);
// init_scanner.c

t_scanner			*scanner(void);
void				init_scanner(char *source);

// is_something.c

int					is_space(char c);
int					is_sep(char c);
int					is_allnum(char c);

// scan_scanner.c
char				peek(void);
int					is_at_end(void);
char				advance(void);
char				peek_next(void);

//make_token.c
t_token				make_token(t_token_type type);
t_token				string(char quote);
char				*skip_white(void);
t_token				expand(void);

//  lexer.c
t_token				create_single_token(char c);
t_token				scan_one_token(void);
const char			*type_to_str(t_token_type type);
void				free_all(t_token **head);
t_cmd				*merge(t_info *info, char *source);
void				ft_list_len(t_cmd *cmd);

//create_linked_list.c
void				insert_at_head(t_token **head, t_token *token);
void				insert_last(t_token **head, t_token *token_list);

//print_token.c
void				print_list(t_token **head);
void				print_one_token(t_token *node);
int					create_list_of_token(t_token **head);

// parser.c
void				parser(t_token **head, t_cmd **final);
char				**create_command_line(t_token *start, t_token *pipe);
int					size_cmd_line(t_token *start, t_token *pipe);
void				initialise_node(t_cmd **node, int cmd_size, int redir_size);

// handle.c functions
t_token				*handle_cmd_token(t_cmd *node, t_token *token, int *i);
t_token				*handle_redir_in(t_cmd *node, t_token *token, int *r);
t_token				*handle_redir_out(t_cmd *node, t_token *token, int *r);
t_token				*handle_heredoc(t_cmd *node, t_token *token, int *r);

// parser.c helper
void				init_indices(int indices[2]);

// parser_verif.c
int					handle_redir_syntax(t_token **current_ptr);
int					handle_pipe_syntax(t_token **current_ptr);
int					is_redir_type(t_token_type type);

// syntax_verif.c
int					syntax_verif(t_token **head);

// create_node.c
t_cmd				*create_one_node(t_token *start, t_token *pipe);
t_cmd				*create_one_node(t_token *start, t_token *pipe);

#endif
