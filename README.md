# LibArena

**A lightweight C memory management library with garbage-collector-like functionality**

LibArena simplifies memory management in C by providing automatic tracking, scoped cleanup, and custom finalizers. Designed for reliability and ease of use, it prevents memory leaks while maintaining the control C developers expect.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![42 School](https://img.shields.io/badge/42-School-000000.svg)](https://42.fr/)

---

## ✨ Features

- **📊 Tracked Allocations**: Every allocation is monitored and can be freed automatically
- **🔄 Dynamic Growth**: Internal arrays grow automatically as needed
- **🧹 Custom Finalizers**: Register cleanup functions for complex data structures  
- **🎯 Scoped Memory**: Use checkpoints to create temporary allocation scopes
- **🛡️ Memory Safety**: Prevents leaks with automatic cleanup on collector end
- **📏 Realloc Support**: Safely resize tracked allocations without losing tracking
- **🎓 42-Compliant**: Follows strict coding standards and modular file structure

---

## 🚀 Quick Start

### Installation

```bash
git clone https://github.com/yourusername/LibArena.git
cd LibArena
make
```

### Basic Example

```c
#include "libarena.h"

int main() {
    // Initialize collector with capacity for 10 pointers
    Collector *collector = collector_init(10);
    
    // Allocate tracked memory
    int *numbers = collector_alloc(collector, sizeof(int) * 5);
    char *buffer = collector_alloc(collector, 256);
    
    // Use your memory...
    for (int i = 0; i < 5; i++) {
        numbers[i] = i * i;
    }
    
    // Everything is freed automatically
    collector_end(collector);
    return 0;
}
```

---

## 📚 API Reference

### Core Functions

#### `Collector *collector_init(size_t initial_capacity)`
Initialize a new collector with the specified initial capacity.
```c
Collector *c = collector_init(100);
```

#### `void *collector_alloc(Collector *c, size_t size)`
Allocate tracked memory of the specified size.
```c
int *array = collector_alloc(c, sizeof(int) * 10);
```

#### `void *collector_realloc(Collector *c, void *ptr, size_t new_size)`
Resize a tracked allocation while maintaining tracking.
```c
array = collector_realloc(c, array, sizeof(int) * 20);
```

#### `void collector_free_ptr(Collector *c, void *ptr)`
Free a single tracked pointer immediately.
```c
collector_free_ptr(c, array);
```

#### `void collector_end(Collector *c)`
Free all remaining allocations and destroy the collector.
```c
collector_end(c);  // Everything is cleaned up
```

### Advanced Features

#### Custom Finalizers
Register cleanup functions for complex data structures:

```c
typedef struct {
    char *name;
    int *values;
} MyStruct;

void cleanup_struct(void *ptr) {
    MyStruct *s = (MyStruct *)ptr;
    // Clean up nested allocations
    // Note: Don't free 's' itself - LibArena handles that
    if (s->name) free(s->name);
    if (s->values) free(s->values);
}

// Usage
MyStruct *obj = collector_alloc(c, sizeof(MyStruct));
collector_register_finalizer(c, obj, cleanup_struct);
```

#### Scoped Memory Management
Use checkpoints for temporary allocations:

```c
// Create checkpoint
size_t checkpoint = collector_checkpoint(c);

// Allocate temporary memory
char *temp_buffer = collector_alloc(c, 1024);
int *temp_array = collector_alloc(c, sizeof(int) * 100);

// Do work with temporary memory...

// Free everything allocated after checkpoint
collector_scope_end(c, checkpoint);
// temp_buffer and temp_array are now freed
// Other allocations remain untouched
```

---

## 🏗️ Project Structure

```
LibArena/
├── include/
│   └── libarena.h              # Public API header
├── src/
│   ├── collector_init.c        # Initialization and cleanup
│   ├── collector_alloc.c       # Memory allocation functions
│   ├── collector_realloc.c     # Reallocation support
│   ├── collector_finalizer.c   # Finalizer management
│   ├── collector_scope.c       # Checkpoint and scoping
│   └── collector_end.c         # Cleanup and destruction
├── tests/
│   ├── test_basic.c           # Basic functionality tests
│   ├── test_advanced.c        # Advanced features tests
│   └── Makefile               # Test compilation
├── examples/
│   ├── basic_usage.c          # Simple usage examples
│   └── advanced_features.c    # Complex usage patterns
├── Makefile                   # Library compilation
├── .gitignore
└── README.md
```

---

## 🔧 Building and Testing

### Build Library
```bash
make                    # Build libarena.a
make clean             # Clean build files
make fclean            # Deep clean including library
make re                # Rebuild everything
```

### Run Tests
```bash
cd tests
make                   # Build all tests
./test_basic          # Run basic functionality tests  
./test_advanced       # Run advanced feature tests
make clean            # Clean test files
```

### Integration with Your Project
```bash
# Compile with LibArena
gcc -I./include -L. -larena your_program.c -o your_program

# Or include in your Makefile
CFLAGS += -I./path/to/LibArena/include
LDFLAGS += -L./path/to/LibArena -larena
```

---

## 💡 Usage Patterns

### Pattern 1: Simple Resource Management
```c
Collector *c = collector_init(10);
FILE *file = fopen("data.txt", "r");
char *buffer = collector_alloc(c, 4096);

// Work with file and buffer...

fclose(file);
collector_end(c);  // buffer freed automatically
```

### Pattern 2: Nested Data Structures
```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;

void cleanup_node(void *ptr) {
    Node *node = (Node *)ptr;
    // LibArena will free the node itself
    // Just handle any nested non-tracked allocations
}

Node *create_list(Collector *c, int size) {
    Node *head = NULL;
    for (int i = 0; i < size; i++) {
        Node *new_node = collector_alloc(c, sizeof(Node));
        collector_register_finalizer(c, new_node, cleanup_node);
        new_node->value = i;
        new_node->next = head;
        head = new_node;
    }
    return head;
}
```

### Pattern 3: Function-Scoped Cleanup
```c
void process_data(Collector *c, const char *input) {
    size_t checkpoint = collector_checkpoint(c);
    
    // Temporary allocations for this function
    char *working_buffer = collector_alloc(c, strlen(input) * 2);
    int *temp_array = collector_alloc(c, sizeof(int) * 1000);
    
    // Process data...
    
    collector_scope_end(c, checkpoint);  // Clean up temporaries
}
```

---

## ⚠️ Important Notes

- **Finalizers** should NOT free the pointer they receive - LibArena handles that
- **Thread Safety**: LibArena is not thread-safe by default
- **Performance**: Slight overhead due to tracking, but minimal for most use cases
- **Capacity**: Collector arrays grow automatically, but initial sizing helps performance

---

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch: `git checkout -b feature/amazing-feature`
3. Commit your changes: `git commit -m 'Add amazing feature'`
4. Push to the branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

### Development Guidelines
- Follow 42 School coding standards
- Add tests for new features
- Update documentation as needed
- Ensure no memory leaks in test suite

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- Inspired by arena allocators and garbage collection concepts
- Designed for 42 School projects and C learning
- Thanks to the C programming community for best practices

---

**Made with ❤️ for better C memory management**