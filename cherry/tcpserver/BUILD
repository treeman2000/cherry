cc_library(
    name = "tcpserver",
    srcs = [
        "acceptor.cpp",
        "tcp_connection.cpp",
        "tcpserver.cpp",
    ],
    hdrs = [
        "acceptor.h",
        "callbacks.h",
        "tcp_connection.h",
        "tcpserver.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//cherry/net",
        "//cherry/utils:logging",
    ],
)

cc_test(
    name = "acceptor_test",
    srcs = ["acceptor_test.cpp"],
    deps = [
        ":tcpserver",
        "//cherry/utils:logging",
    ],
)

cc_test(
    name = "tcp_test",
    srcs = ["tcp_test.cpp"],
    deps = [
        ":tcpserver",
        "//cherry/utils:logging",
    ],
)
