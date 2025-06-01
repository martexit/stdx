# stdx: A Minimalist C99 Utility Library

![stdx](https://img.shields.io/badge/stdx-v1.0.0-brightgreen) ![GitHub Release](https://img.shields.io/github/release/martexit/stdx.svg) ![GitHub Issues](https://img.shields.io/github/issues/martexit/stdx.svg)

Welcome to **stdx**, a lightweight and modular utility library designed for C99. Inspired by the STB style, **stdx** provides a collection of single-header components that enhance common programming tasks without introducing dependencies. This library is perfect for developers who seek simplicity and efficiency in their C projects.

## Table of Contents

- [Features](#features)
- [Components](#components)
  - [Array](#array)
  - [Filesystem](#filesystem)
  - [Hashtable](#hashtable)
  - [Logging](#logging)
  - [Networking](#networking)
  - [String Manipulation](#string-manipulation)
  - [Testing Library](#testing-library)
  - [Thread Pool](#thread-pool)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Release Information](#release-information)

## Features

- **Minimalist Design**: Focus on simplicity and ease of use.
- **Modular Architecture**: Use only the components you need.
- **Single-Header Files**: Easy to integrate into your projects.
- **Dependency-Free**: No external libraries required.
- **C99 Compliance**: Fully compatible with C99 standards.

## Components

### Array

The Array component provides a dynamic array implementation that allows you to create, manipulate, and manage arrays easily. It supports resizing and provides functions for adding, removing, and accessing elements.

### Filesystem

The Filesystem component simplifies file operations, including reading, writing, and directory manipulation. It also includes features for monitoring filesystem events, making it easier to respond to changes.

### Hashtable

The Hashtable component offers a fast and efficient way to store key-value pairs. It supports various hashing algorithms and collision resolution techniques to ensure optimal performance.

### Logging

The Logging component allows you to log messages with different severity levels. You can easily configure the logging output and format, making it suitable for debugging and monitoring applications.

### Networking

The Networking component provides basic networking functionality, including TCP and UDP communication. It allows you to create client-server applications with minimal setup.

### String Manipulation

The String Manipulation component includes various functions for handling strings, such as concatenation, splitting, and searching. It also provides a StringBuilder for efficient string construction.

### Testing Library

The Testing Library component offers a simple framework for writing and running tests. It includes assertions and test runners to help you ensure your code works as expected.

### Thread Pool

The Thread Pool component manages a pool of threads for executing tasks concurrently. It simplifies multithreading and improves performance in CPU-bound applications.

## Installation

To install **stdx**, download the latest release from the [Releases](https://github.com/martexit/stdx/releases) section. You can simply download the `.h` file and include it in your project.

## Usage

Here’s a quick example of how to use the Array component:

```c
#include "stdx.h"

int main() {
    Array *arr = array_create(sizeof(int), 10);
    int value = 42;
    array_push(arr, &value);
    printf("Element at index 0: %d\n", *(int *)array_get(arr, 0));
    array_free(arr);
    return 0;
}
```

This example demonstrates creating an array, adding an element, and retrieving it.

## Contributing

We welcome contributions to **stdx**. If you have suggestions, improvements, or bug fixes, please submit a pull request or open an issue. Make sure to follow the coding standards and include tests for new features.

## License

**stdx** is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Release Information

For the latest updates and releases, please visit the [Releases](https://github.com/martexit/stdx/releases) section. This section contains all the information you need about the versions, changes, and download links.

## Additional Resources

- **Documentation**: Comprehensive documentation is available in the `docs` folder.
- **Examples**: Check the `examples` folder for practical usage examples of each component.
- **Community**: Join our community on GitHub Discussions to share ideas and get help.

---

Thank you for considering **stdx** for your C programming needs. We hope you find it useful and easy to integrate into your projects.