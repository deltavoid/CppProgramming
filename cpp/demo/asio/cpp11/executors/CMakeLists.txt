
#actor 
add_executable(actor
    actor.cpp
)
target_include_directories(actor
    PRIVATE
        .
)
target_link_libraries(actor
    PRIVATE 
        asio asio::asio
        glog::glog
        pthread
)

# add_executable(chat_client
#     chat_client.cpp
# )
# target_include_directories(chat_server
#     PRIVATE
#         .
# )
# target_link_libraries(chat_client
#     PRIVATE 
#         asio asio::asio
#         glog::glog
#         pthread
# )