/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Metrological
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

#include "Module.h"

#include <helloworld.h>

#include <interfaces/IHelloWorld.h>
//#include <interfaces/json/JHelloWorld.h>
#include <plugins/Types.h>

namespace Thunder{
class HelloWorld : protected RPC::SmartInterfaceType<Exchange::IHelloWorld> {
private:
    using BaseClass = Thunder::RPC::SmartInterfaceType<Thunder::Exchange::IHelloWorld>;
    using DisplayOutputUpdatedCallbacks = std::map<helloworld_display_output_change_cb, void*>;

    HelloWorld(const string& callsign)
        : BaseClass(),
        _helloWorldInterface(nullptr),
        _notification(this)
    {
        ASSERT(_singleton==nullptr);
        _singleton=this;
        BaseClass::Open(RPC::CommunicationTimeOut, BaseClass::Connector(), "HelloWorld");
    }
    

public:
    HelloWorld() = delete;
    HelloWorld(const HelloWorld&) = delete;
    HelloWorld& operator=(const HelloWorld&) = delete;

private:

   void DisplayOutputUpdated(const string &word)
    {

        //_helloWorldInterface -> PrintStuff(word);
        std::cout << word << std::endl;
    }

    class Notification : public Exchange::IHelloWorld::INotification {
        public:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

            Notification(HelloWorld* parent)
                : _parent(*parent)
            {

                ASSERT(parent != nullptr);
            }

            void Printed(const string &word) const override
            {
                std::cout << "called printed in notif class" << std::endl;
               _parent.DisplayOutputUpdated(word);
            }
        
            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(Exchange::IHelloWorld::INotification)
            END_INTERFACE_MAP

        private:
            HelloWorld& _parent;
        };
    static HelloWorld* _singleton;
    mutable Core::CriticalSection _lock;
    Exchange::IHelloWorld* _helloWorldInterface;
    //OperationalStateChangeCallbacks _operationalStateCallbacks;
    Core::SinkType<Notification> _notification;
    std::string _callsign;

    //DisplayOutputUpdatedCallbacks _displayChangeCallbacks;

public:
    ~HelloWorld() override
    {
        BaseClass::Close(Core::infinite);
        ASSERT(_singleton!=nullptr);
        _singleton = nullptr;
    }

    static HelloWorld& Instance()
    {
        static HelloWorld* instance = new HelloWorld("HelloWorld");
        ASSERT(instance!=nullptr);
        return *instance;
    }

    static void Dispose()
    {
        ASSERT(_singleton != nullptr);
        if(_singleton != nullptr)
        {
            delete _singleton;
        }
    }

    void Operational(const bool upAndRunning) override
    {
        std::cout << "Called operational func";
        _lock.Lock();

        if (upAndRunning) {
            if (_helloWorldInterface == nullptr) {
                _helloWorldInterface = BaseClass::Interface();

                if (_helloWorldInterface!= nullptr) {
                    _helloWorldInterface->Register(&_notification);
                    _helloWorldInterface->QueryInterface<Exchange::IHelloWorld>();
                }

            }
        } else {
            if (_helloWorldInterface != nullptr) {
                _helloWorldInterface->Unregister(&_notification);
                _helloWorldInterface->Release();
                _helloWorldInterface = nullptr;
            }
        }

        _lock.Unlock();
    }


     const string& Name() const
    {
        return _callsign;
    }


    uint32_t HelloWorld_print_stuff(const string &words)
    {
        _lock.Lock();
        
        uint32_t result = Core::ERROR_NONE;
        if (_helloWorldInterface!= nullptr) {
            result = _helloWorldInterface -> PrintStuff(words);
    }
    _lock.Unlock();
   // std::cout << words;

    return result;
    }
};
HelloWorld* HelloWorld::_singleton = nullptr;
}

using namespace Thunder;
extern "C" {
/*
uint32_t playerinfo_register_operational_state_change_callback(
    playerinfo_operational_state_change_cb callback,
    void* userdata)
{
    return PlayerInfo::Instance().RegisterOperationalStateChangedCallback(callback, userdata);
}*/

uint32_t helloworld_print_stuff(const char* buf)
{
    string stringBuf(buf);
    return HelloWorld::Instance().HelloWorld_print_stuff(stringBuf);
}

void helloworld_dispose() {
    HelloWorld::Dispose();
}
}