/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "SecurityContext.h"

namespace WPEFramework {
namespace Plugin {

    SecurityContext::SecurityContext(const AccessControlList* acl, const uint16_t length, const uint8_t payload[])
        : _token(string(reinterpret_cast<const TCHAR*>(payload), length))
        , _accessControlList(nullptr)
    {
        _context.FromString(_token);

        if ( (_context.URL.IsSet() == true) && (acl != nullptr) ) {
            _accessControlList = acl->FilterMapFromURL(_context.URL.Value());
        }
    }

    /* virtual */ SecurityContext::~SecurityContext()
    {
    }

    //! Allow a websocket upgrade to be checked if it is allowed to be opened.
    bool SecurityContext::Allowed(const string&) const /* override */
    {
        return (true);
    }

    //! Allow a request to be checked before it is offered for processing.
    bool SecurityContext::Allowed(const Web::Request&) const /* override */
    {
        bool allowed = (_accessControlList != nullptr);

        if (allowed == true) {
        }

        return (allowed);
    }

    //! Allow a JSONRPC message to be checked before it is offered for processing.
    bool SecurityContext::Allowed(const Core::JSONRPC::Message& message) const /* override */ 
    {
        return ((_accessControlList != nullptr) && (_accessControlList->Allowed(message.Callsign(), message.Method())));
    }

    string SecurityContext::Token() const /* override */
    {
        return (_token);
    }

}
}
