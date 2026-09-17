/* ************************************************************************** */
/*                                                                            */
/*   main.c - ft_malcolm entry point: ARP spoofing / poisoning demo.         */
/*                                                                            */
/*   Usage: sudo ./ft_malcolm [-v] source_ip source_mac target_ip target_mac */
/*                                                                            */
/*   Waits for the target to broadcast an ARP request for source_ip, then    */
/*   sends one spoofed ARP reply binding source_ip to source_mac and exits.  */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

/* The one and only global variable allowed by the subject. */
t_malcolm	g_malcolm;

void	usage(void)
{
	fprintf(stderr,
		"Usage: %s [-v] [-r] [--interval N] [-i iface] "
		"source_ip source_mac target_ip target_mac\n",
		PROG_NAME);
}

/*
** SIGINT (Ctrl+C) handler. Uses only async-signal-safe calls: write(),
** close() and _exit(). This guarantees a clean, predictable shutdown
** without leaving the raw socket open.
*/
static void	handle_sigint(int sig)
{
	const char	msg[] = "\nExiting program...\n";

	(void)sig;
	write(STDOUT_FILENO, msg, sizeof(msg) - 1);
	if (g_malcolm.sockfd >= 0)
		close(g_malcolm.sockfd);
	_exit(0);
}

void	setup_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

int	main(int argc, char **argv)
{
	ft_memset(&g_malcolm, 0, sizeof(g_malcolm));
	g_malcolm.sockfd = -1;
	if (parse_arguments(argc, argv) != 0)
		return (EXIT_FAILURE);
	if (getuid() != 0)
		return (error_msg("you must run this program as root (raw sockets "
				"require privileges)", NULL));
	if (find_interface() != 0)
		return (EXIT_FAILURE);
	if (open_socket() != 0)
	{
		if (g_malcolm.sockfd >= 0)
			close(g_malcolm.sockfd);
		return (EXIT_FAILURE);
	}
	setup_signals();
	if (wait_for_request() != 0 || send_reply() != 0)
	{
		close(g_malcolm.sockfd);
		return (EXIT_FAILURE);
	}
	if (g_malcolm.repeat)
		persist();
	close(g_malcolm.sockfd);
	g_malcolm.sockfd = -1;
	printf("Exiting program...\n");
	return (EXIT_SUCCESS);
}
