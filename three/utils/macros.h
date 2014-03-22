#ifndef THREE_MACROS_UTILS_HPP
#define THREE_MACROS_UTILS_HPP

/*

MS only

#define PROPERTY(type,name)  __declspec( property \
	( put = property__set_##name, get = property__get_##name ) ) type name;\
	typedef type property__tmp_type_##name

#define READONLY_PROPERTY(type,name) __declspec( property (get = property__get_##name) ) type name;\
	typedef type property__tmp_type_##name

#define WRITEONLY_PROPERTY(type,name) __declspec( property (put = property__set_##name) ) type name;\
	typedef type property__tmp_type_##name

#define GET(name) property__tmp_type_##name property__get_##name() 

#define SET(name) property__set_##name(const property__tmp_type_##name& value)

*/

#endif //THREE_MACROS_UTILS_HPP