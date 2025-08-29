# LibArena: A Lightweight C Memory Arena Library

LibArena is a small and efficient C library for memory management using the arena allocation pattern. It is designed to simplify memory tracking, prevent common errors like memory leaks, and provide fine-grained control over allocation lifetimes.

This library is ideal for applications that allocate and deallocate memory in batches, such as compilers, game engines, and other systems where performance and memory control are critical.

## Features

- **Arena-Based Allocation**: Allocate memory blocks that are tracked by a central collector.
- **Automatic Cleanup**: Free all tracked allocations with a single call.
- **Custom Finalizers**: Register custom cleanup functions for specific allocations.
- **Scoped Allocations**: Create allocation checkpoints and roll back to a previous state.
- **Safe Reallocation**: `arena_realloc` handles pointer updates within the arena.
- **Dynamic Growth**: The arena automatically expands its tracking capacity as needed.

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc` or `clang`)
- `make` for building the library

### Building the Library

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/your-username/libarena.git
    cd libarena
    ```

2.  **Build the static library**:
    ```bash
    make
    ```
    This will create the static library file `libarena.a`.

## Usage

To use LibArena in your project, include the `lib_arena.h` header and link against the `libarena.a` library.

### Example

Here is a simple example demonstrating how to use the core features of the library:

```c
#include <stdio.h>
#include "lib_arena.h"

// A custom finalizer function
void custom_free(void *data) {
    printf("Finalizer called for: %s\n", (char *)data);
}

int main() {
    // 1. Create an arena with an initial capacity of 10
    Collector *arena = arena_create(10);
    if (!arena) {
        return 1;
    }

    // 2. Allocate memory
    char *str1 = arena_alloc(arena, sizeof(char) * 12);
    strcpy(str1, "Hello");

    // 3. Allocate with a custom finalizer
    char *str2 = arena_alloc(arena, sizeof(char) * 12);
    strcpy(str2, "World");
    arena_set_destructor(arena, str2, custom_free);

    printf("Allocated: %s, %s\n", str1, str2);

    // 4. Free all memory in the arena
    arena_destroy(arena);

    return 0;
}
```

### Compiling the Example

To compile this example, save it as `example.c` and run:

```bash
gcc example.c -L. -larena -I./include -o example
./example
```

## API Reference

### Core Functions

| Function                 | Description                                                                 |
| ------------------------ | --------------------------------------------------------------------------- |
| `arena_create(capacity)` | Creates a new memory arena.                                                 |
| `arena_alloc(arena, size)` | Allocates a block of memory in the arena.                                   |
| `arena_realloc(arena, ptr, new_size)` | Reallocates a tracked memory block.                                         |
| `arena_free(arena, ptr)`   | Frees a single tracked allocation.                                          |
| `arena_destroy(arena)`   | Frees all memory in the arena and destroys it.                              |

### Advanced Functions

| Function                           | Description                                                              |
| ---------------------------------- | ------------------------------------------------------------------------ |
| `arena_set_destructor(arena, ptr, fn)` | Registers a custom cleanup function for an allocation.                   |
| `arena_snapshot(arena)`            | Creates a checkpoint of the current allocation state.                    |
| `arena_restore(arena, checkpoint)` | Frees all allocations made after a checkpoint.                           |

For detailed information, see the Doxygen-style comments in `include/lib_arena.h`.

## Building and Testing

This project uses a `Makefile` with several targets:

- `make all` or `make`: Compiles the static library `libarena.a`.
- `make clean`: Removes temporary object files.
- `make fclean`: Removes object files and the static library.
- `make re`: Rebuilds the project from scratch.
- `make debug`: Builds the library with debug symbols and address sanitizer flags.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
