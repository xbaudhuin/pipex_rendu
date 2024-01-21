/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 14:23:28 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/19 18:10:13 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"
#include "ft_printf.h"
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

static t_cmd	*parse_arg(char **av, t_main *main_struct)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = ft_calloc(sizeof(*cmd), 3);
	if (!cmd)
		handle_error(main_struct, FAIL_MALLOC, -1);
	while (i < 2)
	{
		cmd[i].cmd = ft_strdup_pipex(av[i + 2], ' ');
		if (!cmd[i].cmd)
			handle_error(main_struct, FAIL_MALLOC, -1);
		cmd[i].args = ft_split(av[i + 2], ' ');
		if (!cmd[i].args)
			handle_error(main_struct, FAIL_MALLOC, -1);
		i++;
	}
	return (cmd);
}

static void	init_main(char **av, t_main *main_struct)
{
	ft_bzero(main_struct, sizeof(*main_struct));
	main_struct->cmd = parse_arg(av, main_struct);
	main_struct->fd_file[0] = open(av[1], O_RDWR);
	if (main_struct->fd_file[0] == -1)
		ft_fprintf(2, "pipex: Permission denied: %s\n", av[1]);
	main_struct->fd_file[1] = open(av[4], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (main_struct->fd_file[1] == -1)
		ft_fprintf(2, "pipex: Permission denied: %s\n", av[4]);
	main_struct->pid = malloc(sizeof(pid_t) * 2);
	if (!main_struct->pid)
		handle_error(main_struct, FAIL_MALLOC, -1);
}

int	main(int ac, char **av, char **envp)
{
	t_main	main_struct;

	if (ac != 5)
		return (ft_printf("Invalid numbers of arguments\n"), 2);
	init_main(av, &main_struct);
	main_struct.pip = pipe(main_struct.fd);
	if (main_struct.pip < 0)
		handle_error(&main_struct, FAIL_PIPE, -1);
	main_struct.pid[0] = fork();
	if (main_struct.pid[0] < 0)
		handle_error(&main_struct, FAIL_FORK, -1);
	if (!main_struct.pid[0])
		child_one(&main_struct, envp, 0);
	main_struct.pid[1] = fork();
	if (main_struct.pid[1] < 0)
		handle_error(&main_struct, FAIL_FORK, -1);
	if (!main_struct.pid[1])
		child_two(&main_struct, envp, 1);
	close(main_struct.fd[1]);
	close(main_struct.fd[0]);
	end_program(&main_struct);
}
