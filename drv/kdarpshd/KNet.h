#ifdef __cplusplus
extern "C" 
{
#endif

#define  BINARY_COMPATIBLE 0 
//#define  NDIS_WDM          0
#include <ndis.h>

//==============================================================================
#define NET_LIL_ENDIAN   1234
#define NET_BIG_ENDIAN   4321

// BSD and IRIX
#ifdef   BYTE_ORDER
#if      BYTE_ORDER ==   LITTLE_ENDIAN
# define NET_BYTESEX     NET_LIL_ENDIAN
#elif    BYTE_ORDER ==   BIG_ENDIAN
# define NET_BYTESEX     NET_BIG_ENDIAN
#endif
#endif

// Linux
#ifdef __BYTE_ORDER
#if    __BYTE_ORDER == __LITTLE_ENDIAN
# define NET_BYTESEX     NET_LIL_ENDIAN
#elif  __BYTE_ORDER == __BIG_ENDIAN
# define NET_BYTESEX     NET_BIG_ENDIAN
#endif
#endif

// Solaris
#if defined(_BIT_FIELDS_LTOH)
# define NET_BYTESEX     NET_LIL_ENDIAN
#elif defined (_BIT_FIELDS_HTOL)
# define NET_BYTESEX     NET_BIG_ENDIAN
#endif

// Win32 - XXX
#ifdef _WIN32
# define NET_BYTESEX     NET_LIL_ENDIAN
#endif

//==============================================================================

__inline unsigned short __swab16(unsigned short x)
{
  unsigned short __x = x;
  return ((unsigned short)((((unsigned short)(__x)&(unsigned short)0x00ffU)<< 8) | (((unsigned short)(__x)&(unsigned short)0xff00U)>> 8)));
}

__inline unsigned long __swab32(unsigned long x)
{
  unsigned long __x = (x);
  return ((unsigned long)(
    (((unsigned long)(__x) & (unsigned long)0x000000ffUL) << 24) |
    (((unsigned long)(__x) & (unsigned long)0x0000ff00UL) <<  8) | 
    (((unsigned long)(__x) & (unsigned long)0x00ff0000UL) >>  8) |
    (((unsigned long)(__x) & (unsigned long)0xff000000UL) >> 24) ));
}

__inline unsigned __int64 __swab64(unsigned __int64 x)
{
  unsigned __int64 __x = (x);
  return ((unsigned __int64)(
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0x00000000000000ffUL) << 56) |
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0x000000000000ff00UL) << 40) |
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0x0000000000ff0000UL) << 24) |
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0x00000000ff000000UL) <<  8) |
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0x000000ff00000000UL) >>  8) |
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0x0000ff0000000000UL) >> 24) |
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0x00ff000000000000UL) >> 40) |
    ( unsigned __int64)(((unsigned __int64)(__x) & (unsigned __int64)0xff00000000000000UL) >> 56) ));
}

#define __constant_swab16(x)                                       \
  ( (unsigned short)(                                              \
  (((unsigned short)(x) & (unsigned short)0x00ffU) << 8) |         \
  (((unsigned short)(x) & (unsigned short)0xff00U) >> 8) ))

#define __constant_swab32(x)                                       \
  ( (unsigned long)(                                               \
  (((unsigned long)(x) & (unsigned long)0x000000ffUL) << 24) |     \
  (((unsigned long)(x) & (unsigned long)0x0000ff00UL) <<  8) |     \
  (((unsigned long)(x) & (unsigned long)0x00ff0000UL) >>  8) |     \
  (((unsigned long)(x) & (unsigned long)0xff000000UL) >> 24) ))

#define __constant_swab64(x)                                                                        \
  ((unsigned __int64)(                                                                              \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0x00000000000000ffULL) << 56) |    \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0x000000000000ff00ULL) << 40) |    \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0x0000000000ff0000ULL) << 24) |    \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0x00000000ff000000ULL) <<  8) |    \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0x000000ff00000000ULL) >>  8) |    \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0x0000ff0000000000ULL) >> 24) |    \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0x00ff000000000000ULL) >> 40) |    \
  ( unsigned __int64)(((unsigned __int64)(x) & (unsigned __int64)0xff00000000000000ULL) >> 56) ))

//==============================================================================

#if NET_BYTESEX == NET_LIL_ENDIAN
#define HTONL(x) (__constant_swab32((x)))
#define NTOHL(x) (__constant_swab32((x)))
#define HTONS(x) (__constant_swab16((x)))
#define NTOHS(x) (__constant_swab16((x)))
#else
#define HTONL(x) (x)
#define NTOHL(x) (x)
#define HTONS(x) (x)
#define NTOHS(x) (x)
#endif

__inline unsigned short NTAPI ntohs(unsigned short x)
{
#if NET_BYTESEX == NET_LIL_ENDIAN
  return __swab16(x);
#else
  return(x);
#endif
}

__inline unsigned short NTAPI htons(unsigned short x)
{
#if NET_BYTESEX == NET_LIL_ENDIAN
  return __swab16(x);
#else
  return(x);
#endif
}

__inline unsigned long NTAPI ntohl(unsigned long x)
{
#if NET_BYTESEX == NET_LIL_ENDIAN
  return __swab32(x);
#else
  return(x);
#endif
}

__inline unsigned long NTAPI htonl(unsigned long x)
{
#if NET_BYTESEX == NET_LIL_ENDIAN
  return __swab32(x);
#else
  return(x);
#endif
}

//==============================================================================

ULONG  NTAPI inet_addr(IN const char *cp);


//==============================================================================
#ifndef NET_ETH_H
#define NET_ETH_H

#define ETH_ADDR_LEN                     6
#define ETH_ADDR_BITS                    48
#define ETH_TYPE_LEN                     2
#define ETH_CRC_LEN                      4
#define ETH_HDR_LEN                      14
#define ETH_LEN_MIN                      64   // minimum frame length with CRC
#define ETH_LEN_MAX                      1518 // maximum frame length with CRC
#define ETH_MTU                          (ETH_LEN_MAX - ETH_HDR_LEN - ETH_CRC_LEN)
#define ETH_MIN                          (ETH_LEN_MIN - ETH_HDR_LEN - ETH_CRC_LEN)

#pragma pack(push, 1)
typedef struct _ETH_ADDR
{
	unsigned char data[ETH_ADDR_LEN];
} ETH_ADDR, *PETH_ADDR;

// Ethernet Packet Header
typedef struct _ETH_HDR
{
	unsigned char  eth_dst[ETH_ADDR_LEN];  // Destination address
	unsigned char  eth_src[ETH_ADDR_LEN];  // Source address
	unsigned short eth_type;    // Ethernet type field. // unsigned short eth_len; // 802.3 length field.
} ETH_HDR, *PETH_HDR;

typedef void* LPADAPTER;
typedef void* LPPACKET;

typedef struct _eth_handle
{
	LPADAPTER lpa;
	LPPACKET  pkt;
} eth_handle, *peth_handle; 
typedef struct eth_handle eth, *peth; 
#pragma pack(pop)

