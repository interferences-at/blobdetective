/*
 * Toonloop
 *
 * Copyright (c) 2010 Alexandre Quessy <alexandre@quessy.net>
 * Copyright (c) 2010 Tristan Matthews <le.businessman@gmail.com>
 *
 * Toonloop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Toonloop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the gnu general public license
 * along with Toonloop.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __OSC_INTERFACE_H__
#define __OSC_INTERFACE_H__

#include <string>
#include "oscsender.h"
#include "oscreceiver.h"

namespace blobdetective {

class Application; // forward declaration

/** Open Sound Control sending and receiving for this app.
 */
class OscInterface 
{
    public:
        OscInterface(
                Application* application,
                int peer_identifier,
                const std::string &send_port,
                const std::string &send_addr,
                const std::string &receive_port
                ); 
        ~OscInterface();
        bool is_verbose();
        void start_receiver();
        Application* get_application();
        /**
         * Sends blob position.
         */
        void send_blob_position(float x, float y, float size);
    private:
        Application* application_;
        int peer_id;
        OscSender sender_;
        bool send_enabled_;
        bool receiving_enabled_;
        OscReceiver receiver_;
        static int option_int_cb(const char *path, 
                const char *types, lo_arg **argv, 
                int argc, void *data, void *user_data);
        static int option_string_cb(const char *path, 
                const char *types, lo_arg **argv, 
                int argc, void *data, void *user_data);
        static int option_float_cb(const char *path, 
                const char *types, lo_arg **argv, 
                int argc, void *data, void *user_data);
        static int option_boolean_cb(const char *path, 
                const char *types, lo_arg **argv, 
                int argc, void *data, void *user_data);
        static int ping_cb(const char *path, 
                const char *types, lo_arg **argv, 
                int argc, void *data, void *user_data);
        void receive_int_option_cb(const char* name, int value);
        void receive_float_option_cb(const char* name, float value);
        void receive_boolean_option_cb(const char* name, bool value);
        void receive_string_option_cb(const char* name, const char* value);
};

} // end of namespace

#endif // __OSC_INTERFACE_H__
