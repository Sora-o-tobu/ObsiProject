
# Vue.js

!!! quote "Vue 生命周期"
	![[VuelifeHook.png]]

## 项目结构

一个最基本的 Vue 项目的结构如图：

```bash
Vue_Project
├── build
│   └── ...
├── node_modules
│   └── ...
├── public
│   └── ...
├── src
│   └── ...
├── index.html
├── package.json
└── README.md
```


| 目录/文件         | 说明                                                                                      |
| ------------- | --------------------------------------------------------------------------------------- |
| build         | 项目构建，`npm run build`                                                                    |
| node\_modules | npm 加载的项目依赖                                                                             |
| src           | 开发目录，包含：<br>- assets：放置资源<br>- components：放置vue组件<br>- App.vue：项目入口<br>- main.js：项目核心文件 |
| index.html    | 首页入口，可添加meta信息等                                                                         |
| package.json  | 项目配置                                                                                    |

!!! warning "vue 依赖于 HTTP 服务器，不能通过 `file://` 协议工作，因此一般需要 `Node.js`"

对于上面的文件结构，创建一个 vue 应用的简单示例如下，我们通过 `createApp` 来指定操作的组件：

```js
// main.js
import './assets/main.css'
import { createApp } from 'vue'
import App from './App.vue'

const app = createApp(App)

// mount 代表渲染时，该句表示 id=#app 的块被渲染时，替换为 app 组件实例
app.mount('#app')
```

对应的 `index.html` 如下，其将 body 部分渲染为 `App.vue` 的内容

```html
<!-- index.html -->
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <link rel="icon" href="src/components/icons/logo.png">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>This is the title</title>
  </head>
  <body>
    <div id="app"></div>
    <script type="module" src="/src/main.js"></script>
  </body>
</html>
```

