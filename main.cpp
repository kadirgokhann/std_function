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

int main(int argc, char** ) 
{
    constexpr int N = 100'000'000; 

    // 1. Raw function pointer
    {
        int x = 0;
        void (*fp)(int&,int) = plain_func;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            fp(x,argc);

        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Function pointer: "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
                  << " nanosecs in total, " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N
                  << " ns per call  "
                  << std::endl;
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
            lambda(x ,argc);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Lambda (inline) : "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
                  << " nanosecs in total, " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N
                  << " ns per call  "
                  << std::endl;
    }

    // 3. std::function
    {
        int x = 0;
        std::function<void(int&,int)> f = plain_func;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            f(x,argc);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "std::function   : "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " milliseconds in total, " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N
                  << " ns per call  "
                  << std::endl;
    }

    return 0;
}
