#include <stdlib.h> // atol, malloc, free
#include <string.h> // strdup
#include <unistd.h> // read, close
#include <fcntl.h>  // open

#define BUFFER 4096
#define LAST_FILE_LINE 90230

char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789        .,:;!?(){}[]<>+-*/=@#$%^&_|~´\"'\\áéíóúÁÉÍÓÚñÑàèìòùÀÈÌÒÙâêîôûÂÊÎÔÛäëïöüÄËÏÖÜãõÃÕçÇ";

size_t	my_strlen(const char *s)
{
	if (!s)
		return (0);
	size_t i = 0;
	while (s[i])
		i++;
	return (i);
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
        s1 = NULL;
        return (tmp);
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

char	**matrixnjoin(char **s1, char **s2, int len)
{
	char **matrix = malloc(sizeof(char *) * (LAST_FILE_LINE + len + 1));
	if (!matrix)
		return (NULL);
	int i = 0;
	while (s1[i])
	{
		matrix[i] = strdup(s1[i]);
		if (!matrix[i])
			return (free_matrix(matrix), NULL);
		i++;
	}
	int j = 0;
	while (j < len && s2[j])
	{
		matrix[i] = strdup(s2[j]);
		if (!matrix[i])
			return (free_matrix(matrix), NULL);
		i++;
		j++;
	}
	matrix[i] = NULL;
	return (matrix);
}

char    **read_cryptography(void)
{
        int fd = open("encrypted.txt", O_RDONLY);
        if (fd < 0)
                return (NULL);

        char *box = NULL;
        char *buffer = malloc(sizeof(char) * (BUFFER + 1));
        if (!buffer)
                return (NULL);
        int bytes = 1;
        while (bytes > 0)
        {
                bytes = read(fd, buffer, BUFFER);
                if (bytes < 0)
                        return (free(buffer), buffer = NULL, NULL);
                buffer[bytes] = '\0';
                box = joiner(box, buffer);
        }
        free(buffer);
        buffer = NULL;
        close(fd);

        char **tmp = split(box, '\n');
        free(box);
        box = NULL;
	if (!tmp)
		return (NULL);
	char **encrypted = matrixnjoin(tmp, tmp, my_strlen(alphabet));
	free_matrix(tmp);
        if (!encrypted)
                return (NULL);
        return (encrypted);
}

char    **alphabet_from_key(char **file, int key)
{
        int i = 0;
        while (file[i])
        {
                if (i == key)
                        break ;
                i++;
        }
        int alphabet_len = my_strlen(alphabet);
        char **alphabet_key = malloc(sizeof(char *) * (alphabet_len + 1));
        if (!alphabet_key)
                return (NULL);
        int j = 0;
        while (j < alphabet_len)
        {
                alphabet_key[j] = strdup(file[i]);
		if (!alphabet_key[j])
			return (free_matrix(alphabet_key), NULL);
                i++;
                j++;
        }
	alphabet_key[j] = NULL;
	return (alphabet_key);
}

void    writer(int fd, char *s)
{
        write(fd, s, my_strlen(s));
}

int     main(int ac, char **av)
{
        if (ac != 2)
                return (2);

        char **encrypted_file = read_cryptography();
        if (!encrypted_file)
                return (2);
        long key = atol(av[1]);
        if (key < 0)
                key = -key;
        while (key > LAST_FILE_LINE)
                key -= LAST_FILE_LINE;

        char **encrypted_alphabet = alphabet_from_key(encrypted_file, key);
        free_matrix(encrypted_file);
        if (!encrypted_alphabet)
                return (2);

	char *tmp = strjoin("key", av[1]);
	if (!tmp)
		return (2);
	char *filename = strjoin(tmp, ".txt");
	free(tmp);
	tmp = NULL;
	if (!filename)
		return (2);
	int fd = open(filename, O_CREAT | O_RDWR, 0644);
	free(filename);
	filename = NULL;
	if (fd < 0)
		return (2);

	int i = 0;
	while (encrypted_alphabet[i])
	{
		writer(fd, encrypted_alphabet[i]);
		writer(fd, "\n");
		i++;
	}
        free_matrix(encrypted_alphabet);
	close(fd);
        return (0);
}
