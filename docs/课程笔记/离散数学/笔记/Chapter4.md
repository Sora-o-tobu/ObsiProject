# CH 4 : Number Theory and Cryptography

## 4.1 Divisibility and Modular Arithmetic

- [Division Theorem](#): 𝑎=𝑑𝑞+𝑟 (𝑑>0,0≤𝑟<𝑑)
    - 𝑑 is the divisor
    - 𝑎 is the dividend
    - 𝑞 is the ==quotient== 商
    - 𝑟 is the remainder

!!! example "负数的Division Theorem?"
	-11 = -4 × 3 + 1
	
	余数要是正数


[Some Theorem](#)

- if m divides (a-b) , a is ==congruent== to b modulo m    即 a 与 b 同余
- $a\equiv b(\mod m)\ iff\ (a\mod m)=(b\mod m)$
- $a\equiv b(\mod m)\ iff\ \exists k\ such\ that\ a=b+km$
- $a\equiv b(\mod m)\ and\ c\equiv d(\mod m)$ , then $a+c\equiv b+d(\mod m)\ and\ ac\equiv bd(\mod m)$
	- ==一个有用的推论==  $ab\mod m=((a\mod m)(b\mod m))\mod m$


### Modular Arithmatic

- Addition modulo : $a+_mb=(a+b)\mod m$
- Multiplication modulo : $a\cdot _mb=(a\cdot b)\mod m$

## 4.3 Primes and Greatest Common Divisors

!!! info
	There are infinitely many primes

<center>ab=gcd(a,b) × lcd(a,b)</center>

- Let a = bq + r , then gcd(a,b) = gcd(b,r)
- There exist such integers that gcd(a,b) = sa + tb ==裴蜀定理==
	- ![[ExampleBezout.png]]




## 4.4 Solving Gongruences

- 形如 $ax\equiv b (\mod m)$ 的方程被称为同余方程
	- An integer $\overline{a}$ 满足 $\overline{a}a \equiv 1(\mod m)$ 被称为 an inverse of a modulo m ==(数论倒数)==
		- Example: 5 is an inverse of 3 of a modulo 7 since 15%7 = 1
- 如何求解数论倒数？
	- 根据裴蜀定理，1 = gcd(a,m) = sa + tm , 两边对 m 取模得 $sa\equiv 1(\mod m)$ ，那么s即为数论倒数 
	- 例子1: 求 101 modulo 4620 的数论倒数
		- ![[数论倒数例1.png]]
	- 例子2: 
		- $$\begin{gather}7=2\times 3+1 \\1= (-2)\times3 +1\times 7 \\1 \equiv (-2\times 3+7)\ \mod 7 \\\end{gather}$$
- 有了数论倒数后，既可以更公式化的求解同余方程了:
	- 我们有 $\overline{a}a \equiv 1(\mod m)$ 和 $ax\equiv b (\mod m)$ ，两边同时乘以 $\overline{a}$ 得到 $x\equiv \overline{a}b(\mod m)$ 

