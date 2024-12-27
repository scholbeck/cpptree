# cpptree - An Extensible C++ Library for Decision Trees

This repository contains a pre-alpha version of cpptree - a C++ library for decision trees.

The source code is based on the C++ 11 standard and has no dependencies except for the C++ Standard Library.
A modular software design allows for an implementation of a large variety of decision tree algorithms via simple software extensions.
The library is written in C++ to achieve a high degree of computational performance, while allowing for object-oriented programming and portability to other programming languages, e.g., Java, R or Python.

##

### Standalone Application via Command Line Interface

Compile the application first by running `make` from a terminal in `code/cpp`. Go to `code/cpp/bin` and run one of the commands supplied by the makefile, e.g., `make boston`, which is a shortcut to execute the application via the CLI.

![cpptree_boston](https://github.com/user-attachments/assets/2a144357-6f76-4d9c-8555-d65b2f44ff14)

### R API

Simply go to `code/R` and open `test_package.R`, which provides code examples on how to access the R API, including converting the tree structure to `partykit` package format for visualizations.

<img src="https://github.com/user-attachments/assets/e03a78c9-6447-41c5-b396-90ca3d1a0567" alt="R API" width="500"/>

<img src="https://github.com/user-attachments/assets/38b24272-6268-46f7-a12a-bf07c2218cc1" alt="R API" width="500"/>



![ggparty_mean](https://github.com/user-attachments/assets/bf0d2dd8-404e-47c8-9d53-f2b5ce18a961)




