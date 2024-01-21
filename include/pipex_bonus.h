/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:35:28 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/21 15:50:40 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
}				t_cmd;

typedef struct s_main
{
	t_cmd	*cmd;
	int		fd_file[2];
	pid_t	*pid;
	int		pip;
	int		fd[2];
	int		fd_tmp;
	int		*status;
	int		nb_cmd;
	int		here_doc;
}				t_main;

enum e_error
{
	FAIL_OPEN_INFILE,
	FAIL_OPEN_OUTFILE,
	FAIL_PIPE,
	FAIL_DUP2,
	FAIL_FORK,
	FAIL_MALLOC,
	FAIL_COMMAND,
	FAIL_COMMAND_NOT_FOUND,
};

void	first_child(t_main *main_struct, char **envp, int n);
void	middle_child(t_main *main_stuct, char **envp, int n);
void	last_child(t_main *main_struct, char **envp, int n);
char	*get_path(t_main *main_struct, char *cmd, char **envp);
void	init_here_doc(char **av, t_main *main_struct, int ac, int *n);
void	do_here_doc(char **av, t_main *main_struct);
char	**ft_split_pipex(const char *s, char c);
void	free_cmd(t_cmd *cmd);
void	end_program(t_main *main_struct, int n);
void	free_main_struct(t_main *main_struct);
void	handle_error(t_main *main_struct, enum e_error error, int n);

#endif
