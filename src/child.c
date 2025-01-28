/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:39:07 by pn                #+#    #+#             */
/*   Updated: 2025/01/28 20:11:50 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    setup_first_child(t_pipex *data, int i)
{
    if (data->is_heredoc)
    {
        if (dup2(data->pipes[i][1], STDOUT_FILENO) < 0)
            handle_error("Dup2 failed", -1, -1);
        handle_here_doc(data, data->pipes[i][1]);
        close_pipes(data->pipes, data->num_pipes);
        free_pipex(data);
        exit(EXIT_SUCCESS);
    }
    redirect_first(data, i);
}

void execute_child(t_pipex *data, int i)
{
    if (i == 0)
        setup_first_child(data, i);
    else if (i == data->argc - 4)
         redirect_last(data, i);
    else
        redirect_input_output(data->pipes[i - 1][0], data->pipes[i][1]);
    if (check_cmd(data->argv[i + 2], data->env) == -1)
    {
        close_pipes(data->pipes, data->num_pipes);
        free_pipex(data);
        exit(127);
    }
    close_pipes(data->pipes, data->num_pipes);
    if (data->is_heredoc && i == 0)
        execute_command(data, data->argv[i + 3]);
    else
        execute_command(data, data->argv[i + 2]);
}

void handle_fork(t_pipex *data, int i)
{
    data->pids[i] = fork();
    if (data->pids[i] < 0)
    {
        free_pipex(data);
        handle_error("Fork failed", -1, -1);
    }
    if (data->pids[i] == 0)
        execute_child(data, i);
}

void    handle_here_doc(t_pipex *data, int pipe_fd)
{
    char    *line;
    size_t  len;

    while (1)
    {
        fprintf(stderr, "heredoc> ");
        line = get_next_line(STDIN_FILENO);
        if (!line)
        {
            close(pipe_fd);
            free_pipex(data);
            exit(EXIT_FAILURE);
        }
        len = ft_strlen(data->argv[2]);
        if (ft_strncmp(line, data->argv[2], len) == 0 && 
            (line[len] == '\n' || line[len] == '\0'))
        {
            free(line);
            break ;
        }
        write(pipe_fd, line, ft_strlen(line));
        free(line);
    }
    close(pipe_fd);
}
