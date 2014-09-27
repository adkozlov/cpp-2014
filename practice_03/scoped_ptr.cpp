#include "scoped_ptr.h"

template<class T> scoped_ptr<T>::scoped_ptr(T* ptr) {
    t = ptr;
}

template<class T> scoped_ptr<T>::~scoped_ptr() {
    if (t != 0) {
        delete t;
    }
}

template<class T> T& scoped_ptr<T>::operator *() const {
    return *t;
}

template<class T> T* scoped_ptr<T>::operator ->() const {
    return t;
}

template<class T> bool scoped_ptr<T>::is_null() const {
    return t == 0;
}
