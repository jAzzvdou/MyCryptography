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

char	**splitn(char *s, int sep)
{
	if (!s || sep < 1)
		return (NULL);

	int len = my_strlen(s);
	int num_chunks = (len + sep - 1) / sep;
	char **result = malloc(sizeof(char *) * (num_chunks + 1));
	if (!result)
		return (NULL);

	int i = 0;
	while (i < num_chunks)
	{
		result[i] = substr(s, i * sep, sep);
		if (!result[i])
			return (free_matrix(result), NULL);
		i++;
	}
	result[num_chunks] = NULL;
	return (result);
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

char    *joiner(char *s1, char *s2)
{
        char *tmp = strjoin(s1, s2);
        free(s1);
        return (tmp);
}

char    *readfile(int fd)
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

int     endswith(char *s, char *end)
{
        int i = 0;
        int j = 0;
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

int     main(int ac, char **av)
{
        if (ac != 2)
                return (2);
        if (!endswith(av[1], ".txt"))
                return (2);

        int from = open("input.txt", O_RDONLY);
        int key = open(av[1], O_RDONLY);
        if (from < 0 || key < 0)
                return (2);

        char *tmp = readfile(from);
        close(from);
        char **alphabet_key = split(readfile(key), '\n');
        close(key);
        if (!tmp || !alphabet_key)
                return (2);
	char **readed = splitn(tmp, 10);
	free(tmp);
	if (!readed)
		return (2);

        int out = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
        if (out < 0)
                return (2);
	
	int i = 0;
	while (readed[i])
	{
		int j = 0;
		while (alphabet_key[j])
		{
			if (!strcmp(readed[i], "0000000000"))
			{
				write(out, "\n", 1);
				break ;
			}
			else if (!strcmp(readed[i], alphabet_key[j]))
			{
				write(out, &alphabet[j], 1);
				break ;
			}
			j++;
		}
		i++;
	}
	close(out);
	free_matrix(readed);
	free_matrix(alphabet_key);
	return (0);
}
