# circilarlist-cpp

A circular (ring) singly linked list implemented in modern C++17, with a small demo `main()`.

The list keeps a `tail` pointer whose `tail->next` is the head, so the whole structure
forms a closed circle — every node is reachable from any other node.

## Features

| Operation | Complexity |
|---|---|
| `pushFront(value)` | O(1) |
| `pushBack(value)` | O(1) |
| `popFront()` | O(1) |
| `removeValue(value)` | O(n) |
| `find(value)` | O(n) |
| `toString()` / `display()` | O(n) |
| `clear()` | O(n) |

Notes:

- Walks always iterate exactly `count` steps, so traversal always terminates even though the links are circular.
- The class follows the rule of three/five: copying is `= delete`d (raw owning pointer), and the destructor calls `clear()` to release every node.
- `popFront()` / `find()` return `std::optional<int>`, so "empty" and "not found" are explicit.

## Build & run

```bash
g++ -std=c++17 -Wall -Wextra -O1 -o CircularList CircularList.cpp
./CircularList
```

## Sample output

```
===== Circular List Demo =====

Start            : (empty)   [size=0]
pushBack(10)     : 10 -> (back to 10)   [size=1]
pushBack(20)     : 10 -> 20 -> (back to 10)   [size=2]
pushBack(30)     : 10 -> 20 -> 30 -> (back to 10)   [size=3]
pushBack(40)     : 10 -> 20 -> 30 -> 40 -> (back to 10)   [size=4]

pushFront(5)     : 5 -> 10 -> 20 -> 30 -> 40 -> (back to 5)   [size=5]
find(30)       -> found at index 3
find(99)       -> not found

popFront()     -> removed 5
after popFront   : 10 -> 20 -> 30 -> 40 -> (back to 10)   [size=4]
removeValue(20) -> removed 20
after remove     : 10 -> 30 -> 40 -> (back to 10)   [size=3]

Full lap       : 10 30 40 
After emptying   : (empty)   [size=0]

Program finished successfully.
```

A full captured run is available in [`run_output.txt`](run_output.txt), and a screenshot of the run in [`program_run.png`](program_run.png).
