/* ************************************************************************** */
/*                                                                            */
/*   ft_malcolm.h                                                             */
/*                                                                            */
/*   ARP spoofing / poisoning - an introduction to Man In The Middle.        */
/*   Linux only: relies on AF_PACKET raw sockets (RFC 826 / RFC 7042).       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <signal.h>
# include <ctype.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <ifaddrs.h>
# include <net/if.h>
# include <netinet/in.h>
# include <net/ethernet.h>
# include <netinet/if_ether.h>
# include <linux/if_packet.h>

# include "../libft/libft.h"

/* Ethernet type for ARP frames (0x0806). Defined by <linux/if_ether.h>,     */
/* re-declared here defensively in case a platform header misses it.         */
# ifndef ETH_P_ARP
#  define ETH_P_ARP 0x0806
# endif
# ifndef ETH_P_IP
#  define ETH_P_IP 0x0800
# endif
# ifndef ARPHRD_ETHER
#  define ARPHRD_ETHER 1
# endif
# ifndef ARPOP_REQUEST
#  define ARPOP_REQUEST 1
# endif
# ifndef ARPOP_REPLY
#  define ARPOP_REPLY 2
# endif

# define MAC_LEN 6
# define IPV4_LEN 4
# define PROG_NAME "ft_malcolm"

/*
** A complete ARP-over-Ethernet frame, laid out byte for byte on the wire.
** __attribute__((packed)) forbids the compiler from inserting padding, so
** the struct maps 1:1 onto the 42 bytes we send/receive.
*/
typedef struct __attribute__((packed)) s_arp_frame
{
	/* --- Ethernet header (14 bytes) --- */
	uint8_t		eth_dst[MAC_LEN];	/* destination MAC                     */
	uint8_t		eth_src[MAC_LEN];	/* source MAC                          */
	uint16_t	eth_type;			/* 0x0806 = ARP (network byte order)   */
	/* --- ARP payload (28 bytes) --- */
	uint16_t	htype;				/* hardware type: 1 = Ethernet         */
	uint16_t	ptype;				/* protocol type: 0x0800 = IPv4        */
	uint8_t		hlen;				/* hardware addr length: 6             */
	uint8_t		plen;				/* protocol addr length: 4             */
	uint16_t	oper;				/* 1 = request, 2 = reply              */
	uint8_t		sha[MAC_LEN];		/* sender hardware address             */
	uint8_t		spa[IPV4_LEN];		/* sender protocol (IPv4) address      */
	uint8_t		tha[MAC_LEN];		/* target hardware address             */
	uint8_t		tpa[IPV4_LEN];		/* target protocol (IPv4) address      */
}	t_arp_frame;

/*
** The single global state (the one global variable the subject allows).
** It holds everything the signal handler must reach to exit cleanly.
*/
typedef struct s_malcolm
{
	int				sockfd;				/* raw socket, -1 when closed        */
	int				ifindex;			/* chosen interface index            */
	char			ifname[IFNAMSIZ];	/* chosen interface name             */
	uint8_t			if_mac[MAC_LEN];	/* our real MAC on that interface    */
	struct in_addr	src_ip;				/* spoofed source IPv4               */
	uint8_t			src_mac[MAC_LEN];	/* spoofed source MAC                */
	struct in_addr	tgt_ip;				/* victim (target) IPv4              */
	uint8_t			tgt_mac[MAC_LEN];	/* victim (target) MAC               */
	int				verbose;			/* bonus: -v verbose mode            */
	int				repeat;				/* bonus: -r keep re-poisoning       */
	unsigned int	interval;			/* bonus: seconds between re-sends   */
	int				has_forced_iface;	/* bonus: -i was given               */
	char			forced_iface[IFNAMSIZ];	/* bonus: interface name         */
}	t_malcolm;

extern t_malcolm	g_malcolm;

/* main.c */
void	usage(void);
void	setup_signals(void);

/* args.c */
int		parse_arguments(int argc, char **argv);
int		parse_ip(const char *str, struct in_addr *out, const char *label);
int		parse_mac(const char *str, uint8_t *out);

/* net.c */
int		find_interface(void);
int		open_socket(void);

/* arp.c */
int		wait_for_request(void);
int		send_reply(void);
void	persist(void);

/* utils.c */
int		error_msg(const char *fmt, const char *arg);
void	mac_to_string(const uint8_t *mac, char *dst);
void	print_hex_dump(const char *title, const uint8_t *data, size_t len);

#endif
