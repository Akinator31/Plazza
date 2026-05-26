# Plazza

A pizzeria simulation built in C++ using multi-process and multi-thread architecture.

## Overview

Plazza simulates a pizzeria composed of a reception and several kitchens. The reception accepts orders from the user, dispatches pizzas across kitchens, and kitchens cook them concurrently using a thread pool of cooks.

## Usage

```bash
./plazza <multiplier> <cooks_per_kitchen> <restock_time_ms>
```

| Parameter | Description |
|---|---|
| `multiplier` | Cooking time multiplier (accepts float, e.g. `0.5` to halve cooking times) |
| `cooks_per_kitchen` | Number of cooks (threads) per kitchen |
| `restock_time_ms` | Time in milliseconds between each ingredient restock |

**Example:**
```bash
./plazza 2 5 2000
```

## Shell Commands

Once running, the reception exposes an interactive shell:

| Command | Description |
|---|---|
| `TYPE SIZE xN [; TYPE SIZE xN]*` | Place a pizza order |
| `status` | Display the status of all kitchens |

**Order example:**
```
regina XXL x2; fantasia M x3; margarita S x1
```

### Pizza types

| Type | Ingredients | Cooking time |
|---|---|---|
| `margarita` | dough, tomato, gruyere | 1s × multiplier |
| `regina` | dough, tomato, gruyere, ham, mushrooms | 2s × multiplier |
| `americana` | dough, tomato, gruyere, steak | 2s × multiplier |
| `fantasia` | dough, tomato, eggplant, goat cheese, chief love | 4s × multiplier |

### Sizes

`S`, `M`, `L`, `XL`, `XXL`

## Architecture

```
Reception (main process)
├── Kitchen 1 (child process)
│   ├── Cook 1 (thread)
│   ├── Cook 2 (thread)
│   └── ...
├── Kitchen 2 (child process)
│   └── ...
└── ...
```

- **Reception** — interactive shell, load balancer, manages kitchen lifecycle
- **Kitchens** — child processes created on demand via `fork()`, closed after 5 seconds of inactivity
- **Cooks** — threads managed by a thread pool local to each kitchen
- **IPC** — Unix Domain Sockets between reception and kitchens

### Key constraints

- Each kitchen accepts at most `2 × N` pizzas (queued or cooking), with N being the number of cooks
- Pizzas are dispatched one by one to the least loaded kitchen
- A new kitchen is spawned when all existing ones are saturated
- Each kitchen starts with 5 units of each ingredient, restocked by 1 unit every `restock_time_ms`

## Build

```bash
# Makefile
make

# CMake
cmake -B build && cmake --build build
```

## Requirements

- C++20
- Linux (Unix Domain Sockets, `fork`, `pthread`)