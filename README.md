# minitalk
The purpose of this project is to code a small data exchange program using
UNIX signals.

#encode
```c
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
		while (char_size < 8) // on fais l'operation sur chaque bit du char (8-bit)
		{
			usleep(200);
			bin = c & 0b10000000; // on met le premier bit dans bin
			c = c << 1; // on decale les bit de c de 1 vers la droite 01010000 -> 1010000
			char_size++;
			if (bin)
				kill(pid, SIGUSR2); // si bit fort (1)
			else
				kill(pid, SIGUSR1); // si bit faible (0)
		}
		i++;
	}
}
```

#decode
```c
static char	char_build = 0;
	static int	char_size = 0;
	static char	*string = NULL;

	char_build = char_build << 1;
	if (sig_no == SIGUSR2)
		char_build = char_build | 0b00000001; // si bit fort on applique le masque donc on set le dernier bit a 1
	char_size++;
	if (char_size == 8)
	{
		char_size = 0;
		//char_build is the char
	}
```
