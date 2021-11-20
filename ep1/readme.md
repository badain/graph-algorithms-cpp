# 2-SAT Problem
## Description
Checks if a 2-CNF formula statisfiable by searching for a valid truth assignment.

## Pseudocode
1. Builds digraph corresponding to the 2-CNF formula in the implicative normal form;
2. Find all strongly connected components (SCC);<br/>
*May use Tarjan's or Kosaraju's algorithm*
3. For each variable (x)
    1. if (x) and (!x) are in the same SCC, then is UNSATISFIABLE;
4. Sort SCCs in Topological Order (may be the result of the SCC algorithm);
5. For each SCC (C), in reverse order
    1. if (C) literals are not assigned:
        1. set each literal to 1 (TRUE)
        2. set each negation to 0 (FALSE)
6. Return satisfying assignment

## Input
Each test case has the following format:
- The first line has an integer d ≥0 by itself. This shall be better explained in Section 5.
- The second line has two integers, n and m, such that 1 ≤ n ≤ 105 and 1 ≤ m ≤ 105. Here n is the number of variables, which are x1, x2, . . . , xn, and m is the number of clauses.
- The next group of m input lines encode the clauses, C1, . . . , Cm. For each i ∈ [m], the ith line in this group has two (potentially equal) integers, a and b, describing the clause Ci, with the following interpretation. If a > 0, then the literal ‘xa’ appears in clause Ci; if a < 0, then the literal ‘¬x−a’ appears in clause Ci. The same interpretation applies to the integer b.

### Sample data
```
0
3 4
-1 -1
2 -3
-1 3
2 3
```
encodes for
```
> debug = 0
> num_variables = 3
> num_clauses = 4
> C1 = (!x1 || !x1) => !(!x1) ~> (!x1) and !(!x1) ~> (!x1)
> C2 = ( x2 || !x3)
> C3 = (!x1 ||  x3)
> C4 = ( x2 ||  x3)
```

## Goals
### Debug Level 2: Digraph Construction
> `(a || b) => ((!a) -> (b)) and ((!b) -> (a))`

If d = 2, then the output of your program should be a description of the digraph DΦ = (V, A) in the following format.

1. Print two integers, |V| and |A|
2. After this the output should print |A| lines, one for each arc, and each one containing two integers. An arc of the form (l1, l2), where l1 and l2 are literals, should be printed in the reverse format of the input description. Namely, if l1 is a variable xi, print i, and if l1 is the negation of a variable, e.g., l1 = !xi, print −i. The same applies to the second literal l2.

You do not need to worry about the existence of parallel arcs, e.g., two arcs joining the same pair of vertices. That is, your program may print multiple arcs between two vertices or not, without being penalized. However, you must make sure to have |A| ≤ 2m.

```
1 -1
1 -1
-2 -3
3 2
1 3
-3 -1
-2 3
-3 2
```

### Debug Level 1: Strong Components of Auxiliary Digraph
1. For each vertex u of the graph, mark u as unvisited. Let L be empty.
2. For each vertex u of the graph do Visit(u), where Visit(u) is the recursive subroutine:
    - If u is unvisited then:
        1. Mark u as visited.
        2. For each out-neighbour v of u, do Visit(v).
        3. Prepend u to L.
    - Otherwise do nothing.

3. For each element u of L in order, do Assign(u,u) where Assign(u,root) is the recursive subroutine:
    - If u has not been assigned to a component then:
        1. Assign u as belonging to the component whose root is root.
        2. For each in-neighbour v of u, do Assign(v,root).
    - Otherwise do nothing.