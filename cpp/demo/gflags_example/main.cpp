#include <cstdio>
#include <cstdlib>

#include <gflags/gflags.h>
#include <glog/logging.h>



DEFINE_bool(echo_attachment, true, "Echo attachment as well");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_string(server, "0.0.0.0:8000", "IP Address of server");



int main(int argc, char** argv, char** env)
{
    printf("env:\n");
    for (int i = 0; env[i] != NULL; i++)
        printf("%s\n", env[i]);
    printf("\n");

    printf("arg:\n");
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);

    // Parse gflags. We recommend you to use gflags as well.
    google::ParseCommandLineFlags(&argc, &argv, true);
    LOG(INFO) << "echo_attachment: " << FLAGS_echo_attachment
            << ", timeout_ms: " << FLAGS_timeout_ms
            << ", server: " << FLAGS_server;


    return 0;
}
