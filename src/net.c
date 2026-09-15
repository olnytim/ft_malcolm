/* ************************************************************************** */
/*                                                                            */
/*   net.c - interface discovery and raw socket creation.                    */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

/*
** Return 1 if this ifaddrs entry is a usable link-layer (AF_PACKET) address:
** it must be UP, RUNNING and not the loopback interface.
*/
static int	is_usable(struct ifaddrs *ifa)
{
	if (ifa->ifa_addr == NULL)
		return (0);
	if (ifa->ifa_addr->sa_family != AF_PACKET)
		return (0);
	/* Bonus: if the user forced an interface with -i, match it by name    */
	/* and skip the loopback/up/running heuristics (their explicit choice).*/
	if (g_malcolm.has_forced_iface)
		return (!strcmp(ifa->ifa_name, g_malcolm.forced_iface));
	if (ifa->ifa_flags & IFF_LOOPBACK)
		return (0);
	if (!(ifa->ifa_flags & IFF_UP) || !(ifa->ifa_flags & IFF_RUNNING))
		return (0);
	return (1);
}

/*
** Walk the list of interfaces with getifaddrs() and pick the first usable
** one. Store its name, kernel index and MAC address in the global state.
** Prints "Found available interface: <name>" like the subject example.
*/
int	find_interface(void)
{
	struct ifaddrs		*list;
	struct ifaddrs		*ifa;
	struct sockaddr_ll	*sll;

	if (getifaddrs(&list) != 0)
		return (error_msg(strerror(errno), NULL));
	ifa = list;
	while (ifa != NULL)
	{
		if (is_usable(ifa))
		{
			sll = (struct sockaddr_ll *)ifa->ifa_addr;
			strncpy(g_malcolm.ifname, ifa->ifa_name, IFNAMSIZ - 1);
			g_malcolm.ifindex = if_nametoindex(ifa->ifa_name);
			memcpy(g_malcolm.if_mac, sll->sll_addr, MAC_LEN);
			freeifaddrs(list);
			printf("Found available interface: %s\n", g_malcolm.ifname);
			fflush(stdout);
			return (0);
		}
		ifa = ifa->ifa_next;
	}
	freeifaddrs(list);
	return (error_msg("no usable network interface found", NULL));
}

/*
** Create a raw packet socket that only receives ARP frames
** (protocol filter htons(ETH_P_ARP)) and bind it to our interface, so
** recvfrom() and sendto() both operate on that link.
*/
int	open_socket(void)
{
	struct sockaddr_ll	sll;

	g_malcolm.sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (g_malcolm.sockfd < 0)
		return (error_msg(strerror(errno), NULL));
	memset(&sll, 0, sizeof(sll));
	sll.sll_family = AF_PACKET;
	sll.sll_protocol = htons(ETH_P_ARP);
	sll.sll_ifindex = g_malcolm.ifindex;
	if (bind(g_malcolm.sockfd, (struct sockaddr *)&sll, sizeof(sll)) < 0)
		return (error_msg(strerror(errno), NULL));
	return (0);
}
