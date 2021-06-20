// static
int QuicSocketUtils::ReadPacket(int fd,
                                char *buffer,
                                size_t buf_len,
                                QuicPacketCount *dropped_packets,
                                QuicIpAddress *self_address,
                                QuicWallTime *walltimestamp,
                                QuicSocketAddress *peer_address)
{
    DCHECK(peer_address != nullptr);
    char cbuf[kCmsgSpaceForReadPacket];

    iovec iov = {buffer, buf_len};
    struct sockaddr_storage raw_address;
    msghdr hdr;

    hdr.msg_name = &raw_address;
    hdr.msg_namelen = sizeof(sockaddr_storage);
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
    hdr.msg_flags = 0;

    struct cmsghdr *cmsg = reinterpret_cast<struct cmsghdr *>(cbuf);
    cmsg->cmsg_len = QUIC_ARRAYSIZE(cbuf);
    hdr.msg_control = cmsg;
    hdr.msg_controllen = QUIC_ARRAYSIZE(cbuf);

    int bytes_read = recvmsg(fd, &hdr, 0);

    // Return before setting dropped packets: if we get EAGAIN, it will
    // be 0.
    if (bytes_read < 0 && errno != 0)
    {
        if (errno != EAGAIN)
        {
            LOG(ERROR) << "Error reading " << strerror(errno);
        }
        return -1;
    }
    if (hdr.msg_flags & MSG_CTRUNC)
    {
        QUIC_BUG << "Incorrectly set control length: " << hdr.msg_controllen
                 << ", expected " << QUIC_ARRAYSIZE(cbuf);
        return -1;
    }

    if (dropped_packets != nullptr)
    {
        GetOverflowFromMsghdr(&hdr, dropped_packets);
    }

    QuicIpAddress stack_address;
    if (self_address == nullptr)
    {
        self_address = &stack_address;
    }

    QuicWallTime stack_walltimestamp = QuicWallTime::FromUNIXMicroseconds(0);
    if (walltimestamp == nullptr)
    {
        walltimestamp = &stack_walltimestamp;
    }

    GetAddressAndTimestampFromMsghdr(&hdr, self_address, walltimestamp);

    *peer_address = QuicSocketAddress(raw_address);
    return bytes_read;
}

// static
void QuicSocketUtils::GetAddressAndTimestampFromMsghdr(
    struct msghdr *hdr,
    QuicIpAddress *address,
    QuicWallTime *walltimestamp)
{
    if (hdr->msg_controllen > 0)
    {
        for (cmsghdr *cmsg = CMSG_FIRSTHDR(hdr); cmsg != nullptr;
             cmsg = CMSG_NXTHDR(hdr, cmsg))
        {
            char *addr_data = nullptr;
            int len = 0;
            if (cmsg->cmsg_type == IPV6_PKTINFO)
            {
                in6_pktinfo *info = reinterpret_cast<in6_pktinfo *>(CMSG_DATA(cmsg));
                addr_data = reinterpret_cast<char *>(&info->ipi6_addr);
                len = sizeof(in6_addr);
                address->FromPackedString(addr_data, len);
            }
            else if (cmsg->cmsg_type == IP_PKTINFO)
            {
                in_pktinfo *info = reinterpret_cast<in_pktinfo *>(CMSG_DATA(cmsg));
                addr_data = reinterpret_cast<char *>(&info->ipi_addr);
                len = sizeof(in_addr);
                address->FromPackedString(addr_data, len);
            }
            else if (cmsg->cmsg_level == SOL_SOCKET &&
                     cmsg->cmsg_type == SO_TIMESTAMPING)
            {
                LinuxTimestamping *lts =
                    reinterpret_cast<LinuxTimestamping *>(CMSG_DATA(cmsg));
                timespec *ts = &lts->systime;
                int64_t usec = (static_cast<int64_t>(ts->tv_sec) * 1000 * 1000) +
                               (static_cast<int64_t>(ts->tv_nsec) / 1000);
                *walltimestamp = QuicWallTime::FromUNIXMicroseconds(usec);
            }
        }
    }
}