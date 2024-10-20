# 多线程

## 线程函数

包含文件 `#include <pthread.h>`

### 线程

#### 线程创建(pthread_create)

每一个线程都有一个唯一的线程 ID，ID 类型为` pthread_t`，这个 ID 是一个无符号长整形数，如果想要得到当前线程的线程 ID，可以调用如下函数：

创建线程 id

```
pthread_t pthread_self(void);	// 返回当前线程的线程ID
```

原型如下：

```
typedef unsigned long int pthread_t;
```

在一个进程中调用线程创建函数，就可得到一个子线程，和进程不同，需要给每一个创建出的线程指定一个处理函数，否则这个线程无法工作。

函数原型：

```
extern int pthread_create (pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg) __THROWNL __nonnull ((1, 3));
```

第一种解释

参数 1： 线程 ID，由函数 `pthread_self()`获取， 返回值为调用线程的线程 ID。

参数 2：用于定制各种不同的线程属性，暂可以把它设置为 NULL，以创建默认属性的线程；

参数 3(类似于创建一个子线程)：线程中执行函数。新创建的线程从 `start_rtn` 函数的地址开始运行，该函数只有一个无类型指针参数 `arg`

参数 4：执行函数中中参数。如果需要向`start_rtn`函数传递的参数不止一个，那么需要把这些参数放到一个结构体中，然后把这个结构的地址作为` arg` 参数传入

```
    void *callback(void *arg){}
   pthread_create(&tid, NULL, callback, NULL(传出参数));
```

第二种解释

thread: 传出参数，是无符号长整形数，线程创建成功, 会将线程 ID 写入到这个指针指向的内存中

attr: 线程的属性, 一般情况下使用默认属性即可, 写 NULL

start_routine: 函数指针，创建出的子线程的处理动作，也就是该函数在子线程中执行。

arg: 作为实参传递到 start_routine 指针指向的函数内部

返回值：线程创建成功返回 0，创建失败返回对应的错误号
、

#### 创建子线程

```
extern int pthread_create (pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg) __THROWNL __nonnull ((1, 3));
    //也就是上面的第三个参数
void *(*__start_routine) (void *),

```

创建线程也就是上面的第三个参数 不要要注意在创建过程中一定要保证编写的线程函数与规定的函数指针类型一致`void *(*__start_routine) (void *),`
类似于创建了一个函数 例如

```
typedef struct test
{
    string name;
    int age;
    test(string n, int a) : name(n), age(a) {}
} test;
void *callback(void *arg)
{
    for (int i = 0; i <= 5; i++)
    {
        cout << "子线程" << i << endl;
    }
    cout << "子线程id:" << pthread_self() << endl;
    test *t = new test("张三", 18);

    pthread_exit(t);
    delete t;
};
int main{
    pthread tid;
    pthread_create(&tid, NULL, callback, NULL);
}
```

```
子线程被创建出来之后需要抢 cpu 时间片, 抢不到就不能运行，如果主线程退出了, 虚拟地址空间就被释放了, 子线程就一并被销毁了。但是如果某一个子线程退出了, 主线程仍在运行, 虚拟地址空间依旧存在。
```

#### 线程退出(pthread_exit)

在编写多线程程序的时候，如果想要让线程退出，但是不会导致虚拟地址空间的释放（针对于主线程），我们就可以调用线程库中的线程退出函数，只要调用该函数当前线程就马上退出了，并且不会影响到其他线程的正常运行，不管是在子线程或者主线程中都可以使用。

单个线程可以通过以下三种方式退出，在不终止整个进程的情况下停止它的控制流：

1）线程只是从启动例程中返回，返回值是线程的退出码。

2）线程可以被同一进程中的其他线程取消。

3）线程调用 `pthread_exit`：

```
函数 原型

extern void pthread_exit (void *__retval) __attribute__ ((__noreturn__));


int pthread_exit(void **rval_ptr);
```

参数: 线程退出的时候携带的数据，当前子线程的主线程会得到该数据。如果不需要使用，指定为 NULL

`rval_ptr：`是一个无类型指针，与传给启动例程的单个参数类似。进程中的其他线程可以通过调用 `pthread_join` 函数访问到这个指针。

#### 线程回收(pthread_join)

线程和进程一样，子线程退出的时候其内核资源主要由主线程回收，线程库中提供的线程回收函叫做 pthread_join()，这个函数是一个阻塞函数，如果还有子线程在运行，调用该函数就会阻塞，子线程退出函数解除阻塞进行资源的回收，函数被调用一次，只能回收一个子线程，如果有多个子线程则需要循环进行回收。

另外通过线程回收函数还可以获取到子线程退出时传递出来的数据，函数原型如下：

```
#include <pthread.h>
// 这是一个阻塞函数, 子线程在运行这个函数就阻塞
// 子线程退出, 函数解除阻塞, 回收对应的子线程资源, 类似于回收进程使用的函数 wait()
int pthread_join(pthread_t thread, void **retval);

```

参数:
thread: 要被回收的子线程的线程 ID

retval: 二级指针, 指向一级指针的地址, 是一个传出参数, 这个地址中存储了 pthread_exit() 传递出的数据，如果不需要这个参数，可以指定为 NULL

返回值：线程回收成功返回 0，回收失败返回错误号。

#### 回收子线程数据

在子线程退出的时候可以使用 pthread_exit()的参数将数据传出，在回收这个子线程的时候可以通过 phread_join()的第二个参数来接收子线程传递出的数据。接收数据有很多种处理方式，下面来列举几种：

