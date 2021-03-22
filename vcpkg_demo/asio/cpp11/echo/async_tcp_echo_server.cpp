//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <glog/logging.h>

using asio::ip::tcp;

class session
    : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
        : socket_(std::move(socket))
    {
        DLOG(INFO) << "session::ctor";
    }

    ~session()
    {
        DLOG(INFO) << "session::dtor";
    }

    void start()
    {
        DLOG(INFO) << "session::start: 1";
        do_read();
        DLOG(INFO) << "session::start: 2";
    }

private:
    void do_read()
    {
        DLOG(INFO) << "session::do_read: 1";
        
        auto self(shared_from_this());
        
        socket_.async_read_some(asio::buffer(data_, max_length),
                [this, self](std::error_code ec, std::size_t length) {
                    DLOG(INFO) << "session::do_read: 2, ec: " << ec;
                                    
                    if (!ec) {
                        do_write(length);
                    }
                                
                    DLOG(INFO) << "session::do_read: 3"; 
                }
        );

        DLOG(INFO) << "session::do_read: 4";
    }

    void do_write(std::size_t length)
    {
        DLOG(INFO) << "session::do_write: 1";
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(data_, length),          

                [this, self](std::error_code ec, std::size_t /*length*/) {
                    DLOG(INFO) << "session::do_write: 2, ec: " << ec;
                    if (!ec) {
                        do_read();
                    }
                    DLOG(INFO) << "session::do_write: 3";
                }
        );
        DLOG(INFO) << "session::do_write: 4";
    }

    tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};

class server
{
public:
    server(asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          socket_(io_context)
    {
        DLOG(INFO) << "server::ctor: 1";
        do_accept();
        DLOG(INFO) << "server::ctor: 2";
    }

private:
    void do_accept()
    {
        DLOG(INFO) << "server::do_accept: 1";

        acceptor_.async_accept(socket_,
                
                [this](std::error_code ec) {
                                
                    DLOG(INFO) << "server::do_accept: 2, ec: " << ec;      
                    if (!ec) {
                        LOG(INFO) << "establish a connection";
                        std::make_shared<session>(std::move(socket_))->start();
                    }

                    DLOG(INFO) << "server::do_accept: 3";      
                    do_accept();

                    DLOG(INFO) << "server::do_accept: 4";
                }
        );

        DLOG(INFO) << "server::do_accept: 5";
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main(int argc, char *argv[])
{
    DLOG(INFO) << "main: 1";
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        DLOG(INFO) << "main: 2";
        asio::io_context io_context;

        DLOG(INFO) << "main: 3";
        server s(io_context, std::atoi(argv[1]));

        DLOG(INFO) << "main: 4";
        io_context.run();

        DLOG(INFO) << "main: 5";
    }
    catch (std::exception &e)
    {
        DLOG(INFO) << "main: 6";
        std::cerr << "Exception: " << e.what() << "\n";
    }

    DLOG(INFO) << "main: 7";
    return 0;
}
