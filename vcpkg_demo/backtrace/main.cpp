#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <boost/stacktrace.hpp>
#include <glog/logging.h>

void dfs(int k, int n)
{
    if  (k > n)
        // std::cout << boost::stacktrace::stacktrace();
        DLOG(INFO) << "trace: \n" << boost::stacktrace::stacktrace();
    else
        dfs(k + 1, n);
}

int main()
{
    dfs(1, 3);
    return 0;
}