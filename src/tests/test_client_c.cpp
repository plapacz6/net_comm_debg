/*
Copyright 2024-2024 plapacz6@gmail.com

This file is part of net_comm_dbg.

net_comm_dbg is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

net_comm_dbg is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
 with net_comm_dbg. If not, see <https://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>
#include "../client_c.h"
// #define _GNU_SOURCE
#include <thread>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cassert>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
// #include <sys/select.h>
#include <poll.h>
#include <sys/time.h>

using namespace std;
using namespace testing;

#include "Ttest_server_for_one_connection.h"



Ttest_server& test_server = Ttest_server::create_test_server();

class net_comm_dbg_client_c_fixture_01 : public Test
{
    void SetUp() override
    {        
        thread server_thread = thread(&Ttest_server::run_test_server, &test_server);
        server_thread.detach();        
        sleep(1);
    }    
    void TearDown() override
    {   
        if(test_server.server_accepted_fd != -1) {
            test_server.close_connection();
            sleep(1);
        }        
        test_server.stop_test_server();
        fflush(stdout);
        while(test_server.server_state != Ttest_server::sv_state::SV_STOPPED){            
        }
    }
};

TEST_F(net_comm_dbg_client_c_fixture_01, test_01) 
{   
    // ASSERT_EQ(Ttest_server::sv_state::SV_WAIT_FOR_CONNECTION, test_server.server_state); 
    int client_fd = connect_to_server(
        const_cast<char*>(test_server.server_ip_address), test_server.server_port);
    sleep(1);
    // ASSERT_EQ(Ttest_server::sv_state::SV_CONNECTED, test_server.server_state);
    sleep(1);
    ASSERT_NE(client_fd, -1);  //error
    ASSERT_NE(client_fd, 0);  //stdin
    ASSERT_NE(client_fd, 1);  //stdout
    ASSERT_NE(client_fd, 2);  //stderr    
    // ASSERT_EQ(Ttest_server::conn_state::CONN_POSSIBLE_WR_RD, test_server.connection_state);
    close(client_fd);
    sleep(1);
    // ASSERT_EQ(Ttest_server::conn_state::CONN_RDHUP_AND_WR_RD, test_server.connection_state);
    // ASSERT_EQ(Ttest_server::sv_state::SV_CONNECTED, test_server.server_state);
    test_server.close_connection();
    sleep(1);
    client_fd = connect_to_server(
        const_cast<char*>(test_server.server_ip_address), test_server.server_port);                
    sleep(1);
    ASSERT_NE(client_fd, -1);  //error
    ASSERT_NE(client_fd, 0);  //stdin
    ASSERT_NE(client_fd, 1);  //stdout
    ASSERT_NE(client_fd, 2);  //stderr    
        
    test_server.stop_test_server();
    // ASSERT_EQ(Ttest_server::sv_state::SV_WAIT_FOR_CONNECTION, test_server.server_state); 
}
