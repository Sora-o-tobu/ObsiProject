# CH 3 : Algorithms
## 3.1 Algorithms

[Definition](#) An algorithm is a finite set of precise instructions for performing a computation or for solving a problem.

### Some common properties of algorithms 算法的性质

- **Input:** An algorithm has input values from a specified set.
- **Output:** From each set of input values, an algorithm produces output values from a specified set.
- **Definiteness:** The steps of an algorithm must be defined ==precisely== 准确性
- **Correctness:** An algorithm should produce the ==correct output values== for each set of  input values. 正确性
- **Finiteness:** An algorithm should produce the desired output after a ==finite number of steps== for any input in the set. 有限性
- **Effectiveness:** Each step of an algorithm must be executed exactly and in a finite amount of time. 
- **Generality:** The procedure should be applicable for all problems of the desired form, not just for a particular set of input values.

## 3.2 The growth of functions

### Factors to be considered for choosing an algorithm
- **Simplicity** - Easy to implement and obviously correct
- **Clarity** - Easy to read and to maintain
- **Extensibility** - Can easily be extended to new tasks
- **Reusability** - Can be adapted to different tasks
- **Efficiency** - Uses time and space well (computation complexity)

### Big-O Notation
$$
We\ say\ that\ f(x)=O(g(x))\ if\ \exists C\exists k\forall (x\gt k)\ |f(x)|\lt C|g(x)|
$$

### Big-Omega Notation
$$
We\ say\ that\ f(x)=\Omega(g(x))\ if\ \exists C\exists k\forall (x\gt k)\ |f(x)|\ge C|g(x)|
$$

### Big-Theta Notation
$$
If\ f(x)\ is\ O(g(x))\ as\ well\ as\ \Omega (g(x)),then\ we\ say\ that\ f(x)=\Theta(g(x))
$$

!!! note
	$O(n^3 2^n)<O(n^2 3^n)$
	
	$O(2^n)\ne O(3^n)$
## 3.3 Complexity of Algorithms
两个方面，space complexity 和 time complexity

**[Definition](#) Tractable:** A problem is solvable using an algorithm with polynomial worst-case complexity. 最坏情况下能够使用多项式复杂度的算法解决问题

> **Class P** : Tractable problems
> 
> **Class NP(nondeterministic polynomial time 非确定性多项式时间):** problems for which a solution can be ==checked== in polynomial time. 
> 
> **NP-complete(非多项式完全问题):** An important class of problems with the property that if any of these problems can be solved by a polynomial worst-case time algorithm, then all can be solved by polynomial worst-case time algorithms.

