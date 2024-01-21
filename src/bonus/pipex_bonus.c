/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 14:23:28 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/21 15:48:55 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "libft.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include <time.h>

static char	*ft_strdup_pipex(char *s, char c)
{
	size_t	i;
	char	*dup;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && s[i] != c)
		i++;
	dup = malloc(sizeof(*dup) * (i + 1));
	if (!dup)
		return (NULL);
	ft_memmove(dup, s, i);
	dup[i] = '\0';
	return (dup);
}

static t_cmd	*parse_arg(char **av, t_main *main_struct, int ac)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = ft_calloc(sizeof(*cmd), ac);
	if (!cmd)
		handle_error(main_struct, FAIL_MALLOC, -1);
	while (i < ac - 1)
	{
		cmd[i].cmd = ft_strdup_pipex(av[i], ' ');
		if (!cmd[i].cmd)
			handle_error(main_struct, FAIL_MALLOC, -1);
		cmd[i].args = ft_split(av[i], ' ');
		if (!cmd[i].args)
			handle_error(main_struct, FAIL_MALLOC, -1);
		i++;
	}
	cmd[i].cmd = NULL;
	cmd[i].args = NULL;
	return (cmd);
}

static void	init_main(char **av, t_main *main_struct, int ac)
{
	int	n;

	n = 0;
	ft_bzero(main_struct, sizeof(*main_struct));
	if (ft_strncmp(av[n], "here_doc", ft_strlen("here_doc") + 1) == 0)
		init_here_doc(av, main_struct, ac, &n);
	else
	{
		main_struct->fd_file[n] = open(av[n], O_RDWR);
		if (main_struct->fd_file[0] == -1)
			ft_fprintf(2, "pipex: Permission denied: %s\n", av[n]);
		n++;
		main_struct->fd_file[1] = open(av[ac - 1],
				O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (main_struct->fd_file[1] == -1)
			ft_fprintf(2, "pipex: Permission denied: %s\n", av[ac - 1]);
		n++;
	}
	main_struct->nb_cmd = ac - n;
	main_struct->fd_tmp = -1;
	main_struct->cmd = parse_arg(&av[n - 1], main_struct, ac - 1);
	main_struct->pid = malloc(sizeof(pid_t) * ac - n);
	if (!main_struct->pid)
		handle_error(main_struct, FAIL_MALLOC, -1);
}

static int	pipex(t_main *main_struct, char **envp, int n)
{
	while (n < main_struct->nb_cmd - 1)
	{
		main_struct->pip = pipe(main_struct->fd);
		if (main_struct->pip < 0)
			handle_error(main_struct, FAIL_PIPE, n);
		main_struct->pid[n] = fork();
		if (main_struct->pid[n] < 0)
			handle_error(main_struct, FAIL_FORK, -1);
		if (!main_struct->pid[n])
		{
			if (n == 0)
				first_child(main_struct, envp, n);
			middle_child(main_struct, envp, n);
		}
		else
		{
			if (main_struct->fd_tmp != -1)
				close(main_struct->fd_tmp);
			close(main_struct->fd[1]);
			main_struct->fd_tmp = main_struct->fd[0];
			n++;
		}
	}
	return (n);
}

int	main(int ac, char **av, char **envp)
{
	t_main	main_struct;
	int		n;

	if (ac < 5)
		return (ft_fprintf(2, "Invalid numbers of arguments\n"), 2);
	init_main(&av[1], &main_struct, ac - 1);
	n = pipex(&main_struct, envp, 0);
	main_struct.pid[n] = fork();
	if (main_struct.pid[n] < 0)
		handle_error(&main_struct, FAIL_FORK, -1);
	if (!main_struct.pid[n])
		last_child(&main_struct, envp, n);
	close(main_struct.fd_tmp);
	end_program(&main_struct, n);
}
