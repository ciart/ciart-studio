# 개발 준비
`bazel run @hedron_compile_commands//:refresh_all`

# 빌드
## Windows

1. Install LLVM.

2. `set BAZEL_LLVM=C:\Program Files\LLVM`

3. `bazel build --config=windows //main:hello-world`

## Mac
1. `bazel run //ciart_studio/mac:CiartStudio`
