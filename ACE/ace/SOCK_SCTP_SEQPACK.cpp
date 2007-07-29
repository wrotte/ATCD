// $Id$

#include "ace/SOCK_SCTP_SEQPACK.h"
#include <iostream>
#include <netinet/sctp.h>
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_SOCK_SCTP_SEQPACK::ACE_SOCK_SCTP_SEQPACK (void)
{
}

ACE_SOCK_SCTP_SEQPACK::~ACE_SOCK_SCTP_SEQPACK (void)
{
}

int
ACE_SOCK_SCTP_SEQPACK::open (const ACE_Multihomed_INET_Addr &local_sap,
                             int reuse_addr,
                             int protocol_family)
                         
{
  ACE_TRACE ("ACE_SOCK_SCTP_SEQPACK::open");
  int error = 0;
  
  if (protocol_family == PF_UNSPEC)
    protocol_family = local_sap.get_type ();

  if (ACE_SOCK::open (SOCK_SEQPACKET,
                      protocol_family,
                      IPPROTO_SCTP,
                      reuse_addr) == -1)
  {
    return -1;
  }

  if (protocol_family == PF_INET)
    {
      sockaddr_in local_inet_addr;
      ACE_OS::memset (reinterpret_cast<void *> (&local_inet_addr),
                      0,
                      sizeof local_inet_addr);

      if (local_sap.ACE_Addr::operator== (ACE_Addr::sap_any))
        {
          local_inet_addr.sin_port = 0;
        }
      else
        local_inet_addr = *reinterpret_cast<sockaddr_in *> (local_sap.get_addr ());

        {
          // The total number of addresses is the number of secondary
          // addresses plus one.
          size_t num_addresses = local_sap.get_num_secondary_addresses() + 1;

          // Create an array of sockaddr_in to hold the underlying
          // representations of the primary and secondary
          // addresses.
          sockaddr_in*  local_inet_addrs = 0;
          ACE_NEW_NORETURN (local_inet_addrs,
                            sockaddr_in[num_addresses]);

          if (!local_inet_addrs)
            error = 1;
          else
            {
              // Populate the array by invoking the get_addresses method
              // on the Multihomed_INET_Addr
              local_sap.get_addresses(local_inet_addrs,
                                      num_addresses);

#if defined (ACE_HAS_LKSCTP)

              sockaddr_in *local_sockaddr = 0;

              // bind the primary first
              if (ACE_OS::bind (this->get_handle (),
                                reinterpret_cast<sockaddr *> (&(local_inet_addrs[0])),
                                sizeof(sockaddr)) == -1)
              {
                error = 1;
              }

              // do we need to bind multiple addresses?
              if (num_addresses > 1)
              {
                ACE_NEW_NORETURN(local_sockaddr,
                               sockaddr_in[num_addresses - 1]);

                // all of the secondary addresses need the local port set
                for (size_t i = 1; i < num_addresses; i++)
                {
                  local_inet_addrs[i].sin_port = local_inet_addrs[0].sin_port;
                }

                // copy only the sockaddrs that we need to bindx
                for (size_t i = 0; i < num_addresses - 1; i++)
                {
                  ACE_OS::memcpy(&(local_sockaddr[i]),
                                 &(local_inet_addrs[i + 1]),
                                 sizeof(sockaddr_in));
                }

                // now call bindx
                if (!error && sctp_bindx(this->get_handle (),
                                         reinterpret_cast<sockaddr *> (local_sockaddr),
                                         num_addresses - 1,
                                         SCTP_BINDX_ADD_ADDR))
                {
                  error = 1;
                }

                delete [] local_sockaddr;
              }
#else
              // Call bind
              size_t name_len = (sizeof local_inet_addr) * num_addresses;
              if (ACE_OS::bind (this->get_handle (),
                                reinterpret_cast<sockaddr *> (local_inet_addrs),
                                static_cast<int> (name_len)) == -1)
                error = 1;
#endif /* ACE_HAS_LKSCTP */
            }

          delete [] local_inet_addrs;
        }
    }
  else if (ACE_OS::bind (this->get_handle (),
                         (sockaddr *) local_sap.get_addr (),
                         local_sap.get_size ()) == -1)
    error = 1;

  return error ? -1 : 0;
}

int ACE_SOCK_SCTP_SEQPACK::peeloff(int assoc_id, ACE_SOCK_SCTP_STREAM& peer)
{
  // Call the underlying sctp_peeloff method to return a 1-to-1 style 
  // socket
  int new_handle = sctp_peeloff(this->get_handle(), assoc_id);

  if(new_handle != -1)
  {
    peer.set_handle(new_handle);
  }

  return new_handle;
}

ACE_END_VERSIONED_NAMESPACE_DECL
