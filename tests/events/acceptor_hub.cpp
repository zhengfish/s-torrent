#include <gtest/gtest.h>

#include "wrappers/sockets/client_socket.h"
#include "events/acceptor_hub.h"
#include "events/lambda_receiver.hpp"
#include "events/events.h"
#include "events/connection_receiver.h"
#include "events/hub.h"

#include <memory>
#include <chrono>
#include <thread>

TEST ( AcceptorHubTest, GeneralTest )
{
    bool accepted = true;
    auto acceptor_receiver = std::make_shared < events::Lambda_receiver < std::shared_ptr < events::Event > > > (
            [ & ] ( std::shared_ptr < events::Event > e )
            {
                std::shared_ptr < events::Connection_accepted_event > c_a_event = std::dynamic_pointer_cast < events::Connection_accepted_event > ( e );
                if ( c_a_event->get_service () == "AcceptorHubTest" )
                {
                    accepted = true;
                }
            });
    events::Hub::get_filter ( "Connection_accepted_event" ).subscribe ( acceptor_receiver );
    events::Acceptor_hub::accept ( 12344, "AcceptorHubTest" );

    sockets::Client_socket c ( "127.0.0.1", 12344 );
    std::this_thread::sleep_for ( std::chrono::microseconds ( 500 ) );
    EXPECT_TRUE ( accepted );

    c.shutdown ();
    events::Acceptor_hub::stop ( "AcceptorHubTest" );
}

TEST ( AcceptorHubTest, DoubleCreateTest )
{
    events::Acceptor_hub::accept ( 12345, "AcceptorHubTest.doubleCreateTest" );
    EXPECT_THROW ( events::Acceptor_hub::accept ( 12345, "AcceptorHubTest.doubleCreateTest" ), std::runtime_error );
    std::this_thread::sleep_for ( std::chrono::microseconds ( 500 ) );
    events::Acceptor_hub::stop ( "AcceptorHubTest.doubleCreateTest" );
}
