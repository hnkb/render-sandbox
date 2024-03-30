# render-sandbox

To set up:

1. Manually install bun and C++/WASM tooling:
    - [Bun](https://bun.sh) (or `curl -fsSL https://bun.sh/install | bash`)
    - [CMake](https://cmake.org/download/)
    - [Ninja](https://ninja-build.org) (or `brew install ninja`)
    - [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) (or `brew install emscripten`)

2. Install Javascript dependencies:

    ```bash
    bun install
    ```

To run:

```bash
bun run build-cpp
bun run dev
```
