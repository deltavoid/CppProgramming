
ADDR := 127.0.0.1
PORT := 8192

.PHONY: 
default: run_server_epoll




run_client_epoll_bench: client_epoll_bench
	numactl --cpubind=1 --membind=1 ./$< $(ADDR) $(PORT) 1 4 64 1000

run_client_epoll_verify: client_epoll_verify
	numactl --cpubind=1 --membind=1 ./$< $(ADDR) $(PORT) 1 1 64 1000

run_client_test_epollin: client_test_epollin
	./$< $(ADDR) $(PORT)

run_client_test_epollout: client_test_epollout
	./$< $(ADDR) $(PORT)

run_client: client
	./$< $(ADDR) $(PORT) 1 3





run_server_epoll_bench: server_epoll_bench
	numactl --cpubind=0 --membind=0 ./$< $(PORT) 2

run_server_epoll_handle_c: server_epoll_handle_c
	./$< $(PORT)

run_server_epoll_handle: server_epoll_handle
	./$< $(PORT)

run_server_epoll_reuseport: server_epoll_reuseport
	./$< $(PORT) 2

run_server_epoll: server_epoll
	./$< $(PORT)




run_server_epollet_epollout_handle: server_epollet_epollout_handle
	./$< $(PORT)

run_server_epollet_epollout_response: server_epollet_epollout_response
	./$< $(PORT)


run_server_epollet_ref: server_epollet_ref
	./$< 

run_server_epollet_simple: server_epollet_simple
	./$< $(PORT)



run_server_epollout_epollin_handle_native: server_epollout_epollin_handle_native
	./$< $(PORT)

run_server_epollout_epollin_handle: server_epollout_epollin_handle
	./$< $(PORT)

run_server_epollout_handle_native: server_epollout_handle_native
	./$< $(PORT)

run_server_epollout_handle: server_epollout_handle
	./$< $(PORT)

run_server_epollout_ref: server_epollout_ref
	./$< $(PORT)

run_server_epollout_response: server_epollout_response
	./$< $(PORT)


run_server_poll_handle: server_poll_handle
	./$< $(PORT)

run_server: server
	./$< $(PORT)







client_epoll_bench: client_epoll_bench.cpp util.h util.cpp
	g++ -o $@ $^ -pthread

client_epoll_verify: client_epoll_verify.cpp util.h util.cpp
	g++ -o $@ $^ -pthread

client_test_epollin: client_test_epollin.cpp util.h util.cpp
	g++ -o $@ $^

client_test_epollout: client_test_epollout.cpp util.h util.cpp
	g++ -o $@ $^

client: client.cpp util.h util.cpp
	g++ -g -fno-omit-frame-pointer -o $@ $^ -pthread






server_epoll_bench: server_epoll_bench.cpp util.h util.cpp
	g++ -o $@ $^ -pthread

server_epoll_handle_c: server_epoll_handle.c
	g++ -o $@ $^ -pthread

server_epoll_handle: server_epoll_handle.cpp
	g++ -o $@ $^ -pthread

server_epoll_reuseport: server_epoll_reuseport.cpp util.h util.cpp
	g++ -o $@ $^ -pthread

server_epoll: server_epoll.cpp util.h util.cpp
	g++ -o $@ $^ -pthread





server_epollet_epollout_handle: server_epollet_epollout_handle.cpp
	g++ -o $@ $^ -pthread

server_epollet_epollout_response: server_epollet_epollout_response.cpp
	g++ -o $@ $^ -pthread

server_epollet_ref: server_epollet_ref.cpp
	g++ -o $@ $^ -pthread

server_epollet_simple: server_epollet_simple.cpp
	g++ -o $@ $^ -pthread



server_epollout_epollin_handle_native: server_epollout_epollin_handle_native.cpp
	g++ -o $@ $^ -pthread

server_epollout_epollin_handle: server_epollout_epollin_handle.cpp
	g++ -o $@ $^ -pthread

server_epollout_handle_native: server_epollout_handle_native.cpp
	g++ -o $@ $^ -pthread

server_epollout_handle: server_epollout_handle.cpp
	g++ -o $@ $^ -pthread

server_epollout_ref: server_epollout_ref.cpp
	g++ -o $@ $^ -pthread

server_epollout_response: server_epollout_response.cpp
	g++ -o $@ $^ -pthread



server_poll_handle: server_poll_handle.cpp
	g++ -o $@ $^ -pthread


server: server.cpp util.h util.cpp
	g++ -o $@ $^ -pthread




server_epoll_lt_recvmsg: server_epoll_lt_recvmsg.cpp
	g++ -o $@ $^ -pthread


server_epoll_lt_recv: server_epoll_lt_recv.cpp
	g++ -o $@ $^ -pthread




clean:
# 	-rm client client_epoll_bench server server_epoll server_epoll_bench
	rm `cat .gitignore` -f 