// Ethernet payload types - http://standards.ieee.org/regauth/ethertype
#define ETH_TYPE_PUP                     0x0200   // PUP protocol
#define ETH_TYPE_IPV4                    0x0800   // IP  protocol
#define ETH_TYPE_ARP                     0x0806   // address resolution protocol
#define ETH_TYPE_REVARP                  0x8035   // reverse addr resolution protocol
#define ETH_TYPE_8021Q                   0x8100   // IEEE 802.1Q VLAN tagging
#define ETH_TYPE_IPV6                    0x86DD   // IPv6 protocol
#define ETH_TYPE_MPLS                    0x8847   // MPLS
#define ETH_TYPE_MPLS_MCAST              0x8848   // MPLS Multicast
#define ETH_TYPE_PPPOEDISC               0x8863   // PPP Over Ethernet Discovery Stage
#define ETH_TYPE_PPPOE                   0x8864   // PPP Over Ethernet Session Stage
#define ETH_TYPE_LOOPBACK                0x9000   // used to test interfaces
#undef  ETH_IS_MULTICAST
#define ETH_IS_MULTICAST(ea)             (*(ea)&0x01) // is address mcast/bcast?
#define ETH_ADDR_BROADCAST               "\xff\xff\xff\xff\xff\xff"

#define BuildEthHeader(ethHdr, dst, src, type)              \
{                                                           \
	RtlCopyMemory(&(ethHdr)->eth_dst[0], (dst), ETH_ADDR_LEN);  \
	RtlCopyMemory(&(ethHdr)->eth_src[0], (src), ETH_ADDR_LEN);  \
	(ethHdr)->eth_type = HTONS((type));                         \
}
#endif // DNET_ETH_H
//==============================================================================

//==============================================================================
#ifndef NET_IP4_H
#define NET_IP4_H

#define IP4_ADDR_LEN                     4                              // IP address length
#define IP4_ADDR_BITS                    32                             // IP address bits

#define IP4_HDR_LEN                      20                             // base IP header length
#define IP4_OPT_LEN                      2                              // base IP option length
#define IP4_OPT_LEN_MAX                  40
#define IP4_HDR_LEN_MAX                  (IP4_HDR_LEN + IP4_OPT_LEN_MAX)

#define IP4_LEN_MAX                      65535
#define IP4_LEN_MIN                      IP4_HDR_LEN

#pragma pack(push, 1)
typedef unsigned long IP4_ADDR, *PIP4_ADDR;
// IP header, without options, Internet Protocol (IP) Packet Header
typedef struct _IP4_HDR
{
	union
	{
		unsigned char    ip4_verlen;   // Version and length
		struct  
		{
#if   NET_BYTESEX == NET_BIG_ENDIAN
			unsigned char  ip4_hv:4;     // version
			unsigned char  ip4_hl:4;     // header length (incl any options)
#else NET_BYTESEX == NET_LIL_ENDIAN
			unsigned char  ip4_hl:4;     // header length (incl any options)
			unsigned char  ip4_hv:4;     // version
#endif
		};
	};
	unsigned char      ip4_tos;      // Type of service
	unsigned short     ip4_len;      // Total datagram length
	unsigned short     ip4_id;       // Identification
	unsigned short     ip4_off;      // Flags, fragment offset
	unsigned char      ip4_ttl;      // Time to live
	unsigned char      ip4_pol;      // Protocol
	unsigned short     ip4_sum;      // Header checksum
	unsigned long      ip4_src;      // Source address
	unsigned long      ip4_dst;      // Destination address
} IP4_HDR, *PIP4_HDR;

// Type of service (ip_tos), RFC 1349 ("obsoleted by RFC 2474")
#define IP4_TOS_DEFAULT                  0x00 // default
#define IP4_TOS_LOWDELAY                 0x10 // low delay
#define IP4_TOS_THROUGHPUT               0x08 // high throughput
#define IP4_TOS_RELIABILITY              0x04 // high reliability
#define IP4_TOS_LOWCOST                  0x02 // low monetary cost - XXX
#define IP4_TOS_ECT                      0x02 // ECN-capable transport
#define IP4_TOS_CE                       0x01 // congestion experienced

// IP precedence (high 3 bits of ip_tos), hopefully unused
#define IP4_TOS_PREC_ROUTINE             0x00
#define IP4_TOS_PREC_PRIORITY            0x20
#define IP4_TOS_PREC_IMMEDIATE           0x40
#define IP4_TOS_PREC_FLASH               0x60
#define IP4_TOS_PREC_FLASHOVERRIDE       0x80
#define IP4_TOS_PREC_CRITIC_ECP          0xa0
#define IP4_TOS_PREC_INTERNETCONTROL     0xc0
#define IP4_TOS_PREC_NETCONTROL          0xe0

// Fragmentation flags (ip_off)
#define IP4_RF                           0x8000 // reserved
#define IP4_DF                           0x4000 // don't fragment
#define IP4_MF                           0x2000 // more fragments (not last frag)
#define IP4_OFFMASK                      0x1fff // mask for fragment offset

// Time-to-live (ip_ttl), seconds
#define IP4_TTL_DEFAULT                  64   // default ttl, RFC 1122, RFC 1340
#define IP4_TTL_MAX                      255  // maximum ttl

