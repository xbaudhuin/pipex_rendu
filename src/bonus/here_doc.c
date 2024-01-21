/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:11:08 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/19 18:16:07 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_here_doc(char **av, t_main *main_struct, int ac, int *n)
{
	if (ac < 5)
	{
		ft_fprintf(2, "Invalid numbers of arguments\n");
		exit(2);
	}
	do_here_doc(av, main_struct);
	main_struct->fd_file[1] = open(av[ac - 1],
			O_RDWR | O_CREAT | O_APPEND, 0666);
	if (main_struct->fd_file[1] == -1)
		ft_fprintf(2, "pipex: Permission denied: %s\n", av[ac - 1]);
	*n = 3;
}

static void	get_stdin(t_main *main_struct, char *limiter, char *av)
{
	int		i;
	char	*line;
	size_t	len;

	len = ft_strlen(limiter);
	i = 0;
	while (++i)
	{
		ft_printf("here_doc> ");
		line = get_next_line(0);
		if (!line)
		{
			ft_fprintf(2, "pipex: warning: here_document at line %d "
				"delimited by end-of-file (wanted %s)\n", i, av);
			break ;
		}
		if (ft_strncmp(limiter, line, len + 1) == 0)
			break ;
		ft_putstr_fd(line, main_struct->fd_file[0]);
		free(line);
	}
	free(line);
}

void	do_here_doc(char **av, t_main *main_struct)
{
	char	*limiter;

	limiter = ft_strjoin(av[1], "\n");
	if (!limiter)
		handle_error(main_struct, FAIL_MALLOC, -1);
	main_struct->fd_file[0] = open("here_do_6743918520.tmp",
			O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (main_struct->fd_file[0] == -1)
	{
		ft_fprintf(2, "pipex: Here_doc failed to open\n");
		return ;
	}
	main_struct->here_doc = 1;
	get_stdin(main_struct, limiter, av[1]);
	free(limiter);
	close(main_struct->fd_file[0]);
	main_struct->fd_file[0] = open("here_do_6743918520.tmp", O_RDONLY, 0666);
	if (main_struct->fd_file[0] == -1)
		ft_fprintf(2, "pipex: Here_doc failed to open\n");
}
