#ifndef THREE_CONFIG_H
#define THREE_CONFIG_H

#if defined(_MSC_VER) || defined(__MINGW32__)
#define THREE_HAS_DECLSPEC
#endif

#if defined(THREE_HAS_DECLSPEC)
// We need to import/export our code only if the user has specifically asked
// for it by defining THREE_DYN_LINK.
# if defined(THREE_DYN_LINK)
// WARNING: Shared library support is very much experimental, and will likely
// break horribly in practice... You've been warned.
#  pragma warning(disable : 4251)
// Export if this is our own source, otherwise import.
#  if defined(THREE_SOURCE)
#   define THREE_DECL __declspec(dllexport)
#  else // defined(THREE_SOURCE)
#   define THREE_DECL __declspec(dllimport)
#  endif // defined(THREE_SOURCE)
# endif // defined(THREE_DYN_LINK)
#endif // defined(THREE_HAS_DECLSPEC)


// If THREE_DECL isn't defined yet define it now.
#if !defined(THREE_DECL)
# define THREE_DECL
#endif // !defined(THREE_DECL)

#if !defined(THREE_OBSOLETE)
#define THREE_OBSOLETE
#endif

#if !defined(THREE_REVIEW)
#define THREE_REVIEW(msg)
#endif


#ifndef THREE_DATA_DIR
#define THREE_DATA_DIR "."
#endif

#ifndef THREE_CLASS
#define THREE_CLASS
#endif

// C++11 support
#if defined(__GNUC__)
#  define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#  if GCC_VERSION >= 40300
#    define THREE_HAS_VARIADIC_TEMPLATES 1
#    define THREE_HAS_RVALUE_REFERENCES 1
#  endif
#  if GCC_VERSION >= 40400
#    define THREE_HAS_DELETED_FUNCTIONS 1
#  endif
#  if GCC_VERSION >= 40400
#    define THREE_HAS_EXPLICIT_CONVERSION_OPERATORS 1
#  endif
#  if GCC_VERSION >= 40600
#    define THREE_HAS_RANGE_FOR 1
#  endif
#  if GCC_VERSION >= 40700
#    define THREE_HAS_ALIAS_TEMPLATES 1
#  endif
#elif defined(__clang__)
#  define THREE_HAS_VARIADIC_TEMPLATES __has_feature(cxx_variadic_templates)
#  define THREE_HAS_DELETED_FUNCTIONS  __has_feature(cxx_deleted_functions)
#  define THREE_HAS_RVALUE_REFERENCES  __has_feature(cxx_rvalue_references)
#  define THREE_HAS_RANGE_FOR          __has_feature(cxx_range_for)
#  define THREE_HAS_ALIAS_TEMPLATES    __has_feature(cxx_alias_templates)
#  define THREE_HAS_EXPLICIT_CONVERSION_OPERATORS __has_feature(cxx_explicit_conversions)
#elif defined(_MSC_VER)
// No variadics, no deleted functions, no fun :(
#  if _MSC_VER > 1500
#    define THREE_HAS_RVALUE_REFERENCES 1
#  endif
#  if _MSC_VER > 1600
#    define THREE_HAS_RANGE_FOR 1
#    if MSC_NOV_2012_CTP || _MSC_VER > 1700
#      define THREE_HAS_VARIADIC_TEMPLATES 1
#      define THREE_HAS_DELEGATING_CONSTRUCTORS 1
#      define THREE_HAS_EXPLICIT_CONVERSION_OPERATORS 1
#    endif
#  endif
#endif

#if !defined(THREE_HAS_VARIADIC_TEMPLATES)
#  define THREE_HAS_VARIADIC_TEMPLATES 0
#endif
#if !defined(THREE_HAS_DELETED_FUNCTIONS)
#  define THREE_HAS_DELETED_FUNCTIONS 0
#endif
#if !defined(THREE_HAS_RVALUE_REFERENCES)
#define THREE_HAS_RVALUE_REFERENCES 0
#endif
#if !defined(THREE_HAS_RANGE_FOR)
# define THREE_HAS_RANGE_FOR 0
#endif
#if !defined(THREE_HAS_EXPLICIT_CONVERSION_OPERATORS)
# define THREE_HAS_EXPLICIT_CONVERSION_OPERATORS 0
#endif
#if !defined(THREE_HAS_ALIAS_TEMPLATES)
# define THREE_HAS_ALIAS_TEMPLATES 0
#endif

#if !THREE_HAS_RANGE_FOR
#  error Range-based for loop support required
#endif
#if !THREE_HAS_RVALUE_REFERENCES
#  error R-value reference support required
#endif

#if THREE_HAS_DELETED_FUNCTIONS
#  define THREE_DECL_DELETE = delete
#  define THREE_DECL_DEFAULT = default
#else
#  define THREE_DECL_DELETE
#  define THREE_DECL_DEFAULT
#endif

#if THREE_HAS_EXPLICIT_CONVERSION_OPERATORS
#  define THREE_EXPLICIT explicit
#else
#  define THREE_EXPLICIT
#endif

#endif // THREE_CONFIG_H