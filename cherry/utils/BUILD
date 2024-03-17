cc_library(
    name = "time",
    hdrs = ["time_my.h"],
    srcs = ["time_my.cpp"],
    # 默认只有一个build文件中的目标能互相看见
    visibility = ["//visibility:public"],
)

cc_library(
    name = "logging",
    hdrs = ["logging.h","logging_backend.h","noncopyable.h"],
    srcs = ["logging.cpp","logging_backend.cpp"],
    deps = ["time"],
    # 默认只有一个build文件中的目标能互相看见
    visibility = ["//visibility:public"],
)

cc_test(
    name = "time_test",
    srcs = ["time_my_test.cpp"],
    deps = [":time"],
)

cc_test(
    name = "logging_test",
    srcs = ["logging_test.cpp"],
    deps = [":logging"],
)
