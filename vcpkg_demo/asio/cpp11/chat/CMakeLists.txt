

add_executable(chat_server
    chat_server.cpp
)
target_include_directories(chat_server
    PRIVATE
        .
)
target_link_libraries(chat_server
    PRIVATE 
        asio asio::asio
        glog::glog
        pthread
)

add_executable(chat_client
    chat_client.cpp
)
target_include_directories(chat_server
    PRIVATE
        .
)
target_link_libraries(chat_client
    PRIVATE 
        asio asio::asio
        glog::glog
        pthread
)