// Protocol (ip_pol) - http://www.iana.org/assignments/protocol-numbers
#define IP4_PROTO_IP                     0    // dummy for IP
#define IP4_PROTO_HOPOPTS                IP4_PROTO_IP  // IPv6 hop-by-hop options
#define IP4_PROTO_ICMP                   1    // ICMP
#define IP4_PROTO_IGMP                   2    // IGMP
#define IP4_PROTO_GGP                    3    // gateway-gateway protocol
#define IP4_PROTO_IPIP                   4    // IP in IP
#define IP4_PROTO_ST                     5    // ST datagram mode
#define IP4_PROTO_TCP                    6    // TCP
#define IP4_PROTO_CBT                    7    // CBT
#define IP4_PROTO_EGP                    8    // exterior gateway protocol
#define IP4_PROTO_IGP                    9    // interior gateway protocol
#define IP4_PROTO_BBNRCC                 10   // BBN RCC monitoring
#define IP4_PROTO_NVP                    11   // Network Voice Protocol
#define IP4_PROTO_PUP                    12   // PARC universal packet
#define IP4_PROTO_ARGUS                  13   // ARGUS
#define IP4_PROTO_EMCON                  14   // EMCON
#define IP4_PROTO_XNET                   15   // Cross Net Debugger
#define IP4_PROTO_CHAOS                  16   // Chaos
#define IP4_PROTO_UDP                    17   // UDP
#define IP4_PROTO_MUX                    18   // multiplexing
#define IP4_PROTO_DCNMEAS                19   // DCN measurement
#define IP4_PROTO_HMP                    20   // Host Monitoring Protocol
#define IP4_PROTO_PRM                    21   // Packet Radio Measurement
#define IP4_PROTO_IDP                    22   // Xerox NS IDP
#define IP4_PROTO_TRUNK1                 23   // Trunk-1
#define IP4_PROTO_TRUNK2                 24   // Trunk-2
#define IP4_PROTO_LEAF1                  25   // Leaf-1
#define IP4_PROTO_LEAF2                  26   // Leaf-2
#define IP4_PROTO_RDP                    27   // "Reliable Datagram" proto
#define IP4_PROTO_IRTP                   28   // Inet Reliable Transaction
#define IP4_PROTO_TP                     29   // ISO TP class 4
#define IP4_PROTO_NETBLT                 30   // Bulk Data Transfer
#define IP4_PROTO_MFPNSP                 31   // MFE Network Services
#define IP4_PROTO_MERITINP               32   // Merit Internodal Protocol
#define IP4_PROTO_SEP                    33   // Sequential Exchange proto
#define IP4_PROTO_3PC                    34   // Third Party Connect proto
#define IP4_PROTO_IDPR                   35   // Interdomain Policy Route
#define IP4_PROTO_XTP                    36   // Xpress Transfer Protocol
#define IP4_PROTO_DDP                    37   // Datagram Delivery Proto
#define IP4_PROTO_CMTP                   38   // IDPR Ctrl Message Trans
#define IP4_PROTO_TPPP                   39   // TP++ Transport Protocol
#define IP4_PROTO_IL                     40   // IL Transport Protocol
#define IP4_PROTO_IPV6                   41   // IPv6
#define IP4_PROTO_SDRP                   42   // Source Demand Routing
#define IP4_PROTO_ROUTING                43   // IPv6 routing header
#define IP4_PROTO_FRAGMENT               44   // IPv6 fragmentation header
#define IP4_PROTO_RSVP                   46   // Reservation protocol
#define IP4_PROTO_GRE                    47   // General Routing Encap
#define IP4_PROTO_MHRP                   48   // Mobile Host Routing
#define IP4_PROTO_ENA                    49   // ENA
#define IP4_PROTO_ESP                    50   // Encap Security Payload
#define IP4_PROTO_AH                     51   // Authentication Header
#define IP4_PROTO_INLSP                  52   // Integated Net Layer Sec
#define IP4_PROTO_SWIPE                  53   // SWIPE
#define IP4_PROTO_NARP                   54   // NBMA Address Resolution
#define IP4_PROTO_MOBILE                 55   // Mobile IP, RFC 2004
#define IP4_PROTO_TLSP                   56   // Transport Layer Security
#define IP4_PROTO_SKIP                   57   // SKIP
#define IP4_PROTO_ICMPV6                 58   // ICMP for IPv6
#define IP4_PROTO_NONE                   59   // IPv6 no next header
#define IP4_PROTO_DSTOPTS                60   // IPv6 destination options
#define IP4_PROTO_ANYHOST                61   // any host internal proto
#define IP4_PROTO_CFTP                   62   // CFTP
#define IP4_PROTO_ANYNET                 63   // any local network
#define IP4_PROTO_EXPAK                  64   // SATNET and Backroom EXPAK
#define IP4_PROTO_KRYPTOLAN              65   // Kryptolan
#define IP4_PROTO_RVD                    66   // MIT Remote Virtual Disk
#define IP4_PROTO_IPPC                   67   // Inet Pluribus Packet Core
#define IP4_PROTO_DISTFS                 68   // any distributed fs
#define IP4_PROTO_SATMON                 69   // SATNET Monitoring
#define IP4_PROTO_VISA                   70   // VISA Protocol
#define IP4_PROTO_IPCV                   71   // Inet Packet Core Utility
#define IP4_PROTO_CPNX                   72   // Comp Proto Net Executive
#define IP4_PROTO_CPHB                   73   // Comp Protocol Heart Beat
#define IP4_PROTO_WSN                    74   // Wang Span Network
#define IP4_PROTO_PVP                    75   // Packet Video Protocol
#define IP4_PROTO_BRSATMON               76   // Backroom SATNET Monitor
#define IP4_PROTO_SUNND                  77   // SUN ND Protocol
#define IP4_PROTO_WBMON                  78   // WIDEBAND Monitoring
#define IP4_PROTO_WBEXPAK                79   // WIDEBAND EXPAK
#define IP4_PROTO_EON                    80   // ISO CNLP
#define IP4_PROTO_VMTP                   81   // Versatile Msg Transport*/
#define IP4_PROTO_SVMTP                  82   // Secure VMTP
#define IP4_PROTO_VINES                  83   // VINES
#define IP4_PROTO_TTP                    84   // TTP
#define IP4_PROTO_NSFIGP                 85   // NSFNET-IGP
#define IP4_PROTO_DGP                    86   // Dissimilar Gateway Proto
#define IP4_PROTO_TCF                    87   // TCF
#define IP4_PROTO_EIGRP                  88   // EIGRP
#define IP4_PROTO_OSPF                   89   // Open Shortest Path First
#define IP4_PROTO_SPRITERPC              90   // Sprite RPC Protocol
#define IP4_PROTO_LARP                   91   // Locus Address Resolution
#define IP4_PROTO_MTP                    92   // Multicast Transport Proto
#define IP4_PROTO_AX25                   93   // AX.25 Frames
#define IP4_PROTO_IPIPENCAP              94   // yet-another IP encap
#define IP4_PROTO_MICP                   95   // Mobile Internet Ctrl
#define IP4_PROTO_SCCSP                  96   // Semaphore Comm Sec Proto
#define IP4_PROTO_ETHERIP                97   // Ethernet in IPv4
#define IP4_PROTO_ENCAP                  98   // encapsulation header
#define IP4_PROTO_ANYENC                 99   // private encryption scheme
#define IP4_PROTO_GMTP                   100  // GMTP
#define IP4_PROTO_IFMP                   101  // Ipsilon Flow Mgmt Proto
#define IP4_PROTO_PNNI                   102  // PNNI over IP
#define IP4_PROTO_PIM                    103  // Protocol Indep Multicast
#define IP4_PROTO_ARIS                   104  // ARIS
#define IP4_PROTO_SCPS                   105  // SCPS
#define IP4_PROTO_QNX                    106  // QNX
#define IP4_PROTO_AN                     107  // Active Networks
#define IP4_PROTO_IPCOMP                 108  // IP Payload Compression
#define IP4_PROTO_SNP                    109  // Sitara Networks Protocol
#define IP4_PROTO_COMPAQPEER             110  // Compaq Peer Protocol
#define IP4_PROTO_IPXIP                  111  // IPX in IP
#define IP4_PROTO_VRRP                   112  // Virtual Router Redundancy
#define IP4_PROTO_PGM                    113  // PGM Reliable Transport
#define IP4_PROTO_ANY0HOP                114  // 0-hop protocol
#define IP4_PROTO_L2TP                   115  // Layer 2 Tunneling Proto
#define IP4_PROTO_DDX                    116  // D-II Data Exchange (DDX)
#define IP4_PROTO_IATP                   117  // Interactive Agent Xfer
#define IP4_PROTO_STP                    118  // Schedule Transfer Proto
#define IP4_PROTO_SRP                    119  // SpectraLink Radio Proto
#define IP4_PROTO_UTI                    120  // UTI
#define IP4_PROTO_SMP                    121  // Simple Message Protocol
#define IP4_PROTO_SM                     122  // SM
#define IP4_PROTO_PTP                    123  // Performance Transparency
#define IP4_PROTO_ISIS                   124  // ISIS over IPv4
#define IP4_PROTO_FIRE                   125  // FIRE
#define IP4_PROTO_CRTP                   126  // Combat Radio Transport
#define IP4_PROTO_CRUDP                  127  // Combat Radio UDP
#define IP4_PROTO_SSCOPMCE               128  // SSCOPMCE
#define IP4_PROTO_IPLT                   129  // IPLT
#define IP4_PROTO_SPS                    130  // Secure Packet Shield
#define IP4_PROTO_PIPE                   131  // Private IP Encap in IP
#define IP4_PROTO_SCTP                   132  // Stream Ctrl Transmission
#define IP4_PROTO_FC                     133  // Fibre Channel
#define IP4_PROTO_RSVPIGN                134  // RSVP-E2E-IGNORE
#define IP4_PROTO_RAW                    255  // Raw IP packets
#define IP4_PROTO_RESERVED               IP4_PROTO_RAW         // Reserved
#define IP4_PROTO_MAX                    255

