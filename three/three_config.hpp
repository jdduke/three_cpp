#ifndef THREE_CONFIG_HPP
#define THREE_CONFIG_HPP

#if defined(_MSC_VER) || defined(__MINGW32__)
#define THREE_HAS_DECLSPEC
#endif

// Default to a header-only implementation. The user must specifically request
// separate compilation by defining either THREE_SEPARATE_COMPILATION or
// THREE_DYN_LINK (as a DLL/shared library implies separate compilation).
#if !defined(THREE_HEADER_ONLY)
# if !defined(THREE_SEPARATE_COMPILATION)
#  if !defined(THREE_DYN_LINK)
#   define THREE_HEADER_ONLY
#  endif // !defined(THREE_DYN_LINK)
# endif // !defined(THREE_SEPARATE_COMPILATION)
#endif // !defined(THREE_HEADER_ONLY)

#if defined(THREE_HEADER_ONLY)
# define THREE_DECL inline
#else // defined(THREE_HEADER_ONLY)
# if defined(THREE_HAS_DECLSPEC)
// We need to import/export our code only if the user has specifically asked
// for it by defining THREE_DYN_LINK.
#  if defined(THREE_DYN_LINK)
// Export if this is our own source, otherwise import.
#   if defined(THREE_SOURCE)
#    define THREE_DECL __declspec(dllexport)
#   else // defined(THREE_SOURCE)
#    define THREE_DECL __declspec(dllimport)
#   endif // defined(THREE_SOURCE)
#  endif // defined(THREE_DYN_LINK)
# endif // defined(THREE_HAS_DECLSPEC)
#endif // defined(THREE_HEADER_ONLY)

// If THREE_DECL isn't defined yet define it now.
#if !defined(THREE_DECL)
# define THREE_DECL
#endif // !defined(THREE_DECL)

#endif // THREE_CONFIG_HPP