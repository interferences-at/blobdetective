#include <iostream>
#include <lo/lo.h>
#include "oscinterface.h"

namespace blobspy {

OscInterface::OscInterface(
        const std::string &peer_identifier,
        const std::string &send_port,
        const std::string &send_addr)
    :
    peer_id(peer_identifier),
    sender_(send_addr, send_port)
{
    std::cout << "Sending to osc_udp://" << send_addr << ":" << send_port << std::endl;
}

void OscInterface::send_blob_position(int x, int y, int diameter)
{
    sender_.sendMessage("/blob", "siii", this->peer_id.c_str(), x, y, diameter, LO_ARGS_END);
}

} // end of namespace
