#include "mysocket.h"

Socket::Socket(int port):sockstream(&conn) {
    OpenSocket(port);
}

Socket::~Socket() { 
}

void Socket::OpenSocket(int port) {

    struct sockaddr_in address[1];
    int                i;

    if ( (sock = socket( PF_INET, SOCK_STREAM, 0 )) < 0 )
        cerr << "Can't create socket:" << strerror(errno) << endl;


    /* Tell TCP not to delay small packets.  This greatly speeds up
    interactive response. WARNING: If TCP_NODELAY is set on, then gdb
    may timeout in mid-packet if the (gdb)packet is not sent within a
    single (tcp)packet, thus all outgoing (gdb)packets _must_ be sent
    with a single call to write. (see Stevens "Unix Network
    Programming", Vol 1, 2nd Ed, page 202 for more info) */

#ifdef WE_ACT_AS_SERVER
    socklen_t          addrLength[1];
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    memset( &address->sin_addr, 0, sizeof(address->sin_addr) );


    if ( bind( sock, (struct sockaddr *)address, sizeof(address) ) )
        cerr << "Can not bind socket: "<< strerror(errno) << endl;


    if ( listen(sock, 1) )
    {
        int saved_errno = errno;
        cerr << "Can not listen on socket: " <<  strerror(saved_errno) << endl;
    }

    fprintf( stderr, "Waiting on port %d for user interface client to connect...\n", port );

    /* accept() needs this set, or it fails (sometimes) */
    addrLength[0] = sizeof(struct sockaddr *);

    /* We only want to accept a single connection, thus don't need a loop. */
    conn = accept( sock, (struct sockaddr *)address, addrLength );
    if (conn < 0)
    {
        int saved_errno = errno;
        /*
        if ( signal_has_occurred(SIGINT) )
        {
        break;          // SIGINT will cause accept to be interrupted 
        }
        */
        cerr<<  "Accept connection failed: " <<  strerror(saved_errno) << endl;
    }

    i = 1;
    setsockopt (conn, IPPROTO_TCP, TCP_NODELAY, &i, sizeof (i));
#else //so we are client
    address->sin_family= AF_INET;
    address->sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &address->sin_addr);
    conn=sock;

    int retry=0;
    int ret;
    do {
        ret = connect ( sock, (struct sockaddr *)address, sizeof(address));
        if (ret<0) {
            cerr << "No connect to socket possible now... retry " << strerror(errno) << endl;
            sleep(1);
        } else {
            break; //leave retry loop
        }
        
    } while (retry++ <10);

    if (ret<0) {
        cerr << "Could not contact the ui-server, sorry"<< endl;
        exit(0);
    }


#endif


    /* Let the kernel reuse the socket address. This lets us run
    twice in a row, without waiting for the (ip, port) tuple
    to time out. */
    i = 1;
    setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i) );

    /* If we got this far, we now have a client connected and can start 
    processing. */

    cerr << "User Interface Connection opened by host "<< inet_ntoa(address->sin_addr) << " port " <<   ntohs(address->sin_port) << endl;

    fcntl(conn, F_SETFL, O_NONBLOCK);
}


ssize_t Socket::Read( string &a) {
    char dummy[256];
    ssize_t len=read( conn, &dummy, 255 );
    if (len<0) len=0;
    dummy[len]=0;
    a+=dummy;
    return len; 
}

void Socket::Write(const string &s) {
    int err=::write( conn, s.c_str(), s.size());
    if (err<0) { cerr << "Write in UI fails!" << endl; } 
}

ssize_t Socket::Poll() {
    pollfd pfd= {
        conn,
        POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI,
        0
    };

    int erg=poll( &pfd, 1, 0); 
    if (erg<0) {
        //perror ("Error in polling");
        return 0; //nix gelesen ggf unterbechung
    }

    return erg;
}
