


add_executable(async_tcp_echo_server
    async_tcp_echo_server.cpp
)
target_link_libraries(async_tcp_echo_server
    PRIVATE 
        asio asio::asio
        glog::glog
        pthread
)


add_executable(blocking_tcp_echo_client
    blocking_tcp_echo_client.cpp
)
target_link_libraries(blocking_tcp_echo_client
    PRIVATE 
        asio asio::asio
        glog::glog
        pthread
)