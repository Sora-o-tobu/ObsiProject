
# Lecture 1. Introduction

## Definition

- <1> Database is a very large, integrated collection of data
- <2> A DBMS is a software system designed to *store, manage and facilitate access to* databases

!!! note "换一种表述"
	Datebase Management System(DBMS) = (Database) + A set of programs used to access, update and manage the data in database

传统 OS 提供的 File-Processing System 不能很好地维护 data files 的格式，并且 data files 之间的关联性很低。

??? quote "具体而言存在以下问题"
	- (1) Data redundancy and inconsistency
		- Multiple file formats, duplication of information in different files
	- (2) Difficulty in accessing data
		- Need to write a new program to carry out each new task
	- (3) Data isolation — multiple files and multiple formats
		- Difficult to retrieve, difficult to share
	- (4)Integrity problems
		- Integrity constraints  (e.g. account balance > 0) become part of program code, so that it's hard to add new constraints or change existing ones
	- (5) No atomicity of updates
	- (6) Difficult to concurrent access by multiple users
	- (7) Security problems (i.e., Right person uses right data)

因此我们需要一个不同于操作系统的 Datebase Management System 来解决以上问题。

对于 DB，我们将其抽象为 3-level，不同的层有不同的用途：

![[3levelDB.png]]

- **Physical Level:** how a record is stored
	- 在物理存储介质中如何保存、实现
- **Logical Level:** describes data stored in database, and the relationships among the data on upper level
	- 数据库中存储什么数据？数据之间有什么关系？
- **View Level:** application programs hide details of data types
	- 根据需求展现给不同用户的数据视图

A very brief example:

![[3levelDBex1.png]]

- **Schema** - the structure of the database on different level
	- Physical schema: database structure design at the physical level
	- Logical schema: database structure design at the logical level
	- Subschema: schema at view level
- **Instance** - the actual content of the database at a particular point in time

通俗来讲，可以认为 Schema 是表的 Column，Instance 为表的 Row(及其内容)，二者结合在一起才是 Relation。

Independence 指的是修改某一层的 Schema 定义，而不影响高层的 Schema 的能力。例如，Physical Data Independence 为修改 Physical Schema 并且不需要更改 Logical Schema 的 能力。

!!! info "DBMS 做到 Independence 的要点为只修改两层之间的映射，而不修改实际 Schema"

## Data Models

不同 level 的 data abstraction 可能需要不同的 data model 去描述，常见的 data model 有：

- **Entity-Relationship Model**
	- 通常用于数据库的 Conceptual Design
- **Relational Model**
	- 通常用于数据库的 Logical Design
- Other Models
	- Object-Oriented Model
	- Semi-structured Data Model
	- Older Models such as network model, hierarchy model, etc

本课程主要涉及的数据库建立在关系型模型的基础上，它有如下三点关键：

- Store database in simple data structures (relations).
- Access data through high-level language, DBMS figures out best execution strategy.
- Physical storage left up to the DBMS implementation.

因此，对于 Ralational Model 设计的三个重要概念在于：

- <1> **Structure:** The defination of the database's relations and their contents
- <2> **Intergrity:** Ensure the database's contents satisfy constraints
- <3> **Manipulation:** Programming interface for accessing and modifying a database's contents

## Database Language

- **Data Definition Language, DDL:** 指定了数据库的 Schema，产生的 metadata 会存放在 data dictionary 中
- **Data Manipulation Language, DML:** 从数据库存取数据的语言，即与 DB 交互的方式，也被称为 Query Language，它可以分为两类
	- **Procedural Method:** 对数据库的请求专注于更高层次上对如何查询想要结果的策略
	    - Relational Algebra, C, Java 等，数据库必须由过程式语言编写
	- **Non-Procedural (Declarative):** 对数据库的请求只指出我们想要的数据，而不是怎么去找到它
	    - Relational Calculus, SQL 本身也属于这一类
- **Data Control Language, DCL:** 用来控制数据的权限

!!! info "SQL 是最广泛使用的 Query Language；有些 DBMS 为 SQL 提供了 Procedural Extensions"
	SQL = DDL + DML + DCL

## Users & Administrators

根据用户不同的交互需求，可以将他们分为 Naive Users、Sophisticated users、Specialized users、Application programmers 等：

![[UsersandAdmin1.png]]

Database Administrator(DBA) 是一个特殊的用户，他拥有数据库的所有权限，包括组成 DBMS 的 Programs。DBA 要做定义模式，数据库调谐，故障时恢复（备份）等工作。

## Database Architecture

!!! quote ""
	![[DatabaseArchitecture1.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">Query Processor</font>

Query Processor 包括 DDL interpreter，DML compiler 以及 query processing。它将一个 SQL 查询分割翻译为 Relation-Algebra，再由 Optimzer 优化组织成 Execution Plan，最后在低层次上执行输出：

![[queryprocessor.png]]

<font style="font-weight: 1000;font-size: 20px" color="red">Storage Manager</font>

Storage Manager 包括 Transaction Manager、Authorization and Integrity Manager、File Manager 以及 Buffer Manager，它是内存和磁盘之间的接口。

具体实现以后应该会讲（或者你做了 `minisql` 应该知道）。

!!! quote "[执行一条 SQL 语句的流程](https://xiaolincoding.com/)"
	![[sqloverview.png]]