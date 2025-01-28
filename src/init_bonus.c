/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:06:38 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/28 15:46:07 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **env)
{
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 ... cmdN file2\n");
		return (EXIT_FAILURE);
	}
	launch_pipeline(argc, argv, env);
	return (EXIT_SUCCESS);
}
