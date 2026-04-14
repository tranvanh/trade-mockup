# Repository Guidelines

## Project Structure & Module Organization
`TradeCore/` contains the core market domain: orders, trades, markets, and the order book. `ClientApplication/src/` and `ServerApplication/src/` hold the two executable entry points and their app-specific logic. `TradeCore.Test/` contains GoogleTest coverage for matching behavior, `TradeCore.Benchmark/` holds Google Benchmark targets, and `TradeCore.Sandbox/` is the place for one-off experiments. Root `CMakeLists.txt` wires all targets together.

## Build, Test, and Development Commands
Configure from the repository root with Ninja Multi-Config:

```bash
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Release
```

Enable optional targets when needed:

```bash
cmake -S . -B build -G "Ninja Multi-Config" -DBUILD_TESTS=ON -DBUILD_BENCHMARK=ON
ctest --test-dir build -C Release --output-on-failure
```

Use `cmake --build build --config Release --target TradeCore.Test` or `TradeCore.Benchmark` to build a single target. Built apps land under `build/<Target>/.../Release/`.

## Coding Style & Naming Conventions
This project uses C++23. Match the existing style: 4-space indentation, braces on the same line, and small translation units with matching `.h`/`.cpp` pairs. Use PascalCase for types and file names (`OrderBook.cpp`), camelCase for functions (`registerOrder`), and the `m` prefix for members (`mOrderQueue`). Keep include blocks compact and consistent with nearby files. No formatter config is checked in, so preserve local conventions instead of reformatting unrelated code.

## Testing Guidelines
Tests use GoogleTest and live in `TradeCore.Test/` with file names like `OrderBook.Test.cpp`. Follow `TEST(Component, Behavior)` naming and write deterministic cases around price priority, FIFO behavior, and partial fills. Run tests through `ctest`; add or update tests for every bug fix in `TradeCore`.

## Commit & Pull Request Guidelines
Recent history uses short, imperative subjects such as `reinit` and `Improve thread safe containers`. Keep commit titles brief, specific, and action-oriented. PRs should state which target(s) changed, note the build config used (`Debug`, `Release`, or `Assert`), and include the exact validation commands run. For client/server behavior changes, include sample CLI output or reproduction steps.

## Environment & Dependencies
Initial CMake configure fetches third-party dependencies with `FetchContent`. Ensure `Boost` is installed locally, Git can reach GitHub, and SSH access works for the `toybox` dependency.
