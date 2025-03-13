cc_binary(
    name = "CiartStudio",
    srcs = [
        "main.cpp",
    ],
    deps = ["//ciart_studio/core"],
    copts = ["/std:c++17"],
    linkopts = [
        "-SUBSYSTEM:WINDOWS",
        "User32.lib",
    ],
)
