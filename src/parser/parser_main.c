#include "parser.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s xxx.rt\n", argv[0]);
        return (1);
    }
	else if (ft_strrchr(argv[1], '.') == NULL || ft_strcmp(ft_strrchr(argv[1], '.'), ".rt") != 0)
	{
		printf("Error\nConfiguration file must have a .rt extension\n");
		return (1);
	}
    if (!validate_config_file_full(argv[1]))
        return (1);
    return (0);
}
