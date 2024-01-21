/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:58:06 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/21 15:19:43 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include "ft_printf.h"
#include <unistd.h>

void	child_one(t_main *m_strct, char **envp, int n)
{
	close(m_strct->fd[0]);
	if (m_strct->fd_file[1] != -1)
		close(m_strct->fd_file[1]);
	if (dup2(m_strct->fd[1], STDOUT_FILENO) < 0
		|| dup2(m_strct->fd_file[0], STDIN_FILENO) < 0)
		handle_error(m_strct, FAIL_DUP2, n);
	close(m_strct->fd[1]);
	close(m_strct->fd_file[0]);
	m_strct->cmd[n].cmd = get_path(m_strct, m_strct->cmd[n].cmd, envp);
	free(m_strct->cmd[n].args[0]);
	m_strct->cmd[n].args[0] = ft_strdup(m_strct->cmd[n].cmd);
	if (!m_strct->cmd[n].args[0])
		handle_error(m_strct, FAIL_MALLOC, n);
	if (!ft_strchr(m_strct->cmd[n].cmd, '/')
		|| access(m_strct->cmd[n].cmd, F_OK | X_OK))
		handle_error(m_strct, FAIL_COMMAND_NOT_FOUND, n);
	if (execve(m_strct->cmd[n].cmd, m_strct->cmd[n].args, envp) == -1)
		handle_error(m_strct, FAIL_COMMAND, n);
}

void	child_two(t_main *m_strct, char **envp, int n)
{
	close(m_strct->fd[1]);
	if (m_strct->fd_file[0] != -1)
		close(m_strct->fd_file[0]);
	if (dup2(m_strct->fd_file[1], STDOUT_FILENO) < 0
		|| dup2(m_strct->fd[0], STDIN_FILENO) < 0)
		handle_error(m_strct, FAIL_DUP2, n);
	close(m_strct->fd[0]);
	close(m_strct->fd_file[1]);
	m_strct->cmd[n].cmd = get_path(m_strct, m_strct->cmd[n].cmd, envp);
	free(m_strct->cmd[n].args[0]);
	m_strct->cmd[n].args[0] = ft_strdup(m_strct->cmd[n].cmd);
	if (!m_strct->cmd[n].args[0])
		handle_error(m_strct, FAIL_MALLOC, n);
	if (!ft_strchr(m_strct->cmd[n].cmd, '/')
		|| access(m_strct->cmd[n].cmd, F_OK | X_OK))
		handle_error(m_strct, FAIL_COMMAND_NOT_FOUND, n);
	if (execve(m_strct->cmd[n].cmd, m_strct->cmd[n].args, envp) == -1)
		handle_error(m_strct, FAIL_COMMAND, n);
}
