{ system ? builtins.currentSystem }:

let
  pkgs = import <nixpkgs> { inherit system; };

  glad = pkgs.stdenv.mkDerivation {
    pname = "glad";
    version = "2.0.6";

    src = ~/Documents/MGE/.;

    buildPhase = ''
      mkdir -p $out/include/glad
      mkdir -p $out/include/KHR
      mkdir -p $out/include/stb
      mkdir -p $out/include/miniaudio
      mkdir -p $out/lib
      cp ./include/glad/glad.h $out/include/glad/
      cp ./include/KHR/khrplatform.h $out/include/KHR/
      cp ./include/stb/stb_image.h $out/include/stb/
      cp ./include/miniaudio/miniaudio.h $out/include/miniaudio/
      gcc -c glad.c -o $out/lib/glad.o
      ar rcs $out/lib/libglad.a $out/lib/glad.o
    '';
  };
in

pkgs.stdenv.mkDerivation {
  glad = glad;
  pname = "MGEbuild";
  version = "0.0.1";
  src = ~/Documents/MGE/.;
  buildInputs = [ 
    pkgs.libGLU 
    pkgs.glm
    pkgs.glfw
    pkgs.cmake
    pkgs.ninja
    pkgs.libGL
    pkgs.assimp
    pkgs.tinyxml-2
    glad
    pkgs.wayland
    pkgs.wayland-protocols
    pkgs.pkg-config
    pkgs.freetype
  ];
  CXXFLAGS = "-g";
  patchPhase = ''
    '';
  buildPhase = ''
    mkdir -p $TMPDIR/build
    cd $TMPDIR/build
    export LDFLAGS="-L${pkgs.libGL}/lib -L${pkgs.glm}/lib -L${pkgs.glfw}/lib -L${glad}/lib -L${pkgs.wayland}/lib -L${pkgs.assimp}/lib -L${pkgs.tinyxml-2}/lib -L${pkgs.freetype}/lib"
    cmake -G Ninja $src
    ninja -v
    '';
  installPhase = ''
    mkdir -p $out/bin
    cp -r $TMPDIR/build/ $out/bin/
    '';
  fixupPhase = ''
    find $out -type f -exec file {} \; | grep ELF | cut -d: -f1 | while read -r elf; do
      patchelf --shrink-rpath "$elf" || true
    done
  '';
  meta = {
    description = "Mizl Game Engine (MGE)";
    maintainers = with pkgs.lib.maintainers; [ Mizl_Nimbl ];
  };
}