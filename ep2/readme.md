# Definition
G = (V, E)

**Binary relation**
```
e (~c) f if e = f
or if (e != f) and there is a cycle in G that traverses both e and f
```

> (~c) is an equivalence relation

> (~c) are biconnected components of G


> A parent vtx is a articulation point if low[child] >= disc[parent]

Cut vertex
bridges
biconnected components

```
GetArticulationPoints(i, d)
    visited[i] := true
    depth[i] := d
    low[i] := d
    childCount := 0
    isArticulation := false

    for each ni in adj[i] do
        if not visited[ni] then
            parent[ni] := i
            GetArticulationPoints(ni, d + 1)
            childCount := childCount + 1
            if low[ni] ≥ depth[i] then
                isArticulation := true
            low[i] := Min (low[i], low[ni])
        else if ni ≠ parent[i] then
            low[i] := Min (low[i], depth[ni])
    if (parent[i] ≠ null and isArticulation) or (parent[i] = null and childCount > 1) then
        Output i as articulation point
```

```
time = 0
function DFS(vertex, adj[][], low[], disc[], parent[], visited[], V, stack)
    disc[vertex]=low[vertex]=time+1
    time = time + 1
    visited[vertex]=true
    child = 0
    for i = 0 to V
        if adj[vertex][i] == true
            if visited[i] == false
                child = child + 1
                push edge(u,v) to stack
                parent[i] = vertex
                DFS(i, adj, low, disc, visited, V, time, stack)
                low[vertex] = minimum(low[vertex], low[i])

                if parent[vertex] == nil AND child > 1
                    while last element of stack != (u,v)
                        print last element of stack
                        pop from stack
                    print last element of stack
                    pop from stack

                if parent[vertex] != nil AND low[i] >= disc[vertex]
                    while last element of stack != (u,v)
                        print last element of stack
                        pop from stack
                    print last element of stack
                    pop from stack
                    
            else if parent[vertex] != i AND disc[i] < low[vertex]
                low[vertex] = disc[i]
                push edge(u,v) to stack

fuction biconnected_components(adj[][], V)
    for i = 0 to V
        if visited[i] == false
            DFS(i, adj, low, disc, parent, visited, V, time, stack)
            while stack is not empty
                print last element of stack
                pop from stack
```
```
Vtx adjacent to 1:
2
4
Vtx adjacent to 2:
1
3
5
Vtx adjacent to 3:
2
4
Vtx adjacent to 4:
3
1
Vtx adjacent to 5:
2
```