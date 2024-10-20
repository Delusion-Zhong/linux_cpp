#include <iostream>
#include <cstring>
#include <pthread.h>
#include <memory>
using namespace std;
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
    // 返回给主进程
    test *t = new test("张三", 18);
    pthread_exit(t);
    delete t;
};
int main()
{

    pthread_t tid;
    pthread_create(&tid, NULL, callback, NULL);
    cout << "主线程id:" << pthread_self() << endl;

    // void *ptr;
    // extern int pthread_join (pthread_t __th, void **__thread_return);
    //!  因为返回值是一个 双指针类型 ，下面因该有一个 指针的地址来接收
    // pthread_join(tid, &ptr);
    /*
    void * 的通用性： void * 是一种通用指针类型，可以指向任何数据类型，但它并没有包含类型信息。
    这意味着编译器不知道 void * 实际指向的是什么类型的数据。因此，您在使用 void * 指针时，
    需要将其转换为实际的数据类型指针（如 test *）。
    */
    // struct test *t = (test *)ptr;

    // cout << "子线程的内容：" << t->name << t->age << endl;
    // delete t;

    pthread_detach(tid);
    pthread_exit(NULL);
    return 0;
}
