FROM manjarolinux/base:latest

WORKDIR /src
USER 0

RUN pacman -Syu --noconfirm clang compiler-rt lld wasi-compiler-rt wasi-libc wasi-libc++  

USER 1000:1000
