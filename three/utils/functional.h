#ifndef THREE_FUNCTIONAL_H
#define THREE_FUNCTIONAL_H

#include <iostream>
#include <type_traits>

// TODO: EA: Make more generic for use with Euler order/enums/etc.

template<typename T, class C, class Enable = void>
class NumericalHook;

template<typename T, class C>
class NumericalHook<T, C, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
public:
    
    NumericalHook(const T other, C *objIn, void (C::*hookIn)() )
      : value( other ), obj( objIn), hook( hookIn ) {}
    
    T value;

    C *obj;
    
    void (C::*hook)();

    operator const T() const { return value; }
        
    inline NumericalHook& operator=(const T other){
        
        value = other;
        ((obj)->*(hook))(); 
        
        return *this;
    }
    inline NumericalHook operator=(const NumericalHook& other){
        
        // EA: Ugly..
        value = other;
        ((obj)->*(hook))();
        
        auto result = NumericalHook(other.value, other.obj, other.hook);
        return result;
    }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    inline NumericalHook& operator*=(const N other){ value *= other; ((obj)->*(hook))(); return *this;}
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    inline NumericalHook& operator/=(const N other){ value /= other; ((obj)->*(hook))(); return *this; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    inline NumericalHook& operator+=(const N other){ value += other; ((obj)->*(hook))(); return *this; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    inline NumericalHook& operator-=(const N other){ value -= other; ((obj)->*(hook))(); return *this; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    inline NumericalHook& operator%=(const N other){ value %= other; ((obj)->*(hook))(); return *this; }
    
    inline NumericalHook& operator*=(const NumericalHook& other){ value *= other.value; ((obj)->*(hook))(); return *this;}

    inline NumericalHook& operator/=(const NumericalHook& other){ value /= other.value; ((obj)->*(hook))(); return *this; }

    inline NumericalHook& operator+=(const NumericalHook& other){ value += other.value; ((obj)->*(hook))(); return *this; }

    inline NumericalHook& operator-=(const NumericalHook& other){ value -= other.value; ((obj)->*(hook))(); return *this; }

    inline NumericalHook& operator%=(const NumericalHook& other){ value %= other.value; ((obj)->*(hook))(); return *this; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator==(const N& val, const NumericalHook& fph) { return val == fph.value; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator!=(const N& val, const NumericalHook& fph) { return val != fph.value; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator< (const N& val, const NumericalHook& fph) { return val <  fph.value; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator> (const N& val, const NumericalHook& fph) { return val >  fph.value; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator<=(const N& val, const NumericalHook& fph) { return val <= fph.value; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator>=(const N& val, const NumericalHook& fph) { return val >= fph.value; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator==(const NumericalHook& fph, const N& val) { return fph.value == val; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator!=(const NumericalHook& fph, const N& val) { return fph.value != val; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator< (const NumericalHook& fph, const N& val) { return fph.value <  val; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator> (const NumericalHook& fph, const N& val) { return fph.value >  val; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator<=(const NumericalHook& fph, const N& val) { return fph.value <= val; }
    
    template<typename N, typename std::enable_if<std::is_arithmetic<N>::value>::type>
    friend bool operator>=(const NumericalHook& fph, const N& val) { return fph.value >= val; }
    
    inline friend std::ostream& operator<<(std::ostream &out, const NumericalHook<T, C>& other) { out << other.value; return out; }
    
};

#define FLOAT_HOOK(c, h, v) NumericalHook<float, c>(v, this, &c::h)
#define DOUBLE_HOOK(c, h, v) NumericalHook<double, c>(v, this, &c::h)

#endif