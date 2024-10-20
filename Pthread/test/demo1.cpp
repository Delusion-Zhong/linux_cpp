/**
 * @file name
 * @brief   使用智能指针
 * @version 版本
 * @date 日期
 */

#include <iostream>
#include <cstring>
#include <pthread.h>
#include <memory> // 包含智能指针相关的头文件

using namespace std;

typedef struct test
{
    string name;
    int age;
    test(string n, int a) : name(n), age(a) {}
} test;

// 线程回调函数
void *callback(void *arg)
{
    // 将 void * 类型的参数转换为 shared_ptr<test> 类型
    shared_ptr<test> t = *static_cast<shared_ptr<test> *>(arg);

    for (int i = 0; i <= 5; i++)
    {
        cout << "子线程" << i << endl;
    }
    cout << "子线程id:" << pthread_self() << endl;

    // 在子线程中使用智能指针
    cout << "子线程的内容：" << t->name << " " << t->age << endl;

    pthread_exit(nullptr); // 直接退出线程
}

int main()
{
    pthread_t tid;

    // 创建一个 shared_ptr<test> 实例，使用 make_shared
    shared_ptr<test> t = make_shared<test>("张三", 18);

    // 将智能指针的地址传递给线程
    pthread_create(&tid, NULL, callback, &t); // 注意传递的是指针的地址

    cout << "主线程id:" << pthread_self() << endl;

    // 等待子线程结束
    pthread_join(tid, nullptr);

    // 在主线程中访问智能指针的数据
    cout << "主线程中的内容：" << t->name << " " << t->age << endl;

    // 不需要手动删除，智能指针会自动管理内存
    return 0;
}
