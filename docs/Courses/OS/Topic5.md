
# File System

## File Basic

**File** 是包含分类和索引信息、访问权限的信息的存储空间中的数据。文件系统提供了与二级存储相关资源的抽象，能够让用户在不了解文件的各种属性、文件存储介质的特征以及文件在存储介质上的具体位置等情况下，方便快捷地使用文件。

除了文件本身数据外，大多数操作系统都会保存如下与文件相关的属性：

- **Name:** unique，以容易读取的形式保存
- **Type:** 被支持不同类型的文件系统所使用
- **Creater:** 文件创建者的 ID
- **Owner:** 文件当前所有者的 ID
- **Location:** 指向设备上文件的指针
- **Size:** 文件当前大小，也可包含文件允许的最大值
- **Protection:** 文件访问控制相关信息
- **Time:** 包含文件创建、上次修改、上次访问的时间，用于保护和跟踪文件的使用

文件属于抽象数据类型，我们需要考虑可以对文件执行的操作。OS 提供了一系列文件相关的 Syscall：

- **Create**
	- 1. 为新文件分配 Memory 空间
	- 2. 在 Directory 中为其创建一个 D entry，记录了包含新文件名、在外存中的地址等信息
- **Delete**
	- 根据文件名查找目录，删除对应 D entry 和 FCB，然后回收占用的存储空间
- **Write** - define a pointer
	- 根据文件名查找目录，找到 D entry 后得到其在外存中的地址
	- D entry 中还定义了一个读指针
- **Read** - use the same pointer
	- 根据文件名查找目录，找到 D entry 后利用写指针进行写操作

当用户对一个文件进行多次读/写时，每次都要重新检索目录。为了避免重复，大多数操作系统都要求用户使用系统调用 `open` 将文件打开再进行读写操作，使用系统调用 `close` 关闭。

系统会为打开的文件维护一个 *Open-File Table*，所谓打开就是系统在检索到指定文件的 D entry 后，将该目录项从外存复制到内存的 Open-File Table 中，并将该表项的文件描述符返回给用户。当用户再次对该文件发出操作请求时，可通过文件描述符在表中查找到文件信息，从而节省检索开销。

??? success "对于多进程系统，通常采用进程表和系统表两级"
	- System-wide Table 包含与进程无关的信息，如 Location，Time，Size
	- Per-process Table 包含进程对文件的使用信息，如读/写指针，Access Rights，以及指向系统表中对应条目的指针
	- 当一个进程执行 `open` 时，会为其进程表中增加一个条目，并指向系统表中对应该文件的条目
	- 系统表会为每个文件维护一个 Open Count，以记录多少进程打开了该文件；当 Open Count 为 0 时，即可从系统表中删除该条目
	
	![[topic5_1.png]]

!!! note "文件名只有一开始检索有用，之后用的都是文件描述符来访问 Open-File Table 中的表项"

## Directory

为了便于文件管理，我们引入 File Control Block 存放控制文件所需的各种信息。文件一一与 FCB 对应，FCB 的有序结合称为 Directory，而一个 FCB 就是一个 D entry。

!!! tip "通常我们也将一个 Directory 视为文件，即*目录文件*"

FCB 主要包含以下信息：

- **基本信息:** 文件名、文件物理位置、文件物理结构、文件逻辑结构等
- **存取控制信息:** 各类用户的存取权限
- **使用信息:** 文件建立时间、上次修改时间等

接下来我们由浅入深考虑目录结构的选择。

<font style="font-weight: 1000;font-size: 20px" color="orange">Single-Level Directory</font>

整个文件系统只建立一张目录表，每个文件占用一个目录项：

![[topic5_2.png]]

当建立新文件时，必须检索所有目录项，以确保没有“重名”的情况，因此单级目录结构存在查找速度慢、文件不允许重名、不便于文件共享等缺点。

<font style="font-weight: 1000;font-size: 20px" color="orange">Two-Level Directory</font>

为了适配多用户的操作系统，我们将 Directory 分为 *Master File Directory* 和 *User File Directory* 两级：

![[topic5_3.png]]

其中，主文件目录记录了 `<用户名, UFD 地址>`，用户文件目录记录了该用户所有文件的 FCB。

两级目录提高了检索的速度，解决了多用户之间的文件重名问题，但是缺乏灵活性。

<font style="font-weight: 1000;font-size: 20px" color="orange">Tree-Structured Directory</font>

当用户要访问某个文件时，用文件的路径名表示文件。每个用户都有各自的当前目录，登录后自动进入该用户的当前目录。

![[topic5_4.png]]

树形目录结构可以很方便地对文件进行分类，层次结构清晰，对文件的管理和保护更有效。

但是，在树形目录中查找一个文件，需要按路径逐级访问中间节点，也会影响查询速度。目前大多数操作系统都采用了该结构。

<font style="font-weight: 1000;font-size: 20px" color="orange">Acyclic-Graph Directory</font>

树形目录结构虽然实现了文件分类，但不便于实现文件共享。为此，我们在树形基础上增加了一些指向同一节点的有向边，使得整个目录成为一个*有向无环图*。这种结构允许目录共享子目录或文件，同一文件或子目录可以出现在两个或多个目录中：

![[topic5_5.png]]

!!! note "当然，需要为共享节点设置一个 Count，只有计数器为 0 时才真正删除该节点"

## File System

