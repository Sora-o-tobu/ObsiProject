
# Entity-Relationship Model

## E-R Diagrams

!!! quote ""
	![[ERModelEx.png]]

> https://www.drawio.com/

在 E-R 图中：

- **Rectangles**：方形框表示 Entities Set，框内内容即为其 attributes。.
- **Ellipses**：椭圆表示 attributes，但是一般都放进 Entities Set 内了，不单独画出
- **Diamonds**：菱形框表示 Relationship Set

## Attributes

一个 entity 可由 attribute 的集合表示，attribute 可允许的取值范围被称为 *Domain*。attribute 可划分为：

- <1> Simple / Composite Attributes
	- 复合属性例如 name，由 first name，last name 复合得到
- <2> Single-valued / Multi-valued Attributes
	- 多值属性例如 phone，可由多个电话号码组成
- <3> Derived Attributes
	- 相对于 Base Attribute 派生出来的属性，例如 age 由 birthday 派生

在 E-R 图中，不同类型的属性表示方法如下：

=== "E-R Diagram"
	![[AERD1.png]]
=== "Entity(仅作为实体表示)"
	![[AEn2.png]]

## Cardinality & Participation Constraints

**Mapping Cardinality Constraints** 限定了一个实体与关系另一端的实体可能关联的数目上限：

!!! question ""
	=== "One-to-One Relationship"
		![[O2OR.png]]
	=== "One-to-Many Relationship"
		![[O2MR.png]]
	=== "Many-to-One Relationship"
		![[M2OR.png]]
	=== "Many-to-Many Relationship"
		![[M2MR.png]]

全参与和部分参与约束反映了一个实体参与关联的数目下限是 1 次还是 0 次，在 E-R 图中分别用 Double Line 和 Line 表示：

![[TPPEx1.png]]


## Weak Entity Sets

不存在完整的 Primary Key 的实体集称为 Weak Entity Set。Weak Entity Set 需要依赖另一个实体集才能存在，与另一个实体集的一些 Attributes 组合形成它的 Primary Key。其中位于 Weak Entity Set 中的 Attributes 称为**discriminator**或者**partial key**。

在 E-R 图中，我们在 discriminator 下面添加 dash line(虚线)，设置 Weak Entity Set 的 Identifying Relationship 为 double diamond：

![[WESEx.png]]

对于上图关系，我们认为弱实体集的 Primary Key 为 `(course_id, sec_id, semester, year)`。

!!! info "Weak Entity Set 一定是多对一、全参与的，因此上图采用箭头与双横线"

## Extended E-R features

<font style="font-weight: 1000;font-size: 20px" color="orange">Specialization 特殊化、具体化</font>

Specialization 是一种 Top-down 设计方法，类似于面向对象的继承特性。lower-level 的实体集会继承 high-level 实体集的所有属性，并且可以拥有自己额外的属性以及关系。

=== "E-R 1"
	![[SEREx1.png]]
=== "E-R 2"
	![[SEREx2.png]]

!!! note "ISA 在数据库语义中表示为"is a"，表示父类和子类的联系"
	在上 E-R 1 中，如果 ISA 旁有 `disjoint` ，则表示子类之间是不相交的，一个实体只能属于一个子实体集；否则，则默认为 `overlapping`，即子类之间是可重叠的，一个实体可以属于多个子实体集。

<font style="font-weight: 1000;font-size: 20px" color="orange">Generalization 泛化、普遍化</font>

Generalization 是一种 Bottom-Up 设计方法，其特点是组合拥有相同 features 的实体集，形成 higher-level entity set。

Specialization 和 Generalization 只是普通的倒置，因此在 E-R 图上，它们的表示完全相同。

## E-R to Tables

- **Composite Attributes** 拆分成多个 Separate Attributes
	- 例如，`address` 拆为 `address.city`, `address.state`
- **Multi-Valued Attributes** 用单独一个 Tables 存储
	- 例如，将实体集 `user(id, phone_number)` 转换成下面两个 Table
	- `user(id)`
	- `user_phone(id, phone_number)` Primary Key 为 `(id, phone_number)`
- **Many-to-One/One-to-Many** 可以选择将“关系”对应的 Table 合并到 Many 端实体集对应的 Table 中
	- 例如，我们有三个 Table：`account(account_number, balance)`, `branch(branch_name, assets)`, `account_branch(account_number, branch_name)`。其中 account 和 branch 属于 Many-to-One Relation
	- 可以合并为：`account(account_number, branch_name, balance)`
- 对于 **One-to-One**，则可以任意选取一边合并
- **Many-to-Many** 不能将关系对应的 Table 合并，或者说合并了也没有意义

!!! info "关系对应的 Table 除了包含两边的 Primary Key 外，还可以存储一些额外的 Decriptive Message"

