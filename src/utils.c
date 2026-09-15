/* ************************************************************************** */
/*                                                                            */
/*   utils.c - small helpers: error printing, formatting, hex dump.          */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

/*
** Print "ft_malcolm: <fmt>: (<arg>).\n" to stderr and always return 1,
** so callers can do `return (error_msg(...));` to bubble a failure up.
** If arg is NULL we print the message alone.
*/
int	error_msg(const char *fmt, const char *arg)
{
	if (arg)
		fprintf(stderr, "%s: %s: (%s).\n", PROG_NAME, fmt, arg);
	else
		fprintf(stderr, "%s: %s\n", PROG_NAME, fmt);
	return (1);
}

/* Format 6 raw bytes into "aa:bb:cc:dd:ee:ff". dst must hold >= 18 bytes. */
void	mac_to_string(const uint8_t *mac, char *dst)
{
	sprintf(dst, "%02x:%02x:%02x:%02x:%02x:%02x",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/* Bonus: verbose hex dump of a raw buffer (offset + hex + ASCII). */
void	print_hex_dump(const char *title, const uint8_t *data, size_t len)
{
	size_t	i;
	size_t	j;

	printf("%s (%zu bytes):\n", title, len);
	i = 0;
	while (i < len)
	{
		printf("  %04zx  ", i);
		j = 0;
		while (j < 16)
		{
			if (i + j < len)
				printf("%02x ", data[i + j]);
			else
				printf("   ");
			j++;
		}
		printf(" ");
		j = 0;
		while (j < 16 && i + j < len)
		{
			if (isprint(data[i + j]))
				printf("%c", data[i + j]);
			else
				printf(".");
			j++;
		}
		printf("\n");
		i += 16;
	}
}
