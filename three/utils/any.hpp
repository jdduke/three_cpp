#ifndef THREE_ANY_HPP
#define THREE_ANY_HPP

#include <stdexcept>

namespace three {

// High Performance Heterogeneous Container
//  http://www.codeproject.com/Articles/23304/High-Performance-Heterogeneous-Container

namespace detail {

struct bad_any_cast { };

struct empty_any { };

struct base_any_policy {
    virtual void static_delete(void** x) = 0;
    virtual void copy_from_value(void const* src, void** dest) = 0;
    virtual void clone(void* const* src, void** dest) = 0;
    virtual void move(void* const* src, void** dest) = 0;
    virtual void* get_value(void** src) = 0;
    virtual const void* get_value(void* const* src) const = 0;
    virtual size_t get_size() = 0;
};

template<typename T>
struct typed_base_any_policy : base_any_policy {
    virtual size_t get_size() { return sizeof(T); }
};

template<typename T>
struct small_any_policy : typed_base_any_policy<T> {
    virtual void static_delete(void** x) { }
    virtual void copy_from_value(void const* src, void** dest)
    { new(dest) T(*reinterpret_cast<T const*>(src)); }
    virtual void clone(void* const* src, void** dest) { *dest = *src; }
    virtual void move(void* const* src, void** dest) { *dest = *src; }
    virtual void* get_value(void** src) { return reinterpret_cast<void*>(src); }
    virtual const void* get_value(void* const* src) const { return reinterpret_cast<const void*>(src); }
};

template<typename T>
struct big_any_policy : typed_base_any_policy<T> {
    virtual void static_delete(void** x) { if (*x)
        delete(*reinterpret_cast<T**>(x)); *x = NULL; }
    virtual void copy_from_value(void const* src, void** dest) {
       *dest = new T(*reinterpret_cast<T const*>(src)); }
    virtual void clone(void* const* src, void** dest) {
       *dest = new T(**reinterpret_cast<T* const*>(src)); }
    virtual void move(void* const* src, void** dest) {
      (*reinterpret_cast<T**>(dest))->~T();
      **reinterpret_cast<T**>(dest) = **reinterpret_cast<T* const*>(src); }
    virtual void* get_value(void** src) { return *src; }
    virtual const void* get_value(void* const* src) const { return *src; }
};

template<typename T>
struct choose_policy {
    typedef big_any_policy<T> type;
};

template<typename T>
struct choose_policy<T*> {
    typedef small_any_policy<T*> type;
};

struct any;

/// Choosing the policy for an any type is illegal, but should never happen.
/// This is designed to throw a compiler error.
template<>
struct choose_policy<any> {
    typedef void type;
};

/// Specializations for small types.
#define SMALL_POLICY(TYPE) template<> struct \
   choose_policy<TYPE> { typedef small_any_policy<TYPE> type; };

SMALL_POLICY(signed char);
SMALL_POLICY(unsigned char);
SMALL_POLICY(signed short);
SMALL_POLICY(unsigned short);
SMALL_POLICY(signed int);
SMALL_POLICY(unsigned int);
SMALL_POLICY(signed long);
SMALL_POLICY(unsigned long);
SMALL_POLICY(float);
SMALL_POLICY(bool);

#undef SMALL_POLICY

/// This function will return a different policy for each type.
template<typename T>
base_any_policy* get_policy() {
    static typename choose_policy<T>::type policy;
    return &policy;
};

}

struct any {
private:
    detail::base_any_policy* policy;
    void* object;

public:
    /// Initializing constructor.
    template <typename T>
    any(const T& x) : policy(detail::get_policy<detail::empty_any>()), object(NULL) {
        assign(x);
    }

    /// Empty constructor.
    explicit any() : policy(detail::get_policy<detail::empty_any>()), object(NULL) { }

    /// Special initializing constructor for string literals.
    explicit any(const char* x): policy(detail::get_policy<detail::empty_any>()), object(NULL) {
        assign(x);
    }

    /// RValue constructor
    explicit any(any&& x) : policy(detail::get_policy<detail::empty_any>()), object(NULL) {
      swap(x);
    }

    /// Copy constructor.
    explicit any(const any& x) : policy(detail::get_policy<detail::empty_any>()), object(NULL) {
        assign(x);
    }

    /// Destructor.
    ~any() {
        policy->static_delete(&object);
    }

    /// Assignment function from another any.
    any& assign(const any& x) {
        reset();
        policy = x.policy;
        policy->clone(&x.object, &object);
        return *this;
    }

    /// Assignment function.
    template <typename T>
    any& assign(const T& x) {
        reset();
        policy = detail::get_policy<T>();
        policy->copy_from_value(&x, &object);
        return *this;
    }

    /// Assignment operator.
    template<typename T>
    any& operator=(const T& x) {
        return assign(x);
    }

    /// Assignment operator, specialed for literal strings.
    /// They have types like const char [6] which don't work as expected.
    any& operator=(const char* x) {
        return assign(x);
    }

    /// Utility functions
    any& swap(any& x) {
        std::swap(policy, x.policy);
        std::swap(object, x.object);
        return *this;
    }

    /// Cast operator. You can only cast to the original type.
    template<typename T>
    T& cast() {
        if (policy != detail::get_policy<T>())
            throw detail::bad_any_cast();
        T* r = reinterpret_cast<T*>(policy->get_value(&object));
        return *r;
    }

    /// Cast operator. You can only cast to the original type.
    template<typename T>
    const T& cast() const {
        if (policy != detail::get_policy<T>())
            throw detail::bad_any_cast();
        const T* r = reinterpret_cast<const T*>(policy->get_value(&object));
        return *r;
    }

    /// Returns true if the any contains no value.
    bool empty() const {
        return policy == detail::get_policy<detail::empty_any>();
    }

    /// Frees any allocated memory, and sets the value to NULL.
    void reset() {
        policy->static_delete(&object);
        policy = detail::get_policy<detail::empty_any>();
    }

    /// Returns true if the two types are the same.
    bool compatible(const any& x) const {
        return policy == x.policy;
    }
};

} // namespace three

#endif // THREE_ANY_HPP