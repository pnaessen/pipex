/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:11:51 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/14 11:56:42 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv,char **env)
{
	int		pipe_fd[2];
	// int		read_file;
	// int		write_file;
	pid_t	pid1;

	if (argc != 5)
	{
		ft_printf("file1 cmd1 cmd2 file2\n");
		return (1);
	}
	if (pipe(pipe_fd) < 1)
	{
		perror("pipe failed");
		return (1);
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
		return(1);
	}
	ft_child(argv, pid1, env);
// 	read_file = open(argv[1], O_RDONLY);
// 	if (read_file < 1)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
// 	write_file = open(argv[5], O_WRONLY | O_CREAT);	
// 	if (write_file < 1)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
}
