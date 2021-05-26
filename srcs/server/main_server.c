/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 21:20:44 by tmatis            #+#    #+#             */
/*   Updated: 2021/05/26 23:30:41 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

void	add_string(int buff_size, char buff[10], char **string)
{
	int		len;
	char	*dest;

	len = buff_size + 1;
	if (*string)
		len += ft_strlen(*string);
	dest = ft_calloc(len, sizeof(char));
	if (!dest)
	{
		return ;
		ft_log_error(strerror(errno));
	}
	if (*string)
		ft_strcat(dest, *string);
	ft_memcpy(dest + ft_strlen(dest), buff, buff_size);
	ft_safe_free(*string);
	*string = dest;
}

t_bool	buffer_manager(char c, char **string)
{
	static char buff[10];
	static int	buff_size = 0;

	buff[buff_size++] = c;
	if (c == '\0' || &buff[buff_size] == &buff[sizeof(buff)])
	{
		add_string(buff_size, buff, string);
		buff_size = 0;
	}
	if (c == '\0')
		return (true);
	else
		return (false);
}

void	handle_string(char *str)
{
	pid_t	pid;

	pid = ft_atoi(str);
	while (ft_isnum(*str))
		str++;
	str++;
	ft_putnl(str);
	kill(pid, SIGUSR1);
}

/*
** On decode char par char.
** SIGUSR1 = 0;
** SIGUSR2 = 1;
*/

void	decoder(int sig_no)
{
	static char	char_build = 0;
	static int	char_size = 0;
	static char *string = NULL;

	char_build = char_build << 1;
	if (sig_no == SIGUSR2)
		char_build = char_build | 0b00000001;
	char_size++;
	if (char_size == 8)
	{
		char_size = 0;
		if (buffer_manager(char_build, &string) && string)
		{
			handle_string(string);
			free(string);
			string = NULL;
		}
	}
}

int	main(void)
{
	ft_putstr_fd("My pid is: ", STDOUT_FILENO);
	ft_putnbr_fd(getpid(), STDOUT_FILENO);
	ft_putnl("");
	signal(SIGUSR1, decoder);
	signal(SIGUSR2, decoder);
	while (1)
		pause();
}