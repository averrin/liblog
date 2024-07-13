{
  description = "Basic development environment";

  inputs = { };

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs =
    { self, nixpkgs, flake-utils } @ inputs:
    let overlays = [ ];
    in
    flake-utils.lib.eachDefaultSystem
      (system:
      let pkgs = import nixpkgs { inherit overlays system; };
      in rec
      {
        devShells.default =
          pkgs.mkShell {
            buildInputs = (with pkgs; [
              git
              gnumake
              gcc
              cmake
              just
            ]);
          };
      }
      );
}
