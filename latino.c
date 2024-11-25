#include <unistd.h> // read, write, close
#include <fcntl.h>  // open
#include <stdlib.h> // malloc, free
#include <string.h> // strdup

#define BUFFER 4096

char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789        .,:;!?(){}[]<>+-*/=@#$%^&_|~´\"'\\áéíóúÁÉÍÓÚñÑàèìòùÀÈÌÒÙâêîôûÂÊÎÔÛäëïöüÄËÏÖÜãõÃÕçÇ";

size_t  my_strlen(const char *s)
{
        if (!s)
                return (0);
        size_t i = 0;
        while (s[i])
                i++;
        return (i);
}

char    *substr(char const *s, unsigned int start, size_t len)
{
        size_t  i;
        size_t  s_size;
        char    *new_s;

        s_size = my_strlen(s);
        if ((size_t)start > s_size)
                return (strdup(""));
        s += start;
        s_size -= start;
        if (s_size > len)
                s_size = len;
        new_s = malloc(sizeof(char) * (s_size + 1));
        if (!new_s)
                return (NULL);
        i = -1;
        while (++i < s_size)
                new_s[i] = s[i];
        new_s[i] = '\0';
        return (new_s);
}

static size_t   count_words(char const *s, char c)
{
        size_t  length;

        length = 0;
        while (*s)
        {
                if (*s != c)
                        length++;
                while (*s && *s != c)
                        s++;
                while (*s && *s == c)
                        s++;
        }
        return (length);
}

static void     free_matrix(char **matrix)
{
        size_t  i;

        i = 0;
        while (matrix[i])
        {
                free(matrix[i]);
                matrix[i] = NULL;
                i++;
        }
        free(matrix);
}

char    **split(char const *s, char c)
{
        size_t  i;
        size_t  words;
        size_t  wordsize;
        char    **final;

        if (!s)
                return (NULL);
        words = count_words(s, c);
        final = malloc(sizeof(char *) * (words + 1));
        if (!final)
                return (NULL);
        i = 0;
        while (i < words)
        {
                while (*s == c)
                        s++;
                wordsize = 0;
                while (s[wordsize] && s[wordsize] != c)
                        wordsize++;
                final[i] = substr(s, 0, wordsize);
                if (!final[i])
                        return (free_matrix(final), NULL);
                s += wordsize;
                i++;
        }
        return (final[i] = NULL, final);
}

char    *strjoin(char *s1, char *s2)
{
        size_t  i;
        char    *new_s;

        if (!s1 && !s2)
                return (NULL);
        new_s = malloc(sizeof(char) * (my_strlen(s1) + my_strlen(s2) + 1));
        if (!new_s)
                return (NULL);
        i = 0;
        while (s1 && *s1)
                new_s[i++] = *s1++;
        while (s2 && *s2)
                new_s[i++] = *s2++;
        new_s[i] = '\0';
        return (new_s);
}

int	endswith(char *s, char *end)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
		i++;
	while (end[j])
		j++;
	while (j >= 0)
	{
		if (s[i] != end[j])
			return (0);
		i--;
		j--;
	}
	return (1);
}

char	*joiner(char *s1, char *s2)
{
	char *tmp = strjoin(s1, s2);
	free(s1);
	return (tmp);
}

char	*readfile(int fd)
{
	char *box = NULL;
	char *buffer = malloc(sizeof(char) * (BUFFER + 1));
	if (!buffer)
		return (NULL);
	int bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER);
		if (bytes < 0)
		{
			free(buffer);
			box = NULL;
			return (NULL);
		}
		buffer[bytes] = '\0';
		box = joiner(box, buffer);
	}
	free(buffer);
	return (box);
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (2);
	if (!endswith(av[1], ".txt"))
		return (2);
	
	int from = open("input.txt", O_RDONLY);
	int key = open(av[1], O_RDONLY);
	if (from < 0 || key < 0)
		return (2);
	
	char *readed = readfile(from);
	close(from);
	char **alphabet_key = split(readfile(key), '\n');
	close(key);
	if (!readed || !alphabet_key)
		return (2);
	
	int out = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (out < 0)
		return (2);

	int i = 0;
	while (readed[i])
	{
		if (readed[i] == '\n')
			write(out, "0000000000", 10);
		else
		{
			int j = 0;
			while (alphabet[j])
			{
				if (readed[i] == alphabet[j])
				{
					write(out, alphabet_key[j], my_strlen(alphabet_key[j]));
					break;
				}
				j++;
			}
		}
		i++;
	}
	free(readed);
	readed = NULL;
	free_matrix(alphabet_key);
	close(out);
	return (0);
}