// Option types (opt_type) - http://www.iana.org/assignments/ip-parameters
#define IP4_OPT_CONTROL                  0x00                 // control
#define IP4_OPT_DEBMEAS                  0x40                 // debugging & measurement
#define IP4_OPT_COPY                     0x80                 // copy into all fragments
#define IP4_OPT_RESERVED1                0x20
#define IP4_OPT_RESERVED2                0x60

#define IP4_OPT_EOL                      0                    // end of option list
#define IP4_OPT_NOP                      1                    // no operation
#define IP4_OPT_SEC                      (2|IP4_OPT_COPY)      // DoD basic security
#define IP4_OPT_LSRR                     (3|IP4_OPT_COPY)      // loose source route
#define IP4_OPT_TS                       (4|IP4_OPT_DEBMEAS)   // timestamp
#define IP4_OPT_ESEC                     (5|IP4_OPT_COPY)      // DoD extended security
#define IP4_OPT_CIPSO                    (6|IP4_OPT_COPY)      // commercial security
#define IP4_OPT_RR                       7                    // record route
#define IP4_OPT_SATID                    (8|IP4_OPT_COPY)      // stream ID (obsolete)
#define IP4_OPT_SSRR                     (9|IP4_OPT_COPY)      // strict source route
#define IP4_OPT_ZSU                      10                   // experimental measurement
#define IP4_OPT_MTUP                     11                   // MTU probe
#define IP4_OPT_MTUR                     12                   // MTU reply
#define IP4_OPT_FINN                     (13|IP4_OPT_COPY|IP4_OPT_DEBMEAS)  // exp flow control
#define IP4_OPT_VISA                     (14|IP4_OPT_COPY)     // exp access control
#define IP4_OPT_ENCODE                   15                   // ???
#define IP4_OPT_IMITD                    (16|IP4_OPT_COPY)     // IMI traffic descriptor
#define IP4_OPT_EIP                      (17|IP4_OPT_COPY)     // extended IP, RFC 1385
#define IP4_OPT_TR                       (18|IP4_OPT_DEBMEAS)  // traceroute
#define IP4_OPT_ADDEXT                   (19|IP4_OPT_COPY)     // IPv7 ext addr, RFC 1475
#define IP4_OPT_RTRALT                   (20|IP4_OPT_COPY)     // router alert, RFC 2113
#define IP4_OPT_SDB                      (21|IP4_OPT_COPY)     // directed bcast, RFC 1770
#define IP4_OPT_NSAPA                    (22|IP4_OPT_COPY)     // NSAP addresses
#define IP4_OPT_DPS                      (23|IP4_OPT_COPY)     // dynamic packet state
#define IP4_OPT_UMP                      (24|IP4_OPT_COPY)     // upstream multicast
#define IP4_OPT_MAX                      25

#define IP4_OPT_COPIED(o)                ((o) & 0x80)
#define IP4_OPT_CLASS(o)                 ((o) & 0x60)
#define IP4_OPT_NUMBER(o)                ((o) & 0x1f)
#define IP4_OPT_TYPEONLY(o)              ((o) == IP4_OPT_EOL || (o) == IP4_OPT_NOP)

// Security option data - RFC 791, 3.1
typedef struct _IP4_OPT_DATA_SEC
{
	unsigned short  s;                           // security
	unsigned short  c;                           // compartments
	unsigned short  h;                           // handling restrictions
	unsigned char   tcc[3];                      // transmission control code 
} IP4_OPT_DATA_SEC, *PIP4_OPT_DATA_SEC;

#define IP4_OPT_SEC_UNCLASS              0x0000 // unclassified
#define IP4_OPT_SEC_CONFID               0xf135 // confidential
#define IP4_OPT_SEC_EFTO                 0x789a // EFTO
#define IP4_OPT_SEC_MMMM                 0xbc4d // MMMM
#define IP4_OPT_SEC_PROG                 0x5e26 // PROG
#define IP4_OPT_SEC_RESTR                0xaf13 // restricted
#define IP4_OPT_SEC_SECRET               0xd788 // secret
#define IP4_OPT_SEC_TOPSECRET            0x6bc5 // top secret

// {Loose Source, Record, Strict Source} Route option data - RFC 791, 3.1
typedef struct _IP4_OPT_DATA_RR
{
	unsigned char ptr;                            // from start of option, >= 4
	unsigned long iplist;                         // list of IP addresses
} IP4_OPT_DATA_RR, *PIP4_OPT_DATA_RR;

// Timestamp option data - RFC 791, 3.1
typedef struct _IP4_OPT_DATA_TS
{
	union
	{
		unsigned char   ptr;                        // from start of option, >= 5
		struct  
		{
#if   NET_BYTESEX == NET_BIG_ENDIAN
			unsigned char oflw:4;                     // number of IPs skipped
			unsigned char flg :4;                     // address[ / timestamp] flag
#elif NET_BYTESEX == NET_LIL_ENDIAN
			unsigned char flg :4;                     // address[ / timestamp] flag
			unsigned char oflw:4;                     // number of IPs skipped
#endif
		};
	};
	unsigned long     ipts;                       // IP address [/ timestamp] pairs 
} IP4_OPT_DATA_TS, *PIP4_OPT_DATA_TS;

#define IP4_OPT_TS_TSONLY                0      // timestamps only
#define IP4_OPT_TS_TSADDR                1      // IP address / timestamp pairs
#define IP4_OPT_TS_PRESPEC               3      // IP address / zero timestamp pairs

