# Reachability
Reachability is a property assigned to a pair of vertices from a graph. A vertex t is **reachable** from vertex s if there is a *st-walk* in the graph.

```
t is reachable from s
s ~ t  (graph)   [undirected]
s ~> t (digraph) [directed]
```

*Reachability is useful to indicate relatioships between vertices in a graph:*
> Reachability is a *binary* property.

## Reachability Properties
### Digraphs D
1. Reflexive: `v ~> v`
2. Transitive: `x ~> y` and `y ~> z` imply `x ~> z`
### Graphs G
1. Symmetric: `v ~ u` implies `u ~ v`
2. Reflexive `v ~ v`
3. Transitive `x ~ y` and `y ~ z` imply `x ~ z`


# Reachability Problem
- *Input* a Digraph D and vertices s, t;
- *Output* is t reachable from s? (YES || NO)
  - If YES -> exists st-Walk from s to t
  - If there is a st-Walk there is a st-Path

> finding the shortest (st-walk) is more time demanding

## 1) Aplication: 2-Satisfiability Problem (2-SAT)
- *Input:* Logical Formula PHI in CNF (Conjunctive Normal Flow) where each clause has 2 literals;
- *Output:* Is PHI satisfiable? (Is there a truth assignment of literals that can make PHI be truth?)

> Literal: variable that stores a boolean value and, possibly, its negation; (Variable: (a) = Literals (a) and (!a)<br>
> Clause: logical operation with literals;<br>
> Conjunctive Normal Flow: combination of clauses of literals;<br>
> Truth assignment: function that assigns boolean values to literals.

`PHI = (!x || y) && (!z || h)`<br>
*This is a logical formula PHI in CNF. What values must x, y, z and h assume to make PHI truthy?*

### Solution
1. Build a Digraph D=(V,A) from PHI;
2. One vertice for each literal -> n(V) = 2*n(variables of PHI);
3. Two arcs for each clause -> n(A) = 2*n(clauses).<br>
4. Each edge correspond to a implication from a clause. 
*Let (a || b) be a clause of PHI. Then, D must contain arcs ((!a) -> (b)) and ((!b) -> (a))*<br/>
*Because (a || b) can be written as (!a -> b) && (!b -> a) [if one of a or b is false, then the other is true]*<br/>
*Implication means that the values are the same, that is, vertices in the same Strongly Connected Components have the same value.*

**Property:**
> PHI is satisfiable if and only if there is no path as such a ~> !a or !a ~> a<br>
> because a variable cannot be truth and false simultaneously