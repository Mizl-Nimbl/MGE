#!/bin/sh

nix-collect-garbage

export LD_LIBRARY_PATH=$(nix-build --no-out-link -E 'with import <nixpkgs> {}; wayland')/lib:$(nix-build --no-out-link -E 'with import <nixpkgs> {}; libGL')/lib:$(nix-build --no-out-link -E 'with import <nixpkgs> {}; glfw')/lib:$(nix-build --no-out-link -E 'with import <nixpkgs> {}; glm')/lib:$(nix-build --no-out-link -E 'with import <nixpkgs> {}; assimp')/lib:$(nix-build --no-out-link -A glad ./default.nix)/lib:$(nix-build --no-out-link -E 'with import <nixpkgs> {}; freetype')/lib:$LD_LIBRARY_PATH

./result/bin/build/MGE