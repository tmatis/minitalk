/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_client.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 21:21:41 by tmatis            #+#    #+#             */
/*   Updated: 2021/05/27 12:27:40 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int		g_confirm = 0;

void	sig_handler(int sig_n)
{
	(void)sig_n;
	g_confirm = 1;
}

t_bool	is_atoiable(char *str)
{
	if (*str != '-' && *str != '+' && !ft_isnum(*str))
		return (false);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str))
			return (false);
		str++;
	}
	return (true);
}

/*
** Pour acces au dernier bit j'utilise le mask b00000001
*/

void	encode(pid_t pid, char *str)
{
	char	c;
	int		bin;
	int		char_size;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str) + 1;
	while (i < len)
	{
		c = str[i];
		char_size = 0;
		while (char_size < 8)
		{
			usleep(200);
			bin = c & 0b10000000;
			c = c << 1;
			char_size++;
			if (bin)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR1);
		}
		i++;
	}
}

char	*build_string(char *str)
{
	char	*pid_string;
	char	*dest;

	pid_string = ft_itoa(getpid());
	if (!pid_string)
		return (NULL);
	dest = ft_calloc(ft_strlen(str) + ft_strlen(pid_string) + 2, sizeof(char));
	if (!dest)
	{
		free(pid_string);
		return (NULL);
	}
	ft_strcat(dest, pid_string);
	ft_strcat(dest, ";");
	ft_strcat(dest, str);
	free(pid_string);
	return (dest);
}

int	main(int argc, char **argv)
{
	char	*send_string;

	signal(SIGUSR1, sig_handler);
	if (argc == 3 && is_atoiable(argv[1]))
	{
		send_string = build_string(argv[2]);
		if (!send_string)
			return (127);
		encode(ft_atoi(argv[1]), send_string);
		free(send_string);
		ft_putstr_fd("Wait for confirmation: ", STDOUT_FILENO);
		sleep(1);
		if (g_confirm)
			ft_putstr_fd("\033[1;32m[OK]\033[0m\n", STDOUT_FILENO);
		else
			ft_putstr_fd("\033[1;31m[KO]\033[0m\n", STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd("Usage: ./client PID \"your message\"\n", STDERR_FILENO);
		return (2);
	}
	return (0);
}
