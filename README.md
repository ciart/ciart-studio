# 개발 준비
`bazel run @hedron_compile_commands//:refresh_all`

# 빌드
## Windows

1. Install LLVM.

2. `set BAZEL_LLVM=C:\Program Files\LLVM`

3. `bazel build --config=windows //windows:app`

## Mac
1. `bazel run //mac:app`

# 추가 명령

- **Xcode 프로젝트 생성**
`bazel run //mac:xcodeproj`

- **bazel clean**
`bazel clean --expunge`
