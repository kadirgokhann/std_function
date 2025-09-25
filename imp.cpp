#include <memory>
#include <utility>
#include <exception>

template<typename>
class function; // primary template (unspecified)

template<typename R, typename... Args>
class function<R(Args...)> {
    struct vtable_t {
        R (*invoke)(void*, Args&&...);
        void (*destroy)(void*);
        void* (*clone)(void*);
    };

    template<typename F>
    struct functor_holder {
        F functor;

        static R invoke(void* obj, Args&&... args) {
            return (*static_cast<F*>(obj))(std::forward<Args>(args)...);
        }
        static void destroy(void* obj) {
            delete static_cast<F*>(obj);
        }
        static void* clone(void* obj) {
            return new F(*static_cast<F*>(obj));
        }
    };

    void* obj = nullptr;
    vtable_t const* vtable = nullptr;

public:
    function() = default;

    template<typename F>
    function(F f) {
        using holder = functor_holder<F>;
        obj = new F(std::move(f));
        static vtable_t vt = {
            &holder::invoke,
            &holder::destroy,
            &holder::clone
        };
        vtable = &vt;
    }

    function(const function& other) {
        if (other.obj) {
            obj = other.vtable->clone(other.obj);
            vtable = other.vtable;
        }
    }

    function& operator=(const function& other) {
        if (this != &other) {
            if (obj) vtable->destroy(obj);
            if (other.obj) {
                obj = other.vtable->clone(other.obj);
                vtable = other.vtable;
            } else {
                obj = nullptr;
                vtable = nullptr;
            }
        }
        return *this;
    }

    ~function() {
        if (obj) vtable->destroy(obj);
    }

    R operator()(Args... args) const {
        if (!obj) throw std::bad_function_call();
        return vtable->invoke(obj, std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept { return obj != nullptr; }
};