// Traceroute option data - RFC 1393, 2.2
typedef struct _IP4_OPT_DATA_TR
{
	unsigned short id;                            // ID number
	unsigned short ohc;                           // outbound hop count
	unsigned short rhc;                           // return hop count
	unsigned long  origip;                        // originator IP address
} IP4_OPT_DATA_TR, *PIP4_OPT_DATA_TR;

// IP option (following IP header)
typedef struct _IP4_OPT
{
	unsigned char      opt_type;                  // option type
	unsigned char      opt_len;                   // option length >= IP4_OPT_LEN
	union
	{
		IP4_OPT_DATA_SEC sec;                       // IP4_OPT_SEC
		IP4_OPT_DATA_RR  rr;                        // IP4_OPT_{L,S}RR
		IP4_OPT_DATA_TS  ts;                        // IP4_OPT_TS 
		unsigned short   satid;                     // IP4_OPT_SATID
		unsigned short   mtu;                       // IP4_OPT_MTU{P,R}
		IP4_OPT_DATA_TR  tr;                        // IP4_OPT_TR
		unsigned long    addext[2];                 // IP4_OPT_ADDEXT
		unsigned short   rtralt;                    // IP4_OPT_RTRALT
		unsigned long    sdb[9];                    // IP4_OPT_SDB
		unsigned char    data8[IP4_OPT_LEN_MAX - IP4_OPT_LEN];
	}                  opt_data;
} IP4_OPT, *PIP4_OPT;

typedef struct _IP4_PACKET
{
  ETH_HDR eh_hdr;
  IP4_HDR ip_hdr;
} IP4_PACKET, *PIP4_PACKET;

#pragma pack(pop)


// Classful addressing
#define IP4_CLASSA(i)        (((unsigned long)(i) & HTONL(0x80000000)) ==  HTONL(0x00000000))
#define IP4_CLASSA_NET       (HTONL(0xff000000))
#define IP4_CLASSA_NSHIFT    24
#define IP4_CLASSA_HOST      (HTONL(0x00ffffff))
#define IP4_CLASSA_MAX       128

#define IP4_CLASSB(i)        (((unsigned long)(i) & HTONL(0xc0000000)) == HTONL(0x80000000))
#define IP4_CLASSB_NET       (HTONL(0xffff0000))
#define IP4_CLASSB_NSHIFT     16
#define IP4_CLASSB_HOST      (HTONL(0x0000ffff))
#define IP4_CLASSB_MAX       65536

#define IP4_CLASSC(i)        (((unsigned long)(i) & HTONL(0xe0000000)) == HTONL(0xc0000000))
#define IP4_CLASSC_NET       (HTONL(0xffffff00))
#define IP4_CLASSC_NSHIFT    8
#define IP4_CLASSC_HOST      (HTONL(0x000000ff))
#define IP4_CLASSD(i)        (((unsigned long)(i) & HTONL(0xf0000000)) == HTONL(0xe0000000))
// These ones aren't really net and host fields, but routing needn't know.
#define IP4_CLASSD_NET       (HTONL(0xf0000000))
#define IP4_CLASSD_NSHIFT    28
#define IP4_CLASSD_HOST      (HTONL(0x0fffffff))
#define IP4_MULTICAST(i)     IP4_CLASSD(i)

#define IP4_EXPERIMENTAL(i)  (((unsigned long)(i) & HTONL(0xf0000000)) == HTONL(0xf0000000))
#define IP4_BADCLASS(i)      (((unsigned long)(i) & HTONL(0xf0000000)) == HTONL(0xf0000000))
#define IP4_LOCAL_GROUP(i)   (((unsigned long)(i) & HTONL(0xffffff00)) == HTONL(0xe0000000))

// Reserved addresses
#define IP4_ADDR_ANY         (HTONL(0x00000000)) // 0.0.0.0
#define IP4_ADDR_BROADCAST   (HTONL(0xffffffff)) // 255.255.255.255
#define IP4_ADDR_LOOPBACK    (HTONL(0x7f000001)) // 127.0.0.1
#define IP4_ADDR_MCAST_ALL   (HTONL(0xe0000001)) // 224.0.0.1
#define IP4_ADDR_MCAST_LOCAL (HTONL(0xe00000ff)) // 224.0.0.255

#define BuildIp4Header(ip4Hdr, len, sum, tos, id, off, ttl, pol, src, dst) \
{                                                                          \
  (ip4Hdr)->ip4_hv  = 4;                                                   \
  (ip4Hdr)->ip4_hl  = 5;                                                   \
  (ip4Hdr)->ip4_tos = (tos);                                               \
  (ip4Hdr)->ip4_len = HTONS((len));                                        \
  (ip4Hdr)->ip4_id  = HTONS((id ));                                        \
  (ip4Hdr)->ip4_off = HTONS((off));                                        \
  (ip4Hdr)->ip4_ttl = (ttl);                                               \
  (ip4Hdr)->ip4_pol = (pol);                                               \
  (ip4Hdr)->ip4_sum = (sum);                                               \
  (ip4Hdr)->ip4_src = (src);                                               \
  (ip4Hdr)->ip4_dst = (dst);                                               \
}

#define  IP4_CKSUM_CARRY(x)  (x = (x >> 16) + (x & 0xffff), (~(x + (x >> 16)) & 0xffff))

#endif // NET_IP4_H 
//==============================================================================

//==============================================================================
#ifndef NET_IP6_H
#define NET_IP6_H

#define IP6_ADDR_LEN                     16
#define IP6_ADDR_BITS                    128

#define IP6_HDR_LEN                      40            // IPv6 header length
#define IP6_LEN_MIN                      IP6_HDR_LEN
#define IP6_LEN_MAX                      65535         // non-jumbo payload

#define IP6_MTU_MIN                      1280          // minimum MTU (1024 + 256)

#pragma pack(push, 1)
typedef struct _IP6_ADDR
{
	unsigned char data[IP6_ADDR_LEN];
} IP6_ADDR, *PIP6_ADDR;

// IPv6 header
typedef struct _IP6_HDR
{
	union
	{
		struct
		{
			unsigned long  ip6_un1_flow;     // 20 bits of flow ID
			unsigned short ip6_un1_plen;     // payload length
			unsigned char  ip6_un1_nxt;      // next header
			unsigned char  ip6_un1_hlim;     // hop limit
		}                ip6_un1;
		unsigned char    ip6_un2_vfc;      // 4 bits version, top 4 bits class
	}                  ip6_ctlun;
	IP6_ADDR           ip6_src;
	IP6_ADDR           ip6_dst;
} IP6_HDR, *PIP6_HDR;

#define ip6_vfc                          ip6_ctlun.ip6_un2_vfc
#define ip6_flow                         ip6_ctlun.ip6_un1.ip6_un1_flow
#define ip6_plen                         ip6_ctlun.ip6_un1.ip6_un1_plen
#define ip6_nxt                          ip6_ctlun.ip6_un1.ip6_un1_nxt    // IP4_PROTO_*
#define ip6_hlim                         ip6_ctlun.ip6_un1.ip6_un1_hlim

#define IP6_VERSION                      0x60
#define IP6_VERSION_MASK                 0xf0        // ip6_vfc version

