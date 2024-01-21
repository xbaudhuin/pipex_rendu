/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 15:48:50 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/18 15:51:04 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include "pipex.h"
#include <errno.h>
#include <stdlib.h>

void	end_program(t_main *main_struct)
{
	int	status;
	int	exit_value;

	exit_value = 0;
	waitpid(main_struct->pid[1], &status, 0);
	while (wait(NULL) > 0)
	{
	}
	if (WIFEXITED(status))
		exit_value = WEXITSTATUS(status);
	free_main_struct(main_struct);
	exit(exit_value);
}

void	free_main_struct(t_main *main_struct)
{
	free_cmd(main_struct->cmd);
	free(main_struct->pid);
	if (main_struct->fd_file[0] != -1)
		close(main_struct->fd_file[0]);
	if (main_struct->fd_file[1] != -1)
		close(main_struct->fd_file[1]);
	if (main_struct->fd[0] != -1)
		close(main_struct->fd[0]);
	if (main_struct->fd[1] != -1)
		close(main_struct->fd[1]);
}

void	handle_error(t_main *main_struct, enum e_error error, int n)
{
	int	exit_value;

	exit_value = 1;
	if (error == FAIL_COMMAND_NOT_FOUND)
	{
		ft_fprintf(2, "pipex: Command %s not found\n", main_struct->cmd[n].cmd);
		exit_value = 127;
	}
	free_main_struct(main_struct);
	while (wait(NULL) > 0)
	{
	}
	exit(exit_value);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i].cmd)
	{
		free(cmd[i].cmd);
		if (cmd[i].args)
			free_split(cmd[i].args);
		i++;
	}
	free(cmd);
}
