{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "fs-indexer-env";

  buildInputs = with pkgs; [
    gcc
    cmake
    ninja
    gdb
    clang-tools
    perf
  ];
}