##### 使用子线程栈

通过函数 pthread_exit(void *retval);可以得知，子线程退出的时候，需要将数据记录到一块内存中，通过参数传出的是存储数据的内存的地址，而不是具体数据，由因为参数是 void*类型，所有这个万能指针可以指向任意类型的内存地址。先来看第一种方式，将子线程退出数据保存在子线程自己的栈区：

```
    pthread_join(tid, &ptr);
   struct test *t = (test *)ptr;
    cout << "子线程的内容：" << t->name << t->age << endl;
```

因为 `pthread_join(pthread_t thread, void **retval);` 的第二个参数是一个 （void \*）类型。 任何类型都能使用, 因此在这里 `必须使用强制转换 struct test _t = (test _)ptr;`

##### 使用全局变量

位于同一虚拟地址空间中的线程，虽然不能共享栈区数据，但是可以共享全局数据区和堆区数据，因此在子线程退出的时候可以将传出数据存储到全局变量、静态变量或者堆内存中。

```
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

// 定义结构
struct Persion
{
    int id;
    char name[36];
    int age;
};

struct Persion p;	// 定义全局变量

// 子线程的处理代码
void* working(void* arg)
{
    printf("我是子线程, 线程ID: %ld\n", pthread_self());
    for(int i=0; i<9; ++i)
    {
        printf("child == i: = %d\n", i);
        if(i == 6)
        {
            // 使用全局变量
            p.age  =12;
            strcpy(p.name, "tom");
            p.id = 100;
            // 该函数的参数将这个地址传递给了主线程的pthread_join()
            pthread_exit(&p);
        }
    }
    return NULL;
}

int main()
{
    // 1. 创建一个子线程
    pthread_t tid;
    pthread_create(&tid, NULL, working, NULL);

    printf("子线程创建成功, 线程ID: %ld\n", tid);
    // 2. 子线程不会执行下边的代码, 主线程执行
    printf("我是主线程, 线程ID: %ld\n", pthread_self());
    for(int i=0; i<3; ++i)
    {
        printf("i = %d\n", i);
    }

    // 阻塞等待子线程退出
    void* ptr = NULL;
    // ptr是一个传出参数, 在函数内部让这个指针指向一块有效内存
    // 这个内存地址就是pthread_exit() 参数指向的内存
    pthread_join(tid, &ptr);
    // 打印信息
    struct Persion* pp = (struct Persion*)ptr;
    printf("name: %s, age: %d, id: %d\n", pp->name, pp->age, pp->id);
    printf("子线程资源被成功回收...\n");

    return 0;
}


```

##### 使用主线程栈

虽然每个线程都有属于自己的栈区空间，但是位于同一个地址空间的多个线程是可以相互访问对方的栈空间上的数据的。由于很多情况下还需要在主线程中回收子线程资源，所以主线程一般都是最后退出，基于这个原因在下面的程序中将子线程返回的数据保存到了主线程的栈区内存中

```
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

// 定义结构
struct Persion
{
    int id;
    char name[36];
    int age;
};

struct Persion p;	// 定义全局变量

// 子线程的处理代码
void* working(void* arg)
{
    printf("我是子线程, 线程ID: %ld\n", pthread_self());
    for(int i=0; i<9; ++i)
    {
        printf("child == i: = %d\n", i);
        if(i == 6)
        {
            // 使用全局变量
            p.age  =12;
            strcpy(p.name, "tom");
            p.id = 100;
            // 该函数的参数将这个地址传递给了主线程的pthread_join()
            pthread_exit(&p);
        }
    }
    return NULL;
}

int main()
{
    // 1. 创建一个子线程
    pthread_t tid;
    pthread_create(&tid, NULL, working, NULL);

    printf("子线程创建成功, 线程ID: %ld\n", tid);
    // 2. 子线程不会执行下边的代码, 主线程执行
    printf("我是主线程, 线程ID: %ld\n", pthread_self());
    for(int i=0; i<3; ++i)
    {
        printf("i = %d\n", i);
    }

    // 阻塞等待子线程退出
    void* ptr = NULL;
    // ptr是一个传出参数, 在函数内部让这个指针指向一块有效内存
    // 这个内存地址就是pthread_exit() 参数指向的内存
    pthread_join(tid, &ptr);
    // 打印信息
    struct Persion* pp = (struct Persion*)ptr;
    printf("name: %s, age: %d, id: %d\n", pp->name, pp->age, pp->id);
    printf("子线程资源被成功回收...\n");

    return 0;
}

```

# 优秀文章链接及应用

1.  https://blog.csdn.net/m0_74985965/article/details/128815940?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522B94738BE-BE78-4224-A0D0-E60619FF68E5%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=B94738BE-BE78-4224-A0D0-E60619FF68E5&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-128815940-null-null.142^v100^pc_search_result_base8&utm_term=pthread_create&spm=1018.2226.3001.4187

2.  API 调用

    https://blog.csdn.net/jue_1xin/article/details/126803262?ops_request_misc=%257B%2522request%255Fid%2522%253A%252211A5A20D-8D08-46C2-AA8C-ACE88EF92851%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=11A5A20D-8D08-46C2-AA8C-ACE88EF92851&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_click~default-1-126803262-null-null.142^v100^pc_search_result_base8&utm_term=pthread_self&spm=1018.2226.3001.4187

3.  https://subingwen.cn/linux/thread/