#if   NET_BYTESEX == NET_BIG_ENDIAN
#define IP6_FLOWINFO_MASK                0x0fffffff  // ip6_flow info  (28 bits)
#define IP6_FLOWLABEL_MASK               0x000fffff  // ip6_flow label (20 bits)
#elif NET_BYTESEX == NET_LIL_ENDIAN
#define IP6_FLOWINFO_MASK                0xffffff0f  // ip6_flow info  (28 bits)
#define IP6_FLOWLABEL_MASK               0xffff0f00  // ip6_flow label (20 bits)
#endif

// Hop limit (ip6_hlim)
#define IP6_HLIM_DEFAULT                 64
#define IP6_HLIM_MAX                     255

// Preferred extension header order from RFC 2460, 4.1:
//
// IP4_PROTO_IPV6, IP4_PROTO_HOPOPTS, IP4_PROTO_DSTOPTS, IP4_PROTO_ROUTING,
// IP4_PROTO_FRAGMENT, IP4_PROTO_AH, IP4_PROTO_ESP, IP4_PROTO_DSTOPTS, IP4_PROTO_*
// Routing header data (IP4_PROTO_ROUTING)
typedef struct _IP6_EXT_DATA_ROUTING
{
	unsigned char type;        // routing type
	unsigned char segleft;     // segments left
	// followed by routing type specific data
} IP6_EXT_DATA_ROUTING, *PIP6_EXT_DATA_ROUTING;

typedef struct _IP6_EXT_DATA_ROUTING0
{
	unsigned char type;        // always zero
	unsigned char segleft;     // segments left
	unsigned char reserved;    // reserved field
	unsigned char slmap[3];    // strict/loose bit map
	IP6_ADDR      addr[1];     // up to 23 addresses
} IP6_EXT_DATA_ROUTING0, *PIP6_EXT_DATA_ROUTING0;

// Fragment header data (IP4_PROTO_FRAGMENT)
typedef struct _IP6_EXT_DATA_FRAGMENT
{
	unsigned short offlg;      // offset, reserved, and flag
	unsigned long  ident;      // identification
} IP6_EXT_DATA_FRAGMENT, *PIP6_EXT_DATA_FRAGMENT;

// Extension header (chained via {ip6,ext}_nxt, following IPv6 header)
typedef struct _IP6_EXT_HEADER
{
	unsigned char           ext_nxt;                     // next header
	unsigned char           ext_len;                     // following length in units of 8 octets */
	union
	{
		IP6_EXT_DATA_ROUTING  routing;
		IP6_EXT_DATA_FRAGMENT fragment;
	} ext_data;
} IP6_EXT_HEADER, *PIP6_EXT_HEADER;
#pragma pack(pop)

// Fragmentation offset, reserved, and flags (offlg)
#if   NET_BYTESEX == NET_BIG_ENDIAN
#define IP6_OFF_MASK                     0xfff8       // mask out offset from offlg
#define IP6_RESERVED_MASK                0x0006       // reserved bits in offlg
#define IP6_MORE_FRAG                    0x0001       // more-fragments flag
#elif NET_BYTESEX == NET_LIL_ENDIAN
#define IP6_OFF_MASK                     0xf8ff       // mask out offset from offlg
#define IP6_RESERVED_MASK                0x0600       // reserved bits in offlg
#define IP6_MORE_FRAG                    0x0100       // more-fragments flag
#endif

// Option types, for IP4_PROTO_HOPOPTS, IP4_PROTO_DSTOPTS headers
#define IP6_OPT_PAD1                     0x00          // 00 0 00000
#define IP6_OPT_PADN                     0x01          // 00 0 00001
#define IP6_OPT_JUMBO                    0xC2          // 11 0 00010 = 194
#define IP6_OPT_JUMBO_LEN                6
#define IP6_OPT_RTALERT                  0x05          // 00 0 00101
#define IP6_OPT_RTALERT_LEN              4
#define IP6_OPT_RTALERT_MLD              0             // Datagram contains an MLD message
#define IP6_OPT_RTALERT_RSVP             1             // Datagram contains an RSVP message
#define IP6_OPT_RTALERT_ACTNET           2             // contains an Active Networks msg
#define IP6_OPT_LEN_MIN                  2

#define IP6_OPT_TYPE(o)                  ((o) & 0xC0)  // high 2 bits of opt_type
#define IP6_OPT_TYPE_SKIP                0x00          // continue processing on failure
#define IP6_OPT_TYPE_DISCARD             0x40          // discard packet on failure
#define IP6_OPT_TYPE_FORCEICMP           0x80          // discard and send ICMP on failure
#define IP6_OPT_TYPE_ICMP                0xC0          // ...only if non-multicast dst
#define IP6_OPT_MUTABLE                  0x20          // option data may change en route

// Reserved addresses
#define IP6_ADDR_UNSPEC                  "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define IP6_ADDR_LOOPBACK                "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01"

// ip6_pack_hdr
#define BuildIp6Header(hdr, fc, fl, plen, nxt, hlim, src, dst)                      \
{                                                                                   \
	PIP6_HDR ip6Hdr = (PIP6_HDR)(hdr);                                                \
	ip6Hdr->ip6_flow = HTONL(((unsigned long)(fc)<<28)&(IP6_FLOWLABEL_MASK|(fl)));    \
	ip6Hdr->ip6_vfc  = (IP6_VERSION|((fc)>>4));                                       \
	ip6Hdr->ip6_plen = HTONS((plen));                                                 \
	ip6Hdr->ip6_nxt  = (nxt);                                                         \
	ip6Hdr->ip6_hlim = (hlim);                                                        \
	RtlCopyMemory(&ip6Header->ip6_src, &(src), IP6_ADDR_LEN);                         \
	RtlCopyMemory(&ip6Header->ip6_dst, &(dst), IP6_ADDR_LEN);                         \
};

// __BEGIN_DECLS
// char  *ip6_ntop(const ip6_addr *ip6, char *dst, size_t size);
// int    ip6_pton(const char *src, ip6_addr *dst);
// char  *ip6_ntoa(const ip6_addr *ip6);
// void   ip6_checksum(void *buf, size_t len);
// #define ip6_aton ip6_pton
// __END_DECLS
#endif // NET_IP6_H
//==============================================================================

//==============================================================================
#ifndef NET_ADDR_H
#define NET_ADDR_H

#define ADDR_TYPE_NONE                   0  // No address set
#define ADDR_TYPE_ETH                    1  // Ethernet
#define ADDR_TYPE_IP                     2  // Internet Protocol v4
#define ADDR_TYPE_IP6                    3  // Internet Protocol v6

typedef struct _NET_ADDR
{
	unsigned short addr_type;
	unsigned short addr_bits;
	union
	{
		ETH_ADDR       eth;
		IP4_ADDR       ip4;
		IP6_ADDR       ip6;
		unsigned char  data8[16];
		unsigned short data16[8];
		unsigned long  data32[4];
	} u;
} NET_ADDR, *PNET_ADDR;

