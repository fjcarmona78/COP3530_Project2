<p align="center">
  <img src="assets/CineRankAnalytics.png" width="500" alt="CineRank Analytics Logo"/>
</p>

# CineRank Analytics

A C++ application that loads and indexes a large movie dataset using two self-balancing tree structures: a Red-Black Tree and a Splay Tree. Users can search, rank, and analyze movies through an interactive GUI built with ImGui and GLFW.

---

## Dependencies

- **GLFW** — OpenGL window management
- **ImGui** — GUI rendering
- **Catch2** — Unit testing
- **CMake** — Build system

---

## How to Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

## How to Run

1. Run the executable
2. Choose tree type:
   - `0` — Splay Tree
   - `1` — Red-Black Tree
3. Use the search bar with the following commands:

| Input | Result |
|---|---|
| `List` | Shows first 300 movies in level order |
| `popular` | Returns the most popular movie |
| `revenue` | Returns the highest revenue movie |
| Any number | Searches by Movie ID, Popularity Rank, or Revenue depending on the dropdown selection |

---

## Project Structure

```
src/
  main.cpp              - Entry point, tree setup, and CSV parsing
  Movie.h               - Movie data structure
  RBTree.h/.cpp         - Red-Black Tree implementation
  SplayTree.h/.cpp      - Splay Tree implementation
  BSTree.h              - Abstract base class interface
  Parser.h/.cpp         - CSV parsing and popularity rank assignment
  WindowManager.h/.cpp  - ImGui/GLFW GUI management
test/
  test.cpp              - Catch2 unit tests (14 tests)
```

---

## Big O Worst Case Time Complexities

### Red-Black Tree

| Operation | Complexity | Notes |
|---|---|---|
| Insert | O(log n) | RB tree guarantees balanced height |
| Search by Movie ID | O(log n) | BST search on balanced tree |
| Search by Rank (BY_RANK) | O(log n) | BST search on balanced tree |
| Search by Rank (BY_MOVIEID) | O(n) | Falls back to full traversal |
| Get Most Popular Movie | O(n) | Full traversal, no popularity index |
| Get Highest Revenue Movie | O(n) | Full traversal, no revenue index |
| Search by Revenue | O(n) | Full traversal, no revenue index |
| Level Order Traversal | O(min(n, 300)) | Capped at 300 nodes |

### Splay Tree

| Operation | Complexity | Amortized | Notes |
|---|---|---|---|
| Insert | O(n) | Worst case on degenerate tree |
| Search by Movie ID (BY_MOVIEID) | O(n) | Splay worst case |
| Search by Rank (BY_RANK) | O(n) | Splay worst case |
| Search by Movie ID (BY_RANK) | O(n) | Full traversal fallback |
| Search by Rank (BY_MOVIEID) | O(n) | Full traversal fallback |
| Get Most Popular Movie | O(n) | Calls searchByRank |
| Get Highest Revenue Movie | O(n) | Full traversal, no revenue index |
| Search by Revenue | O(n) | Full traversal, no revenue index |
| Level Order Traversal | O(min(n, 300)) | Capped at 300 nodes |
| Destroy Tree | O(n) | Must visit every node |

### Key Observations

- The **Red-Black Tree is strictly better** in worst case, O(log n) vs O(n) for insert and search
- The **Splay Tree has O(log n) amortized** performance, making it efficient over a sequence of operations even if individual operations can be slow
- Both trees cap `levelOrderTraversal` at 300 nodes, making it effectively O(1) once the tree is large enough
- `searchByRevenue` is O(n) on both trees since neither is organized by revenue, a separate revenue-indexed structure would be needed to improve this

---

## Unit Tests

14 tests across two suites using Catch2. Run via Visual Studio's Test Explorer or from the command line with CTest.

### RedBlackTreeTests (6 tests)
- Empty tree level order is empty
- Single insert becomes root
- Insert by movieID produces expected small structure
- Insert by popularity uses popularity for placement
- searchByMovieID prints movie details when found
- searchByMovieID prints not found message when missing

### SplayTreeTests (8 tests)
- Empty splay tree level order is empty
- Single insert in splay tree can be found by rank
- Splay tree searchByRank returns the correct movie
- Splay tree level order returns inserted nodes
- Splay tree searchByMovieID prints movie details when found
- Splay tree searchByMovieID prints not found message when missing
- Splay tree getMostPopularMovie returns popularity of rank 1 movie
- Splay tree getHighestRevenueMovie returns highest revenue value