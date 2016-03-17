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

namespace blobdetective {

/** Open Sound Control sending and receiving for this app.
 */
class OscInterface 
{
    public:
        OscInterface(
                const std::string &peer_identifier,
                const std::string &send_port,
                const std::string &send_addr); 
        ~OscInterface();
        /**
         * Sends blob position.
         */
        void send_blob_position(float x, float y, float size);
    private:
        std::string peer_id;
        OscSender sender_;
};

} // end of namespace

#endif // __OSC_INTERFACE_H__
