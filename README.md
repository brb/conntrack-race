# conntrack race

Discussion: http://patchwork.ozlabs.org/patch/937963/

## Reproducing

1. Apply [nfct_debug.patch](patch/nfct_debug.patch) on top of the proposed
   patch ^^.
2. Enable the dynamic debug: `echo -n 'file net/netfilter/nf_conntrack_core.c
   +p' > /sys/kernel/debug/dynamic_debug/control`.
3. Compile and run [server.c](src/server.c): `gcc server.c -o server &&
   ./server 8000`.
4. Compile and run [client.c](src/client.c): `gcc -lpthread client.c -o client
   && ./client 127.0.0.1 8000`.

## Logs

### No NAT

[logs](log/no-nat/)

- No NAT rules, but NAT null-bindings enabled.
- Client and server communicates over `lo`.

### DNAT

[logs](log/dnat/)

- [DNAT rules](log/dnat/iptables-save-nat.output) are installed.
- Client and server communicates over `lo`.

### SNAT

[logs](log/dnat/)

- [SNAT rules](log/snat/iptables-save-nat.output) are installed by Docker.
- Client runs in a container network namespace, and its IPv4 addr is `172.17.0.2`.
- Communicates with `172.28.128.3` (remote) via `172.28.128.1` (gw).
