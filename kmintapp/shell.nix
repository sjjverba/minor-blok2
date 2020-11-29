{ pkgs ? import <nixpkgs> {} }:

with pkgs;

mkShell rec {
  CPATH = lib.makeSearchPathOutput "dev" "include" buildInputs;
  buildInputs = [
    cmake
    ninja
    SDL2
    SDL2_image
    pkgconfig
    pandoc
  ];
}
