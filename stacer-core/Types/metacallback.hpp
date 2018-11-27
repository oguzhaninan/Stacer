#ifndef _METACALLBACK_HPP_
#define _METACALLBACK_HPP_

#include <type_traits>
#include <QObject>

namespace Types
{
    template<class T, class = typename std::enable_if<std::is_pointer<T>::value, T>::type>
    struct _MetaCallback
    {
        typedef T type;
    };
    template<class T>
    struct _MetaCallback<T>
    {
        typedef typename std::add_pointer<T>::type type;
    };
    template<class T>
    struct MetaCallback : public Types::_MetaCallback<T>
    {
        typedef typename MetaCallback<T>::type                                pointer_type;
        pointer_type                                                          pointer;
        T                                                                     callback;

        constexpr bool is_callback(void) { return std::is_same<T*,pointer_type>::value; }
    };

}

#endif
