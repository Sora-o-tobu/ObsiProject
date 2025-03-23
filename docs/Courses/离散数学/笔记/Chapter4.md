# CH 4 : Number Theory and Cryptography

## 4.1 Divisibility and Modular Arithmetic

- [Division Theorem](#): 𝑎=𝑑𝑞+𝑟 (𝑑>0,0≤𝑟<𝑑)
    - 𝑑 is the divisor
    - 𝑎 is the dividend
    - 𝑞 is the ==quotient== 商
    - 𝑟 is the remainder

!!! example "负数的Division Theorem?"
	-11 = -4 × 3 + 1
	
	==余数要是正数==


[Some Theorem](#)

- if m divides (a-b) , a is ==congruent== to b modulo m    即 a 与 b 同余
- $a\equiv b(\mod m)\ iff\ (a\mod m)=(b\mod m)$
- $a\equiv b(\mod m)\ iff\ \exists k\ such\ that\ a=b+km$
- $a\equiv b(\mod m)\ and\ c\equiv d(\mod m)$ , then $a+c\equiv b+d(\mod m)\ and\ ac\equiv bd(\mod m)$
	- ==一个有用的推论==  $ab\mod m=((a\mod m)(b\mod m))\mod m$

!!! example "同余解题步骤"
	Solve the system of congruence $x\equiv 3(\mod 6)$ and $x\equiv 4(\mod 7)$ using the method of back substitution
	![[同余例题1答案.png]]


### Modular Arithmatic

- Addition modulo : $a+_mb=(a+b)\mod m$
- Multiplication modulo : $a\cdot _mb=(a\cdot b)\mod m$

## 4.3 Primes and Greatest Common Divisors

!!! info
	There are infinitely many primes (反证法证明)

- Fermat's Little Theorem
	- 对于任意质数p，任意整数a，有 $a^p\equiv a(\mod p)$ (若a与p互质，则有 $a^{p-1}\equiv 1(\mod p)$ )
	- 例子: 利用Fertmat定理求$3^{302}\mod 5$
		- $3^{300}\equiv(3 ^{4}) ^{75}\equiv 1^{75} (\mod 5)$
		- So $3^{302}\mod 5 =9\mod 5=4$



<center>ab = gcd(a,b) × lcd(a,b)</center>

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

### 中国剩余定理

**例子**：𝑥≡2(mod3),𝑥≡3(mod5),𝑥≡2(mod7)

- 𝑚=3⋅5⋅7=105
- 𝑀1=35,𝑀2=21,𝑀3=15
- 分别求解 𝑀1mod3 的逆、𝑀2mod5 的逆、𝑀3mod7 的逆
- 分别是 2、1、1
- 𝑥=2⋅35⋅2+1⋅21⋅3+1⋅15⋅2=233≡23(mod105)
---
其实上述问题也可以直接求解，我就不背公式了

- $x\equiv 2(\mod 3)\ \Rightarrow \ x=3t+2$
- $3t+2\equiv 3(\mod 5)\ \Rightarrow \ 3t\equiv 6(\mod 5)$
- $t=5u+2\ \Rightarrow \ x=15u+8$
- $15u\equiv -6(\mod 7)\ \Rightarrow \ 15u\equiv 15(\mod 7)$
- $u=7p+1\ \Rightarrow \ x=105u+23$

!!! example "More Example"
	![[中国剩余定理例子.png]]
	x=323+330k