我们暂时只关注vue文件的结构，其它配置文件有需求时请去[官方文档](https://cn.vuejs.org/)中看。

大多数项目的 vue 文件由 **template、script、style** 三个部分组成，可以理解为其与 HTML、JavaScript、CSS 一一对应，它将一个组件的逻辑、模板和样式封装在同一个文件中，因此称为**单文件组件(Single-File Components, SFC)**：

```vue
<template>
  ...
</template>

<script>
  ...
</script>

<style>
  ...
</style>
```

## Template

Vue 使用一种基于 HTML 的模板语法，它能够声明式的将组件实例的数据绑定到 DOM 上。底层机制上，Vue 会将模板编译成优化的 JavaScript 代码。

!!! info "DOM: Document Object Model，是一个网络文档的编程接口"

<font style="font-weight: 1000;font-size: 20px" color="orange">文本插值</font>

文本插值是最基本的数据绑定形式，使用双大括号语法：

```html
<span>Message: {{ msg }}</span>
```

双大括号内的标签会被替换为对应组件实例中的 `msg` 值，每次 `msg` 属性更新时，HTML 显示的内容也会更新。

如果我们想插入一段 HTML，则需要使用 Vue 的 `v-html` 属性：

```html
<p>Using text interpolation: {{ rawHtml }}</p>
<p>Using v-html directive: <span v-html="rawHtml"></span></p>

<!-- Mounted -->
Using text interpolation: <span style="color: red">This should be red.</span>
```

其中文本插值得到的结果如上，而 `v-html` 则会显示该文字为红色。

!!! danger "动态渲染任意 HTML 非常危险，请仅在内容安全时再使用 `v-html` ，并不要使用用户提供的内容"

<font style="font-weight: 1000;font-size: 20px" color="orange">Attribute 绑定</font>

为标签响应式地绑定一个 attribute 需要使用 `v-bind` 指令：

```html
<div v-bind:id="dynamicId"></div>

<!-- 简写 -->
<div :id="dynamicId"></div>
```

如果绑定的值 `dynamicId` 为 `null` 或 `undefined`，则该 attribute 会从渲染的元素上移除。

!!! note "同名简写"
	在 Vue 3.4 版本以上，如果 attribute 名称和绑定的 JavaScript 名称相同，则可以进一步简化语法：
	
	```html
	<!-- 等价于 <div :id="id"></div> -->
	<div :id></div>
	```

如果我们在 JavaScript 中定义了如下包含多个 attribute 对象的 JavaScript 对象：

```js
data() {
  return {
    objectOfAttrs: {
      id: 'container',
      class: 'wrapper'
    }
  }
}
```

那么，通过不带参数的 `v-bind` ，我们可以动态绑定多个属性值到单个元素上：

```html
<div v-bind="objectOfAttrs"></div>
```

<font style="font-weight: 1000;font-size: 20px" color="orange">v-on / @</font>

这个属性为元素绑定一个监听器，一般语法为：

```html
<a v-on:event="doSomething"> ... </a>

<!-- 简写 -->
<a @event="doSomething"> ... </a>
```

常用的 `event` 有 click、focus、submit 等。

!!! note "按键修饰符"
	Vue 允许监听鼠标和键盘按键事件：
	
	```html
	<!-- 仅在 `key` 为 `Enter` 时调用 `submit` -->
	<input @keyup.enter="submit" />
	
	<!-- 仅当按下 Ctrl 且未按任何其他键时才会触发 --> 
	<button @click.ctrl.exact="onCtrlClick">A</button>
	```

Vue 为 `v-on` 提供了一系列事件修饰符，包含：

- `.stop`
- `.prevent`
- `.self`
- `.capture`
- `.once`
- `.capture`

```html
<!-- 单击事件将停止传递 -->
<a @click.stop="doThis"></a>

<!-- 提交事件将不再重新加载页面 -->
<form @submit.prevent="onSubmit"></form>

<!-- 修饰语可以使用链式书写 -->
<a @click.stop.prevent="doThat"></a>

<!-- 也可以只有修饰符 -->
<form @submit.prevent></form>

<!-- 仅当 event.target 是元素本身时才会触发事件处理器 -->
<!-- 例如：事件处理器不来自子元素 -->
<div @click.self="doThat">...</div>

<!-- 添加事件监听器时，使用 `capture` 捕获模式 -->
<!-- 例如：指向内部元素的事件，在被内部元素处理前，先被外部处理 -->
<div @click.capture="doThis">...</div>

<!-- 点击事件最多被触发一次 -->
<a @click.once="doThis"></a>

<!-- 滚动事件的默认行为 (scrolling) 将立即发生而非等待 `onScroll` 完成 -->
<!-- 以防其中包含 `event.preventDefault()` -->
<!-- 改善移动端滑屏性能 -->
<div @scroll.passive="onScroll">...</div>
```


<font style="font-weight: 1000;font-size: 20px" color="orange">v-if & v-else</font>

这两个属性控制一个元素是否显示，例如：

```html
<button @click="seen = !seen">Toggle</button>

<p v-if="seen">Now you see me</p>
<p v-else>Oh no 😢</p>
```

!!! info "`v-else` 必须跟在 `v-if` 或 `v-else-if` 后面"

<font style="font-weight: 1000;font-size: 20px" color="orange">v-show</font>

另一个用来按条件显示的指令是 `v-show`，其基本用法和 `v-if` 相同：

```html
<h1 v-show="ok">Hello!</h1>
```

区别在于，`v-show` 仅切换了 `display` 的 CSS 属性，并且不支持加在 `<template>` 上。

总的来说，`v-if` 有更高的切换开销，而 `v-show` 有更高的初始渲染开销，需要根据需求来选择哪个条件渲染。

<font style="font-weight: 1000;font-size: 20px" color="orange">v-for</font>

`v-for` 指令基于一个数组来渲染一个列表，需要用到 `item in items` 的特殊语法。

```js
data() {
  return {
    parentMessage: 'Parent',
    items: [{ message: 'Foo' }, { message: 'Bar' }]
  }
}
```

```html
<li v-for="(item, index) in items">
  {{ parentMessage }} - {{ index }} - {{ item.message }}
</li>
```

渲染结果如下：

- Parent - 0 - Foo
- Parent - 1 - Bar

使用 `value`, `key`, `index` 三个关键字遍历一个对象的所有属性：

```js
data() {
  return {
    myObject: {
      title: 'How to do lists in Vue',
      author: 'Jane Doe',
      publishedAt: '2016-04-10'
    }
  }
}
```

```html
<li v-for="(value, key, index) in myObject">
  {{ index }}. {{ key }}: {{ value }}
</li>
```

如果 `v-for` 和 `v-if` 同时存在一个元素上时，`v-if` 的优先级更高。这也意味着 `v-if` 无法访问到 `v-for` 作用域里定义的变量别名。推荐实践为在外面包装一层 `<template v-for=''>`：

```html
<!-- 根据 name 属性排序渲染 -->
<template v-for="todo in todos" :key="todo.name">
  <li v-if="!todo.isComplete">
    {{ todo.name }}
  </li>
</template>
```

<font style="font-weight: 1000;font-size: 20px" color="orange">v-model</font>

`v-model` 简化了接值绑定和事件监听，并且支持各种不同类型的输入。

```html
<!-- 单行文本直接用 input -->
<p>Message is: {{ message }}</p>
<input v-model="message" placeholder="edit me" />


<!-- 多行文本用 textarea -->
<span>Multiline message is:</span>
<p style="white-space: pre-line;">{{ message }}</p>
<textarea v-model="message" placeholder="add multiple lines"></textarea>
```

单个简单选择器：

```html
<div>Selected: {{ selected }}</div>

<select v-model="selected">
  <option disabled value="">Please select one</option>
  <option>A</option>
  <option>B</option>
  <option>C</option>
</select>
```

!!! info "radio：单选按钮，checkbox：复选框，select：选择器"

`v-model` 的[修饰符](https://cn.vuejs.org/guide/essentials/forms.html#modifiers)。


## Script

在选项式 API 中，我们用 `data` 声明组件的响应式状态，它会返回一个对象的函数。Vue 在创建新组件实例时调用此函数，此对象的所有顶层属性都会被代理到组件实例上（即方法中的 `this` 上）。

因此，对于组件实例中需要用到的响应式变量，都应在 `data` 中进行初始化，若所需的值还未准备好，则用 `null`, `undefined` 等值占位。

```js
export default {
  data() {
    return {
      count: 1
    }
  },

  methods: {
    increment() {
      this.count++
    }
  },

  mounted() {
    // `this` 指向当前组件实例
    console.log(this.count) // => 1

	this.increment()  // count++

    this.count = 2
  }
}
```

!!! note "Vue 内置 API 使用 `$` 为前缀，同时为内部属性保留 `_` 前缀，我们要避免使用这两个前缀命名的属性"

TBD：箭头函数


## 基础话题

### 动态参数与计算属性

对于指令的参数，我们可以使用一个 JavaScript 表达式作为动态参数，需要将其包含在一对方括号内：

```html
<!-- v-bind -->
<a v-bind:[attributeName]="url"> ... </a>

<!-- 简写 -->
<a :[attributeName]="url"> ... </a>

<!-- v-on -->
<a v-on:[eventName]="doSomething"> ... </a>

<!-- 简写 -->
<a @[eventName]="doSomething"> ... </a>
```

但需要注意，动态参数语法存在一些限制，不能使用空格、引号等：

```html
<!-- 这会触发一个编译器警告 -->
<a :['foo' + bar]="value"> ... </a>
```

如果我们想传入复杂的动态属性，推荐使用**计算属性**来替换复杂的表达式，例如：

```html
<!-- In HTML -->
<p>Has published books:</p>
<span>{{ publishedBooksMessage }}</span>
```

```js
export default {
  data() {
    return {
      author: {
        name: 'John Doe',
        books: [
          'Vue 2 - Advanced Guide',
          'Vue 3 - Basic Guide',
          'Vue 4 - The Mystery'
        ]
      }
    }
  },
  
  /* 放在 methods 中作为方法，也能得到同样的效果
  * 区别在于，计算属性会被缓存，只有其响应式依赖更新时才重新计算
  * 而方法则在每次调用 getter 中都重复计算
  */
  computed: {
    // 一个计算属性的 getter
    publishedBooksMessage() {
      // `this` 指向当前组件实例
      return this.author.books.length > 0 ? 'Yes' : 'No'
    }
  }
}
```

??? question "计算属性默认只读，可以设置一个 setter 来使其可写"
	```js
	export default {
	  data() {
	    return {
	      firstName: 'John',
	      lastName: 'Doe'
	    }
	  },
	  computed: {
	    fullName: {
	      // getter
	      get() {
	        return this.firstName + ' ' + this.lastName
	      },
	      // setter
	      set(newValue) {
	        // 注意：我们这里使用的是解构赋值语法
	        [this.firstName, this.lastName] = newValue.split(' ')
	      }
	    }
	  }
	}
	```

### 动态 Class

`v-bind` 的一个常用场景是操纵元素的 class 和 style，它们都是 attribute，因此都可以和动态字符串绑定。Vue 还为 class 和 style 提供专属的功能增强。

`v-bind:class` 可以与一般的 `class` 共存，且可以通过传递对象来切换值，下面是一个典型案例：

```js
data() {
  return {
    isActive: true,
    hasError: true
  }
}
```

```html
<div
  class="static"
  :class="{ active: isActive, 'text-danger': hasError }"
></div>
```

渲染结果为：

```html
<div class="static active text-danger"></div>
```

当然，你也可以直接为 `class` 绑定一个对象或计算属性：

```js
// 对象 ver
data() {
  return {
    classObject: {
      active: true,
      'text-danger': false
    }
  }
}

// 计算属性 ver
data() {
  return {
    isActive: true,
    error: null
  }
},
computed: {
  classObject() {
    return {
      active: this.isActive && !this.error,
      'text-danger': this.error && this.error.type === 'fatal'
    }
  }
}
```

```html
<div :class="classObject"></div>

<!-- 渲染结果 -->
<div class="active"></div>
```

`style` 也是同理。


### 响应式基础

Vue 默认情况采用深度响应状态，即在方法或者其它地方对对象进行修改时，该变化会被立即检测到并应用。

一种别的状态使用 `nextTick()` API，在 next Tick 更新周期中的所有修改会被缓存，等到时钟改变时再进行更新，这确保了不管进行多次修改，每个组件都只会被更新一次：

```js
import { nextTick } from 'vue'

export default {
  methods: {
    async increment() {
      this.count++
      await nextTick()
      // 现在 DOM 已经更新了
    }
  }
}
```

在某些情况下，可能需要动态创建一个方法，例如创建一个点击防抖的时间处理器。由于一个方法可能对多个实例共享一个状态，那么它们之间可能存在相互影响。为了避免这种情况，可以在 `created` 生命周期钩子中为每个实例都创建：

```js
export default {
  created() {
    // 每个实例都有了自己的预置防抖的处理函数
    this.debouncedClick = _.debounce(this.click, 500)
  },
  unmounted() {
    // 最好是在组件卸载时
    // 清除掉防抖计时器
    this.debouncedClick.cancel()
  },
  methods: {
    click() {
      // ... 对点击的响应 ...
    }
  }
}
```

### 侦听器

侦听器类似于数据库的触发器，一般以我们需要侦听变量作为名字，在*选项式API*中使用 `watch` 选项：

```js
export default {
  data() {
    return {
      question: '',
      answer: 'Questions usually contain a question mark. ;-)',
      loading: false
    }
  },
  watch: {
    // 每当 question 改变时，这个函数就会执行
    question(newQuestion, oldQuestion) {
      if (newQuestion.includes('?')) {
        this.getAnswer()
      }
    }
  },
  methods: {
    async getAnswer() {
      this.loading = true
      this.answer = 'Thinking...'
      try {
        const res = await fetch('https://yesno.wtf/api')
        this.answer = (await res.json()).answer
      } catch (error) {
        this.answer = 'Error! Could not reach the API. ' + error
      } finally {
        this.loading = false
      }
    }
  }
}
```

侦听器默认时浅层的，即对于嵌套对象（例如数组），在其嵌套属性被改变时不会触发侦听器。如果想要增加侦听的层数，则需要使用 `deep` 参数：

```js
export default {
  watch: {
    someObject: {
      handler(newValue, oldValue) {
        // 注意：在嵌套的变更中，
        // 只要没有替换对象本身，
        // 那么这里的 `newValue` 和 `oldValue` 相同
      },
      deep: 2
    }
  }
}
```

!!! info "`deep` 设为 true 的时候表示侦听所有嵌套变更，当用于大型数据结构时往往带来巨大开销，要注意设置"


## 组件

假设我们写了一个表示 Blog 文章的组件 BlogPost.vue，在我们的博客上，我们需要重复渲染这个组件，但是其内容和标题是不一样的。要实现这样的效果需要使用 `props` 选项：

```vue
<!-- BlogPost.vue -->
<script>
export default {
  props: ['title'],
  emits: ['enlarge-text']
}
</script>

<template>
  <div class="blog-post">
	  <h4>{{ title }}</h4>
	  <button @click="$emit('enlarge-text')">Enlarge text</button>
  </div>
</template>
```

```vue
<!-- App.vue -->
<script>
import BlogPost from './BlogPost.vue'
  
export default {
  components: {
    BlogPost
  },
  data() {
    return {
      posts: [
        { id: 1, title: 'My journey with Vue' },
        { id: 2, title: 'Blogging with Vue' },
        { id: 3, title: 'Why Vue is so fun' }
      ],
      postFontSize: 1
    }
  }
}
</script>

<template>
  <div :style="{ fontSize: postFontSize + 'em' }">
    <BlogPost
      v-for="post in posts"
      :key="post.id"
      :title="post.title"
      @enlarge-text="postFontSize += 0.1"
    ></BlogPost>
  </div>
</template>
```

除此之外，它还通过内置的 `$emit` 方法来抛出一个事件，实现了一个可以增加博客文章字体大小的按钮。上述代码可以在[演练场](https://play.vuejs.org/#eNqNVO9r2zAQ/VcOMXDLknhZ+8m4oevIYINtZR3bh7kfXFtOlMiSkc5pMpP/fSc5/lEoYSEY3733zu/OJzfsQ1XNdjVnEYttZkSFi0SJstIG4U7q1b22CIXRJQSzsEs4QZAogETxvafmvEhridC4bKZJr7hCG7UJ6Eu56Dhx1zzF9OKyww3H2qguAqiIS+o/XQzQgMgjmE8ABUoeQfD1ABtNIn6AZ4Fr+EWeTrXHiveDwplYCbU6y78a+L/XB8cCYcFqKGpF/I7+2Aud1U9a4YP4S6J5m/Y8utA/Dvu5UoC8rGSKnCKAOBc7iCweJL9JWANFX2ZcFN5CwEv3bOZVpBuP0/1200IbKuFkIFQ7voR1cLTlhxM6E/ko7zvtEB8M4C1XMjUrPkW+xxNnsHQD72bzjryI+9Vo+wqpMbqLw75dNmHj7Xmxbq/tUGV05RYg8K6Cdty8FH4rgrG14PG/xpzJ1Fpq44lcTF0vfpoJEry+XjRN+9bheIxDik/IU42oFdxmUmRbEr9xDi5ePv6SCi3bBLhEHLaqM5NAm2lViNVsY7WiUfiOE+bOjZDcfK9QaEXvrz8+CUul1M9ffA5NzU/bR5o1z7av5Dd273IJuzfccrOjF9tj6LxS/w5ePnwjzyOw1Hnt1uAM+INbLWvnsaXd1Son2yOed/vZf0XovP20yz1yZbumnNHhQ5Aw2oePZ1of7F7NrrtzxY7/AIr7iNc=)进行调试。

有些场景可能需要组件动态切换，例如点击按钮切换不同的组件显示，此时需要用到 Vue 的 `<component>` 元素和 `is` Attribute：

```vue
<script>
import Home from './Home.vue'
import Posts from './Posts.vue'
import Archive from './Archive.vue'
  
export default {
  components: {
    Home,
    Posts,
    Archive
  },
  data() {
    return {
      currentTab: 'Home',
      tabs: ['Home', 'Posts', 'Archive']
    }
  }
}
</script>

<template>
  <div class="demo">
    <button
       v-for="tab in tabs"
       :key="tab"
       :class="['tab-button', { active: currentTab === tab }]"
       @click="currentTab = tab"
     >
      {{ tab }}
    </button>
	  <component :is="currentTab" class="tab"></component>
  </div>
</template>
```

!!! info "被切换的组件会被卸载，可以通过 [KeepAlive](https://cn.vuejs.org/guide/built-ins/keep-alive.html) 来强制保持“存活”状态"

`is` 属性还可以用来避免元素位置的限制，例如 `<ul>`，`<ol>`，`<table>` 和 `<select>`，相应的，某些元素仅在放置于特定元素中时才会显示，例如 `<li>`，`<tr>` 和 `<option>`。

这使得直接使用组件会出现问题，例如：

```vue
<table>
  <blog-post-row></blog-post-row>
</table>
```

自定义组件 `blog-post-row` 将作为无效内容被忽略。因此我们使用：

```vue
<table>
  <tr is="vue:blog-post-row"></tr>
</table>
```

