
#ifndef TINYMATWRITER_EXPORT_H
#define TINYMATWRITER_EXPORT_H

#define TINYMATWRITER_STATIC_DEFINE

#ifdef TINYMATWRITER_STATIC_DEFINE
#  define TINYMATWRITER_EXPORT
#  define TINYMATWRITER_NO_EXPORT
#else
#  ifndef TINYMATWRITER_EXPORT
#    ifdef TinyMATShared_EXPORTS
        /* We are building this library */
#      define TINYMATWRITER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define TINYMATWRITER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef TINYMATWRITER_NO_EXPORT
#    define TINYMATWRITER_NO_EXPORT 
#  endif
#endif

#ifndef TINYMATWRITER_DEPRECATED
#  define TINYMATWRITER_DEPRECATED __declspec(deprecated)
#endif

#ifndef TINYMATWRITER_DEPRECATED_EXPORT
#  define TINYMATWRITER_DEPRECATED_EXPORT TINYMATWRITER_EXPORT TINYMATWRITER_DEPRECATED
#endif

#ifndef TINYMATWRITER_DEPRECATED_NO_EXPORT
#  define TINYMATWRITER_DEPRECATED_NO_EXPORT TINYMATWRITER_NO_EXPORT TINYMATWRITER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TINYMATWRITER_NO_DEPRECATED
#    define TINYMATWRITER_NO_DEPRECATED
#  endif
#endif

#endif /* TINYMATWRITER_EXPORT_H */
