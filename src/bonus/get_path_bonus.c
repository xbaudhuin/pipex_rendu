/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 16:00:26 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/21 15:20:29 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include "ft_printf.h"

char	*find_path(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5))
		i++;
	if (envp[i])
		return (envp[i] + 5);
	return (NULL);
}

static void	error_path(t_main *main_struct, char **paths)
{
	if (paths)
		free_split(paths);
	handle_error(main_struct, FAIL_MALLOC, -1);
}

static char	*get_path_from_env(t_main *main_struct, char *cmd, char **envp)
{
	char	*tmp;
	char	**paths;
	int		i;

	tmp = find_path(envp);
	if (!tmp)
		return (cmd);
	paths = ft_split_pipex(tmp, ':');
	if (!paths)
		handle_error(main_struct, FAIL_MALLOC, -1);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], cmd);
		if (!tmp)
			error_path(main_struct, paths);
		if (!access(tmp, F_OK | X_OK))
			return (free(cmd), free_split(paths), tmp);
		free(tmp);
		i++;
	}
	free_split(paths);
	return (cmd);
}

char	*get_path(t_main *main_struct, char *cmd, char **envp)
{
	char	*tmp;

	if (!cmd || !cmd[0])
		return (cmd);
	if (cmd[0] == '/')
		return (cmd);
	if (ft_strncmp(cmd, "./", 2) == 0)
		return (cmd);
	if (ft_strncmp(cmd, "../", 3) == 0)
		return (cmd);
	if (ft_strchr(cmd, '/'))
	{
		tmp = ft_strjoin("./", cmd);
		if (!tmp)
			handle_error(main_struct, FAIL_MALLOC, -1);
		free(cmd);
		return (tmp);
	}
	cmd = get_path_from_env(main_struct, cmd, envp);
	return (cmd);
}
