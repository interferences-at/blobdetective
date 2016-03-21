#include <iostream>
#include <lo/lo.h>
#include "oscinterface.h"

namespace blobdetective {

OscInterface::OscInterface(
        const std::string &peer_identifier,
        const std::string &send_port,
        const std::string &send_addr,
        const std::string &receive_port)
    :
    peer_id(peer_identifier),
    sender_(send_addr, send_port),
    send_enabled_(true),
    receiving_enabled_(true),
    receiver_(receive_port)
{
    std::cout << "Sending to osc_udp://" << send_addr << ":" << send_port << std::endl;
}

void OscInterface::send_blob_position(float x, float y, float size)
{
    sender_.sendMessage("/blob", "sfff", this->peer_id.c_str(), x, y, size, LO_ARGS_END);
}

OscInterface::~OscInterface()
{

}

/** Starts listening if enabled
 */
void OscInterface::start_receiver()
{
    if (receiving_enabled_)
    {
        // start a thread to try and subscribe us
        receiver_.listen(); // start listening in separate thread
    }
}

int OscInterface::option_string_cb(
        const char *path,
        const char * /*types*/, 
        lo_arg ** /*argv*/,
        int /*argc*/, 
        void * /*data*/, 
        void *user_data)
{
    OscInterface* context = static_cast<OscInterface*>(user_data);
    if (context->is_verbose())
        std::cout << "Got " << path << std::endl;
    // context->push_command(std::tr1::shared_ptr<Command>(new AddImageCommand));
    return 0;
}

bool OscInterface::is_verbose()
{
    return true; // FIXME
}


} // end of namespace
