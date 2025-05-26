# stdx

`stdx` is a minimalist, modular C99 utility library inspired by the STB style. It provides a set of dependency-free, single-header components that extend common programming functionality in C.

## Key Features

- Modular single-header components.
- Define `STDX_IMPLEMENTATION_<HEADERNAME>` in **one** C file to compile implementation.
- No dependencies; pure C99.
- Components cover memory, data structures, filesystem, networking, threading, logging, and testing.

## Why
1. No dependencies, easy integration
  You just drop in one or more headers. No complicated setup, no linking external libraries. This is perfect for small or embedded projects, quick prototypes, or when you want maximum control over your build.

2. Modular and selective
  Use only what you need. Don’t pay a cost in compile time or binary size for features you don’t use. Each header is independent and can be compiled on-demand by defining the implementation macro.

3. Portable C99 code
  Written in plain C99, making it highly portable across platforms and compilers—even older or niche ones.

4. Reusable common utilities
  It provides safe, tested implementations of common patterns like dynamic arrays, hash tables, string builders, arenas, logging, threading, and networking helpers. Instead of reinventing these, you get solid building blocks.

5. STB-style simplicity
  No build systems or package managers required. Just include the header, define the implementation once, and compile. This style is very appealing for C programmers who want straightforward, easy-to-maintain code.

6. Encourages clean architecture
  Since each component is standalone, you can keep your codebase clean, include only what you need, and avoid unnecessary coupling.


## Modules

| Header                  | Description                                       |
|-------------------------|-------------------------------------------------|
| `stdx_allocator.h`      | Custom memory allocator interfaces and helpers. |
| `stdx_arena.h`          | Arena allocator for efficient bulk memory use.  |
| `stdx_array.h`          | Dynamic array implementation.                    |
| `stdx_common.h`         | Common macros, types, and utilities.             |
| `stdx_filesystem.h`     | Cross-platform filesystem operations.           |
| `stdx_hashtable.h`      | Generic hashtable (hash map) implementation.    |
| `stdx_log.h`            | Simple logging utilities.                         |
| `stdx_network.h`        | Basic networking and socket utilities.           |
| `stdx_string.h`         | String manipulation helpers.                      |
| `stdx_stringbuilder.h`  | Dynamic string builder for efficient concatenation. |
| `stdx_test.h`           | Minimalist unit testing framework.                |
| `stdx_thread.h`         | Thread and synchronization abstractions.          |

## Usage

Include the header in your source files as needed:

```c
#include "stdx_array.h"
#include "stdx_log.h"
```

In exactly one `.c` file, define the implementation macro before including the header to compile the implementation:

```c
#define STDX_IMPLEMENTATION_ARRAY
#include "stdx_array.h"
```

Repeat for each module you use by defining the corresponding `STDX_IMPLEMENTATION_<HEADERNAME>` macro once.

## Example

```c
// main.c
#define STDX_IMPLEMENTATION_ARRAY
#include "stdx_array.h"

#define STDX_IMPLEMENTATION_LOG
#include "stdx_log.h"

int main() {
    stdx_log_info("Starting program...");
    // Use stdx_array functions here
    return 0;
}
```


## License

MIT License. See the `LICENSE` file.
