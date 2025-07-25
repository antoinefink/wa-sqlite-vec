# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and Development Commands

### Build
- `make` - Default build producing ES6 modules + WASM (synchronous and asynchronous builds)
- `make dist` - Build production artifacts in `dist/`
- `make debug` - Build debug artifacts in `debug/`
- `make clean` - Remove dist, debug, and tmp directories
- `make spotless` - Complete clean including dependencies and cache

### Test
- `yarn test` - Run the full test suite using web-test-runner
- `yarn test-manual` - Run tests interactively

### Development
- `yarn start` - Start development server on http://localhost:8000
- `yarn build-docs` - Generate TypeScript documentation using TypeDoc

## High-level Architecture

This is a WebAssembly build of SQLite with JavaScript bindings that enables SQLite usage in web browsers. The architecture consists of:

### Core Components
- **SQLite WASM Core**: Multiple build variants providing different async capabilities
  - Synchronous build (`wa-sqlite.mjs`)
  - Asynchronous build using Asyncify (`wa-sqlite-async.mjs`)
  - JSPI build (`wa-sqlite-jspi.mjs`)

- **Virtual File System (VFS) Layer**: Allows implementing SQLite storage backends entirely in JavaScript
  - Base VFS class in `/src/VFS.js`
  - Facade pattern implementation in `/src/FacadeVFS.js`
  - Multiple VFS implementations in `/src/examples/` for different storage backends

- **JavaScript API Layer**: 
  - `/src/sqlite-api.js` - Main JavaScript API wrapper
  - `/src/sqlite-constants.js` - SQLite constants exposed to JavaScript
  - `/src/lib*.js` - JavaScript bindings for SQLite callbacks
  - `/src/lib*.c` - C implementations for JavaScript callbacks

### VFS Implementations
The project provides multiple VFS implementations for different use cases:
- **Memory-based**: MemoryVFS, MemoryAsyncVFS
- **IndexedDB-based**: IDBBatchAtomicVFS, IDBMirrorVFS
- **OPFS-based**: AccessHandlePoolVFS, OPFSAdaptiveVFS, OPFSAnyContextVFS, OPFSCoopSyncVFS
- **Hybrid**: OPFSPermutedVFS (OPFS + IndexedDB)

Each VFS has different trade-offs for performance, persistence, and concurrency. See `/src/examples/README.md` for detailed comparisons.

### Build System
The build system uses Emscripten to compile SQLite to WebAssembly. Key aspects:
- Requires Emscripten SDK 3.1.61 or newer
- Downloads SQLite 3.50.1 during build
- Builds multiple WASM variants with different async capabilities
- Uses custom JavaScript bindings for SQLite callbacks and VFS operations

### Testing Architecture
- Tests located in `/test/` directory
- Uses Jasmine testing framework with web-test-runner
- Tests cover both API functionality and VFS implementations
- Can be run in automated or manual/interactive mode