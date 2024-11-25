#include <unistd.h> // read, write, close
#include <fcntl.h>  // open
#include <string.h> // strlen, strdup
#include <stdlib.h> // malloc, free, atol

#define TOTAL_COMB 3628800
#define ERR_FD "Failed to create file\n"

void	writer(int fd, char *s)
{
	write(fd, s, strlen(s));
}

void	swap(char *a, char *b)
{
	char tmp = *a;
	*a = *b;
	*b = tmp;
}

void	permute(char **comb, char *s, int *i, int left, int right)
{
	if (left == right)
	{
		comb[*i] = strdup(s);
		(*i)++;
	}
	else
	{
		int j = left;
		while (j <= right)
		{
			swap(&s[left], &s[j]);
			permute(comb, s, i, left + 1, right);
			swap(&s[left], &s[j]);
			j++;
		}
	}
}

char	**make_all_comb(void)
{
	char **comb = malloc(sizeof(char *) * (TOTAL_COMB + 1));
	if (!comb)
		return NULL;

	int i = 0;
	char *s = strdup("0123456789");
	if (!s)
		return (free(comb), comb = NULL, NULL);
	permute(comb, s, &i, 0, 9);
	comb[i] = NULL;
	free(s);
	s = NULL;
	return (comb);
}

int	is_prime(int nb)
{
	if (nb < 2)
		return (0);
	if (nb == 2)
		return (1);
	if (nb % 2 == 0)
		return (0);
	int i = 3;
	while (i * i <= nb)
	{
		if (nb % i == 0)
			return (0);
		i += 2;
	}
	return (1);
}

int	is_even(char c)
{
	if (c == '0' || c == '2' || c == '4' || c == '6' || c == '8')
		return (1);
	return (0);
}

char	last_digit(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	char last = s[i - 1];
	return (last);
}

void	fill_file(int fd)
{
	char **all_comb = make_all_comb();
	if (!all_comb)
		return ;
	int i = 0;
	while (all_comb[i])
	{
		if (is_even(last_digit(all_comb[i])))
		{
			free(all_comb[i]);
			all_comb[i] = NULL;
			i++;
			continue ;
		}
		long n = atol(all_comb[i]);
		if (is_prime(n))
		{
			writer(fd, all_comb[i]);
			writer(fd, "\n");
		}
		free(all_comb[i]);
		all_comb[i] = NULL;
		i++;
	}
	free(all_comb);
	all_comb = NULL;
	close(fd);
}

int	create_file(void)
{
	int fd = open("encrypted.txt", O_CREAT | O_RDWR, 0644);
	if (fd < 0)
		return (writer(2, ERR_FD), 0);
	fill_file(fd);
	return (1);
}

int	main(void)
{
	if (!create_file())
		return (1);
	return (0);
}
