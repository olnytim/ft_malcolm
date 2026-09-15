# ft_malcolm

An introduction to Man In The Middle attacks: an ARP spoofing / poisoning tool
(42 network security project, based on RFC 826 and RFC 7042).

## Build

```sh
make
```

This is a **Linux-only** program: it uses `AF_PACKET` raw sockets, which do not
exist on macOS/BSD. Build and run it inside a Linux VM (the subject targets a
Debian VM with kernel > 3.14).

## Usage

```sh
sudo ./ft_malcolm [-v] [-r] [--interval N] [-i iface] source_ip source_mac target_ip target_mac
```

- `source_ip`  : the IP you want to impersonate (must belong to you).
- `source_mac` : the (spoofed) MAC to bind to `source_ip`.
- `target_ip`  : the victim's IP.
- `target_mac` : the victim's MAC.

IPv4 arguments accept dotted decimal (`10.12.10.22`), a single 32-bit decimal
integer (`168561174`) or a hostname (`localhost`).

### Bonus flags

- `-v`, `--verbose`     : hex dump of the received request and the sent reply.
- `-r`, `--repeat`      : after the first reply, keep re-poisoning until Ctrl+C.
- `--interval N`        : seconds between re-sends in repeat mode (default 2).
- `-i`, `--interface X` : force the network interface instead of auto-detecting.

The program waits until the target broadcasts an ARP request asking
"who has `source_ip`?", then sends a single spoofed ARP reply and exits.
Afterwards the victim's ARP table maps `source_ip` to `source_mac`.

> You are only allowed to spoof IPs that belong to you (e.g. your own VM).
> Spoofing other IPs may result in problems and/or sanctions.
