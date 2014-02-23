#ifndef THREE_MACROS_UTILS_HPP
#define THREE_MACROS_UTILS_HPP

#define PROPERTY(t,n)  __declspec( property \
	( put = property__set_##n, get = property__get_##n ) ) t n;\
	typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n
#define GET(n) property__tmp_type_##n property__get_##n() 
#define SET(n) property__set_##n(const property__tmp_type_##n& value)

#endif //THREE_MACROS_UTILS_HPP