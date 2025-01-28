/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:43:48 by pn                #+#    #+#             */
/*   Updated: 2025/01/28 15:46:14 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_pipes(int **pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void parent_wait(t_pipex *data)
{
    int i;
    int status;
    int last_status;

    i = 0;
    while (i < data->num_pids)
    {
        if (i == data->num_pids - 1)
            waitpid(data->pids[i], &last_status, 0);
        else
            waitpid(data->pids[i], &status, 0);
        i++;
    }
    close_pipes(data->pipes, data->num_pipes);
    free_pipex(data);
    if (WIFEXITED(last_status))
        exit(WEXITSTATUS(last_status));
    else if (WIFSIGNALED(last_status))
        exit(128 + WTERMSIG(last_status));
    exit(EXIT_FAILURE);
}

void	close_pipes(int **pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	handle_error(const char *str, int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	perror(str);
	exit(EXIT_FAILURE);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*t1;
	unsigned char	*t2;

	t1 = (unsigned char *)s1;
	t2 = (unsigned char *)s2;
	i = 0;
	while (t1[i] != '\0')
	{
		if (t1[i] != t2[i])
			return (t1[i] - t2[i]);
		if (t1[i + 1] == '\0' && t2[i + 1] == '\0')
			return (0);
		i++;
	}
	return (1);
}
