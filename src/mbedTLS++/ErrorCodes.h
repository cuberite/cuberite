
/** Error codes from mbedtls net_sockets.h */
// TODO: Replace with std::error_code

#define MBEDTLS_ERR_NET_SOCKET_FAILED    -0x0042  /**< Failed to open a socket. */
#define MBEDTLS_ERR_NET_CONNECT_FAILED   -0x0044  /**< The connection to the given server / port failed. */
#define MBEDTLS_ERR_NET_BIND_FAILED      -0x0046  /**< Binding of the socket failed. */
#define MBEDTLS_ERR_NET_LISTEN_FAILED    -0x0048  /**< Could not listen on the socket. */
#define MBEDTLS_ERR_NET_ACCEPT_FAILED    -0x004A  /**< Could not accept the incoming connection. */
#define MBEDTLS_ERR_NET_RECV_FAILED      -0x004C  /**< Reading information from the socket failed. */
#define MBEDTLS_ERR_NET_SEND_FAILED      -0x004E  /**< Sending information through the socket failed. */
#define MBEDTLS_ERR_NET_CONN_RESET       -0x0050  /**< Connection was reset by peer. */
#define MBEDTLS_ERR_NET_UNKNOWN_HOST     -0x0052  /**< Failed to get an IP address for the given hostname. */
#define MBEDTLS_ERR_NET_BUFFER_TOO_SMALL -0x0043  /**< Buffer is too small to hold the data. */
#define MBEDTLS_ERR_NET_INVALID_CONTEXT  -0x0045  /**< The context is invalid, eg because it was free()ed. */



