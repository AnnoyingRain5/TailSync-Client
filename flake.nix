{
  description = "Nix devshells!";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = [
          pkgs.clang-tools
          pkgs.platformio
        ];
        shellHook = "echo \"Want to compile TailSync? Run 'pio run' to compile, or 'pio run -t upload' to compile and upload it to your ESP!\"";
      };
    };
}
