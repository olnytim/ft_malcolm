/* ************************************************************************** */
/*                                                                            */
/*   args.c - argument validation: IPv4 (dotted, decimal, hostname) and MAC. */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

/*
** Bonus: accept an IPv4 written as a single 32-bit decimal integer,
** e.g. "168561174" == "10.12.10.22". We parse the digits by hand (ft_atoi
** cannot hold values above INT_MAX) into an unsigned long, reject anything
** that is not all-digits or overflows 32 bits, then store it in network
** byte order with htonl(). Returns 0 on success, -1 otherwise.
*/
static int	parse_decimal_ip(const char *str, struct in_addr *out)
{
	unsigned long	value;
	const char		*p;
	uint8_t			*dst;

	p = str;
	if (*p == '\0')
		return (-1);
	value = 0;
	while (*p)
	{
		if (*p < '0' || *p > '9')
			return (-1);
		value = value * 10 + (unsigned long)(*p - '0');
		if (value > 0xFFFFFFFFUL)
			return (-1);
		p++;
	}
	/* Write the 4 bytes in network (big-endian) order by hand, so we do   */
	/* not depend on htonl and the result is correct on any host endianness.*/
	dst = (uint8_t *)&out->s_addr;
	dst[0] = (uint8_t)((value >> 24) & 0xFF);
	dst[1] = (uint8_t)((value >> 16) & 0xFF);
	dst[2] = (uint8_t)((value >> 8) & 0xFF);
	dst[3] = (uint8_t)(value & 0xFF);
	return (0);
}

/*
** Try to interpret `str` as an IPv4 address, storing the 4 network-order
** bytes into `out`. Order of attempts:
**   1) inet_pton      - standard dotted decimal "10.12.10.22".
**   2) decimal (bonus)- a single 32-bit integer "168561174".
**   3) getaddrinfo    - hostname resolution (bonus), first IPv4 answer.
** On total failure we print the subject's exact error and return 1.
*/
int	parse_ip(const char *str, struct in_addr *out, const char *label)
{
	struct addrinfo		hints;
	struct addrinfo		*res;
	struct sockaddr_in	*sin;

	if (inet_pton(AF_INET, str, out) == 1)
		return (0);
	if (parse_decimal_ip(str, out) == 0)
		return (0);
	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	if (getaddrinfo(str, NULL, &hints, &res) == 0 && res != NULL)
	{
		sin = (struct sockaddr_in *)res->ai_addr;
		*out = sin->sin_addr;
		freeaddrinfo(res);
		return (0);
	}
	(void)label;
	return (error_msg("unknown host or invalid IP address", str));
}

/* Convert one hex character to its value, or -1 if not a hex digit. */
static int	hex_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

/*
** Parse one MAC octet starting at *p ("a" or "ff", 1-2 hex digits).
** Advances *p past the digits, writes the byte to *out.
** Returns 0 on success, -1 on a malformed octet (0 or >2 digits).
*/
static int	parse_octet(const char **p, uint8_t *out)
{
	int	val;
	int	digit;
	int	count;

	val = 0;
	count = 0;
	while (count < 2 && (digit = hex_value(**p)) != -1)
	{
		val = val * 16 + digit;
		(*p)++;
		count++;
	}
	if (count == 0 || hex_value(**p) != -1)
		return (-1);
	*out = (uint8_t)val;
	return (0);
}

/*
** Validate and parse "aa:bb:cc:dd:ee:ff" into 6 bytes.
** Requires exactly 6 octets separated by exactly 5 colons and no trailing
** characters. Returns 0 on success, 1 (with error printed) otherwise.
*/
int	parse_mac(const char *str, uint8_t *out)
{
	const char	*p;
	int			i;

	p = str;
	i = 0;
	while (i < MAC_LEN)
	{
		if (parse_octet(&p, &out[i]) != 0)
			return (error_msg("invalid mac address", str));
		i++;
		if (i < MAC_LEN)
		{
			if (*p != ':')
				return (error_msg("invalid mac address", str));
			p++;
		}
	}
	if (*p != '\0')
		return (error_msg("invalid mac address", str));
	return (0);
}

/* Bonus: parse a strictly positive integer for --interval (seconds). */
static int	parse_interval(const char *str, unsigned int *out)
{
	unsigned long	value;
	const char		*p;

	p = str;
	if (*p == '\0')
		return (error_msg("invalid interval", str));
	value = 0;
	while (*p)
	{
		if (*p < '0' || *p > '9')
			return (error_msg("invalid interval", str));
		value = value * 10 + (unsigned long)(*p - '0');
		if (value > 86400UL)
			return (error_msg("invalid interval", str));
		p++;
	}
	if (value == 0)
		return (error_msg("invalid interval", str));
	*out = (unsigned int)value;
	return (0);
}

/*
** Exact string equality using only libft: ft_strncmp over the flag length
** plus its terminating '\0', so "-v" does not match "-verbose" and vice
** versa. (The bundled ft_strcmp is avoided: it compares pointers, not
** characters, so it is unreliable.)
*/
static int	flag_eq(const char *a, const char *flag)
{
	return (ft_strncmp(a, flag, ft_strlen(flag) + 1) == 0);
}

/*
** Recognise one optional flag at argv[*i]. Flags that take a value consume
** the next argv slot by advancing *i. Returns 1 if a flag was handled,
** 0 if argv[*i] is not a flag, -1 on a flag error (message already printed).
*/
static int	handle_flag(char **argv, int argc, int *i)
{
	char	*a;

	a = argv[*i];
	if (flag_eq(a, "-v") || flag_eq(a, "--verbose"))
		return (g_malcolm.verbose = 1, 1);
	if (flag_eq(a, "-r") || flag_eq(a, "--repeat"))
		return (g_malcolm.repeat = 1, 1);
	if (flag_eq(a, "-i") || flag_eq(a, "--interface"))
	{
		if (++(*i) >= argc)
			return (usage(), -1);
		ft_strlcpy(g_malcolm.forced_iface, argv[*i], IFNAMSIZ);
		return (g_malcolm.has_forced_iface = 1, 1);
	}
	if (flag_eq(a, "--interval"))
	{
		if (++(*i) >= argc || parse_interval(argv[*i], &g_malcolm.interval))
			return (-1);
		return (1);
	}
	if (a[0] == '-' && a[1] != '\0')
		return (usage(), -1);
	return (0);
}

/*
** Split argv into optional flags and exactly four positional arguments
** (source ip, source mac, target ip, target mac), then validate them.
*/
int	parse_arguments(int argc, char **argv)
{
	char	*pos[4];
	int		n;
	int		i;
	int		flag;

	n = 0;
	i = 1;
	g_malcolm.interval = 2;
	while (i < argc)
	{
		flag = handle_flag(argv, argc, &i);
		if (flag == -1)
			return (1);
		if (flag == 0 && n < 4)
			pos[n++] = argv[i];
		else if (flag == 0)
			return (usage(), 1);
		i++;
	}
	if (n != 4)
		return (usage(), 1);
	if (parse_ip(pos[0], &g_malcolm.src_ip, "source ip")
		|| parse_mac(pos[1], g_malcolm.src_mac)
		|| parse_ip(pos[2], &g_malcolm.tgt_ip, "target ip")
		|| parse_mac(pos[3], g_malcolm.tgt_mac))
		return (1);
	return (0);
}