#define addr_eth    u.eth
#define addr_ip4    u.ip4
#define addr_ip6    u.ip6
#define addr_data8  u.data8
#define addr_data16 u.data16
#define addr_data32 u.data32

// addr_pack
#define BuildNetAddr(addr, type, bits, data, len)          \
{                                                          \
	(addr)->addr_type = (type);                              \
	(addr)->addr_bits = (bits);                              \
	RtlCopyMemory((addr)->addr_data8, (char*)(data), (len)); \
}

#endif // NET_ADDR_H
//==============================================================================

//==============================================================================
#ifndef NET_ARP_H
#define NET_ARP_H

#define ARP_HDR_LEN                      8  // base ARP header  length
#define ARP_ETHIP4_LEN                   20 // base ARP message length

#pragma pack(push, 1)
// ARP header
typedef struct _ARP_HDR
{
	unsigned short ar_hrd;                 // format of hardware address
	unsigned short ar_pro;                 // format of protocol address
	unsigned char  ar_hln;                 // length of hardware address (ETH_ADDR_LEN)
	unsigned char  ar_pln;                 // length of protocol address (IP4_ADDR_LEN)
	unsigned short ar_op;                  // operation
} ARP_HDR, *PARP_HDR;

// Ethernet/IP ARP message
typedef struct _ARP_ETHIP
{
	unsigned char  ar_sha[ETH_ADDR_LEN];   // sender hardware address
	unsigned char  ar_spa[IP4_ADDR_LEN];   // sender protocol address
	unsigned char  ar_tha[ETH_ADDR_LEN];   // target hardware address
	unsigned char  ar_tpa[IP4_ADDR_LEN];   // target protocol address
} ARP_ETHIP, *PARP_ETHIP;

// ARP cache entry
typedef struct _ARP_ENTRY
{
	NET_ADDR arp_pa;                    // protocol address
	NET_ADDR arp_ha;                    // hardware address
} ARP_ENTRY, *PARP_ENTRY;

typedef struct _ARP_IP4_HDR
{
	unsigned short ar_hrd;                 // format of hardware address
	unsigned short ar_pro;                 // format of protocol address
	unsigned char  ar_hln;                 // length of hardware address
	unsigned char  ar_pln;                 // length of protocol address
	unsigned short ar_op;                  // ARP/RARP operation

	unsigned char  ar_sha[ETH_ADDR_LEN];   // sender hardware address
	unsigned char  ar_spa[IP4_ADDR_LEN];   // sender protocol address
	unsigned char  ar_tha[ETH_ADDR_LEN];   // target hardware address
	unsigned char  ar_tpa[IP4_ADDR_LEN];   // target protocol address
} ARP_IP4_HDR, *PARP_IP4_HDR;

typedef struct _ARP_PACKET
{
	ETH_HDR     eh_hdr;
	ARP_IP4_HDR ar_hdr;
} ARP_PACKET, *PARP_PACKET;
#pragma pack(pop)

// Hardware address format
#define ARP_HRD_ETH                      0x0001 // ethernet hardware
#define ARP_HRD_IEEE802                  0x0006 // IEEE 802 hardware

// Protocol address format
#define ARP_PRO_IP                       0x0800 // IP protocol

// ARP operation
#define ARP_OP_REQUEST                   1  // request to resolve ha given pa
#define ARP_OP_REPLY                     2  // response giving hardware address
#define ARP_OP_REVREQUEST                3  // request to resolve pa given ha
#define ARP_OP_REVREPLY                  4  // response giving protocol address

// arp_pack_hdr_ethip
__inline void InitializeArpIpHeader(void* hdr, unsigned short op, void* sha, void* spa, void* tha, void* tpa)
{
 PARP_IP4_HDR arpIpHeader = (PARP_IP4_HDR)(hdr);
 arpIpHeader->ar_hrd = HTONS(ARP_HRD_ETH);
 arpIpHeader->ar_pro = HTONS(ARP_PRO_IP);
 arpIpHeader->ar_hln = ETH_ADDR_LEN;
 arpIpHeader->ar_pln = IP4_ADDR_LEN;
 arpIpHeader->ar_op  = HTONS(op);
 RtlCopyMemory(arpIpHeader->ar_sha, (sha), ETH_ADDR_LEN);
 RtlCopyMemory(arpIpHeader->ar_spa, (spa), IP4_ADDR_LEN);
 RtlCopyMemory(arpIpHeader->ar_tha, (tha), ETH_ADDR_LEN);
 RtlCopyMemory(arpIpHeader->ar_tpa, (tpa), IP4_ADDR_LEN);
}

#define BuildArpHeader(hdr, op, sha, spa, tha, tpa)       \
{                                                         \
	PARP_IP4_HDR arpIp4Hdr = (PARP_IP4_HDR)(hdr);           \
	arpIp4Hdr->ar_hrd = HTONS(ARP_HRD_ETH);                 \
	arpIp4Hdr->ar_pro = HTONS(ARP_PRO_IP);                  \
	arpIp4Hdr->ar_hln = ETH_ADDR_LEN;                       \
	arpIp4Hdr->ar_pln = IP4_ADDR_LEN;                       \
	arpIp4Hdr->ar_op  = HTONS((op));                        \
	RtlCopyMemory(arpIp4Hdr->ar_sha, (sha), ETH_ADDR_LEN);  \
	RtlCopyMemory(arpIp4Hdr->ar_spa, (spa), IP4_ADDR_LEN);  \
	RtlCopyMemory(arpIp4Hdr->ar_tha, (tha), ETH_ADDR_LEN);  \
	RtlCopyMemory(arpIp4Hdr->ar_tpa, (tpa), IP4_ADDR_LEN);  \
}
#endif // NET_ARP_H 
//==============================================================================

//==============================================================================
#ifndef NET_INTF_H
#define NET_INTF_H

// Interface entry
#define INTF_NAME_LEN                    16

typedef struct _INTF_ENTRY
{
	unsigned int   intf_len;               // length of entry
	char           intf_name[INTF_NAME_LEN];//interface name
	unsigned short intf_type;              // interface type (r/o)
	unsigned short intf_flags;             // interface flags
	unsigned int   intf_mtu;               // interface MTU
	NET_ADDR       intf_addr;              // interface address
	NET_ADDR       intf_dst_addr;          // point-to-point dst
	NET_ADDR       intf_link_addr;         // link-layer address
	unsigned int   intf_alias_num;         // number of aliases
	NET_ADDR       intf_alias_addrs;       // array of aliases
} INTF_ENTRY, *PINTF_ENTRY;

// MIB-II interface types - http://www.iana.org/assignments/ianaiftype-mib
#define INTF_TYPE_OTHER                  1   // other
#define INTF_TYPE_ETH                    6   // Ethernet
#define INTF_TYPE_TOKENRING              9   // Token Ring
#define INTF_TYPE_FDDI                   15  // FDDI
#define INTF_TYPE_PPP                    23  // Point-to-Point Protocol
#define INTF_TYPE_LOOPBACK               24  // software loopback
#define INTF_TYPE_SLIP                   28  // Serial Line Interface Protocol
#define INTF_TYPE_TUN                    53  // proprietary virtual/internal

