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
		return (ft_strncmp(ifa->ifa_name, g_malcolm.forced_iface,
				ft_strlen(g_malcolm.forced_iface) + 1) == 0);
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
			ft_strlcpy(g_malcolm.ifname, ifa->ifa_name, IFNAMSIZ);
			g_malcolm.ifindex = if_nametoindex(ifa->ifa_name);
			ft_memcpy(g_malcolm.if_mac, sll->sll_addr, MAC_LEN);
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
** Create a raw packet socket that only receives ARP frames: the protocol
** filter htons(ETH_P_ARP) makes the kernel hand us Ethernet frames whose
** type is ARP and nothing else. We do not bind the socket; sending is
** directed by the sll_ifindex we set in sendto(), and incoming frames are
** filtered by content in wait_for_request().
*/
int	open_socket(void)
{
	g_malcolm.sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (g_malcolm.sockfd < 0)
		return (error_msg(strerror(errno), NULL));
	return (0);
}
