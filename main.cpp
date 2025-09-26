#include <functional>
#include <iostream>
#include <chrono>

void plain_func(int& x, int argc)
{
    if (x % argc == 0)
        x += argc;
    else
        x += 1;
}

int main(int argc, char**) 
{
    constexpr int N = 100'000'000; 

    // 1. Raw function pointer
    {
        int x = 0;
        void (*fp)(int&,int) = plain_func;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            fp(x, argc);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Function pointer: "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
                  << " ns total, " 
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N
                  << " ns/call\n";
    }

    // 2. Inline lambda (should inline away)
    {
        int x = 0;
        auto lambda = [](int& y, int argc) 
        { 
            if (y % argc == 0)
                y += argc;
            else
                y += 1;
        };
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            lambda(x, argc);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Lambda (inline) : "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
                  << " ns total, " 
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N
                  << " ns/call\n";
    }

    // 3. std::function
    {
        int x = 0;
        std::function<void(int&,int)> f = plain_func;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            f(x, argc);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "std::function   : "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms total, " 
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N
                  << " ns/call\n";
    }

    // 4. std::bind
    {
        int x = 0;
        auto bound = std::bind(plain_func, std::placeholders::_1, argc);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            bound(x);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "std::bind       : "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms total, " 
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N
                  << " ns/call\n";
    }

    return 0;
}