// Interface flags
#define INTF_FLAG_UP                     0x01  // enable interface
#define INTF_FLAG_LOOPBACK               0x02  // is a loopback net  (r/o)
#define INTF_FLAG_POINTOPOINT            0x04  // point-to-point link(r/o)
#define INTF_FLAG_NOARP                  0x08  // disable ARP
#define INTF_FLAG_BROADCAST              0x10  // supports broadcast (r/o)
#define INTF_FLAG_MULTICAST              0x20  // supports multicast (r/o)

#ifdef _WIN32

#define MIB_IF_TYPE_MAX                  32 // XXX - ipifcons.h
#define MAXLEN_IFDESCR                   256
#define MAX_INTERFACE_NAME_LEN           256
#ifndef ANY_SIZE
#define ANY_SIZE                         1
#endif

typedef struct _MIB_IFNUMBER
{
	unsigned long dwValue;
} MIB_IFNUMBER, *PMIB_IFNUMBER;


#define MAXLEN_IFDESCR 256
#define MAXLEN_PHYSADDR 8

typedef struct _MIB_IFROW
{
	unsigned short wszName[MAX_INTERFACE_NAME_LEN];
	unsigned long  dwIndex;
	unsigned long  dwType;
	unsigned long  dwMtu;
	unsigned long  dwSpeed;
	unsigned long  dwPhysAddrLen;
	unsigned char  bPhysAddr[MAXLEN_PHYSADDR];
	unsigned long  dwAdminStatus;
	unsigned long  dwOperStatus;
	unsigned long  dwLastChange;
	unsigned long  dwInOctets;
	unsigned long  dwInUcastPkts;
	unsigned long  dwInNUcastPkts;
	unsigned long  dwInDiscards;
	unsigned long  dwInErrors;
	unsigned long  dwInUnknownProtos;
	unsigned long  dwOutOctets;
	unsigned long  dwOutUcastPkts;
	unsigned long  dwOutNUcastPkts;
	unsigned long  dwOutDiscards;
	unsigned long  dwOutErrors;
	unsigned long  dwOutQLen;
	unsigned long  dwDescrLen;
	unsigned char  bDescr[MAXLEN_IFDESCR];
} MIB_IFROW,*PMIB_IFROW;

typedef struct _MIB_IFTABLE
{
	unsigned long dwNumEntries;
	MIB_IFROW     table[ANY_SIZE];
} MIB_IFTABLE, *PMIB_IFTABLE;

typedef struct _MIB_IPADDRROW
{
	unsigned long  dwAddr;
	unsigned long  dwIndex;
	unsigned long  dwMask;
	unsigned long  dwBCastAddr;
	unsigned long  dwReasmSize;
	unsigned short unused1;
	unsigned short wType;
} MIB_IPADDRROW, *PMIB_IPADDRROW;

typedef struct _MIB_IPADDRTABLE
{
	unsigned long  dwNumEntries;
	MIB_IPADDRROW  table[ANY_SIZE];
} MIB_IPADDRTABLE, *PMIB_IPADDRTABLE;

typedef struct _ifcombo
{
	unsigned long * idx;
	int             cnt;
	int             max;
} ifcombo, *pifcombo;

typedef struct _intf_handle
{
	ifcombo          ifcombo[MIB_IF_TYPE_MAX];
	PMIB_IFTABLE     iftable;
	PMIB_IPADDRTABLE iptable;
} intf_handle, *pintf_handle; 
#endif

typedef struct intf_handle intf, *pintf;

typedef int (pintf_handler)(CONST PINTF_ENTRY entry, PVOID arg);

// __BEGIN_DECLS
// pintf  intf_open(void);
// int    intf_get(intf_t *i, struct intf_entry *entry);
// int    intf_get_src(intf_t *i, struct intf_entry *entry, struct addr *src);
// int    intf_get_dst(intf_t *i, struct intf_entry *entry, struct addr *dst);
// int    intf_set(intf_t *i, const struct intf_entry *entry);
// int    intf_loop(intf_t *i, intf_handler callback, void *arg);
// pintf  intf_close(intf_t *i);
// __END_DECLS

#endif // DNET_INTF_H
//==============================================================================

//==============================================================================
// TCP Packet Header
/*
typedef struct _TCP_HDR
{
	USHORT tcp_sport;    // Source Port Number
	USHORT tcp_dport;    // Destination Port Number
	ULONG  tcp_seq;      // Sequence Number
	ULONG  tcp_ack_seq;  // Acknowlegdement Number
	USHORT tcp_flags;    // Flags
	USHORT tcp_window;   // Window
	USHORT tcp_check;    // Checksum
	USHORT tcp_urgent;   // Urgent pointer
} TCP_HDR, *PTCP_HDR;
*/
#pragma pack(push, 1)

typedef struct _PSD_HDR//定义TCP伪首部 
{ 
	ULONG  saddr; //源地址 
	ULONG  daddr; //目的地址 
	UCHAR  mbz; 
	UCHAR  ptcl;  //协议类型 
	USHORT tcpl;  //TCP长度 
} PSD_HDR, *PPSD_HDR;

typedef struct _TCP_HDR				//定义TCP首部
{
	USHORT th_sport;				//16位源端口
	USHORT th_dport;				//16位目的端口
	ULONG  th_seq;					//32位序列号
	ULONG  th_ack;					//32位确认号
	USHORT th_lenresflag;			//4位首部长度 6位保留字 6位标志位				
	USHORT th_win;					//16位窗口大小
	USHORT th_sum;					//16位校验和
	USHORT th_urp;					//16位紧急数据偏移量
} TCP_HDR, *PTCP_HDR;

#pragma pack(pop)

#define TCP_HDR_LEN             20
//==============================================================================
// Definitions of bits in internet address integers.

#define INADDR_ANY              (ULONG)0x00000000
#define INADDR_LOOPBACK         0x7F000001
#define INADDR_BROADCAST        (ULONG)0xFFFFFFFF
#define INADDR_NONE             0xFFFFFFFF

#define ADDR_ANY                INADDR_ANY

//==============================================================================
// 定义UDP首部

#define UDP_HDR_LEN                      8

typedef struct _UDP_HDR
{ 
  USHORT udp_sport;        // 16位源端口 
  USHORT udp_dport;        // 16位目的端口 
  USHORT udp_len;          // 16位长度 
  USHORT udp_sum;          // 16位校验和 
} UDP_HDR, *PUDP_HDR;

#define BuildUdpHeader(udpHdr, len, sum, sport, dport) \
{                                                      \
  (udpHdr)->udp_sport = (sport);                       \
  (udpHdr)->udp_dport = (dport);                       \
  (udpHdr)->udp_len   = HTONS((len));                  \
  (udpHdr)->udp_sum   = (sum);                         \
}

//==============================================================================

#ifdef __cplusplus
}
#endif