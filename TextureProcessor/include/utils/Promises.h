#pragma once
#include <coroutine>

namespace ver {

    template <class>
    inline constexpr bool always_false = false;
    
    //noexcept generator
    template <class T>
    struct generator {
        struct promise_type 
        {
            ver::generator<T> get_return_object() noexcept {
                return generator<T>{ *this };
            }
            std::suspend_always initial_suspend() noexcept {
                return {};
            }
            std::suspend_always final_suspend() noexcept {
                return {};
            }
            std::suspend_always yield_value(const T& _Val) noexcept {
                xvalue = std::addressof(_Val);
                return {};
            }

            void unhandled_exception() noexcept {}
            void return_void() noexcept {}
            const T* value()const noexcept { return xvalue; }

            template <class U>
            void await_transform(U&& _Whatever) {
                static_assert(always_false<U>, "co_await is not supported in coroutines of type std::experimental::generator");
            }
        private:
            const T* xvalue;
        };

        struct iterator {
            using iterator_category = std::input_iterator_tag;
            using difference_type = ptrdiff_t;
            using value_type = T;
            using reference = const T&;
            using pointer = const T*;

            std::coroutine_handle<promise_type> handle = nullptr;

            iterator() = default;
            explicit iterator(std::coroutine_handle<promise_type> handle_) noexcept : handle(handle_) {}

            iterator& operator++() {
                handle.resume();
                if (handle.done()) {
                    handle = nullptr;
                }
                return *this;
            }
            bool operator==(const iterator& _Right) const noexcept {
                return handle == _Right.handle;
            }
            reference operator*() const noexcept {
                return *handle.promise().value();
            }
            pointer operator->() const noexcept {
                return handle.promise().value();
            }
        };
        iterator begin() {
            if (handle) {
                handle.resume();
                if (handle.done()) 
                    return {};
            }
            return iterator{ handle };
        }
        iterator end() noexcept {
            return {};
        }

        explicit generator(promise_type& _Prom) noexcept : handle(std::coroutine_handle<promise_type>::from_promise(_Prom)) {}
        generator() = default;
        generator(generator&& _Right) noexcept : handle(std::exchange(_Right.handle, nullptr)) {}

        generator& operator=(generator&& _Right) noexcept {
            handle = std::exchange(_Right.handle, nullptr);
            return *this;
        }
        ~generator() {
            if (handle) 
                handle.destroy();
        }
        bool finished()const
        {
            return !handle || handle.done();
        }
    private:
        std::coroutine_handle<promise_type> handle = nullptr;
    };
} 