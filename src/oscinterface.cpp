#include <iostream>
#include <lo/lo.h>
#include "oscinterface.h"
#include "unused.h"

namespace blobdetective {

OscInterface::OscInterface(
        Application* application,
        int peer_identifier,
        const std::string &send_port,
        const std::string &send_addr,
        const std::string &receive_port)
    :
    application_(application),
    peer_id(peer_identifier),
    sender_(send_addr, send_port),
    send_enabled_(true),
    receiving_enabled_(true),
    receiver_(receive_port)
{
    std::cout << "Sending to osc_udp://" << send_addr << ":" << send_port << std::endl;

    if (receiving_enabled_)
    {
        std::cout << "Listening osc_udp://localhost:" << receive_port << std::endl;
        receiver_.addHandler("/ping", "", ping_cb, this);
        receiver_.addHandler("/option/string", "s", option_string_cb, this);
        receiver_.addHandler("/option/int", "i", option_int_cb, this);
        receiver_.addHandler("/option/boolean", "b", option_boolean_cb, this);
        receiver_.addHandler("/option/float", "f", option_float_cb, this);
    }
}

/**
 * Handles /ping. Answers with /pong
 *
 * \msc
 * Client,App;
 * Client->App [ label = "/ping" ];
 * App->Client [ label = "/pong" ];
 * \endmsc
 */
int OscInterface::ping_cb(
        const char *path, const char * /*types*/, lo_arg ** /*argv*/,
        int /*argc*/, void * /*data*/, void *user_data)
{ 
    UNUSED(path);
    OscInterface* context = static_cast<OscInterface*>(user_data);
    if (context->is_verbose())
        std::cout << "Got " << path << std::endl;
    //if (context->sending_enabled_)
    context->sender_.sendMessage("/pong", "", LO_ARGS_END);
    return 0;
}

int OscInterface::option_string_cb(
        const char *path, const char * /*types*/, lo_arg ** /*argv*/,
        int /*argc*/, void * /*data*/, void *user_data)
{ 
    UNUSED(path);
    OscInterface* context = static_cast<OscInterface*>(user_data);
    if (context->is_verbose())
        std::cout << "Got " << path << std::endl;
    //TODO context->get_app()->get_configuration()->
    return 0;
}

int OscInterface::option_boolean_cb(
        const char *path, const char * /*types*/, lo_arg ** /*argv*/,
        int /*argc*/, void * /*data*/, void *user_data)
{ 
    UNUSED(path);
    OscInterface* context = static_cast<OscInterface*>(user_data);
    if (context->is_verbose())
        std::cout << "Got " << path << std::endl;
    //TODO context->get_app()->get_configuration()->
    return 0;
}

int OscInterface::option_int_cb(
        const char *path, const char * /*types*/, lo_arg ** /*argv*/,
        int /*argc*/, void * /*data*/, void *user_data)
{ 
    UNUSED(path);
    OscInterface* context = static_cast<OscInterface*>(user_data);
    if (context->is_verbose())
        std::cout << "Got " << path << std::endl;
    //TODO context->get_app()->get_configuration()->
    return 0;
}

int OscInterface::option_float_cb(
        const char *path, const char * /*types*/, lo_arg ** /*argv*/,
        int /*argc*/, void * /*data*/, void *user_data)
{ 
    UNUSED(path);
    OscInterface* context = static_cast<OscInterface*>(user_data);
    if (context->is_verbose())
        std::cout << "Got " << path << std::endl;
    //TODO context->get_app()->get_configuration()->
    return 0;
}

void OscInterface::send_blob_position(float x, float y, float size)
{
    sender_.sendMessage("/blob", "ifff", this->peer_id, x, y, size, LO_ARGS_END);
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

bool OscInterface::is_verbose()
{
    return true; // FIXME
}


} // end of namespace
