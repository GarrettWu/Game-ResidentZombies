#ifndef _GF_SINGLETON_H
#define _GF_SINGLETON_H

template <class T>
class Singleton
{
public:
    static inline T* instance();
    void release();
protected:
    Singleton(void){}
    ~Singleton(void){}
    Singleton(const Singleton&){}
    Singleton & operator= (const Singleton &){}
    static T* _instance;
};

template <class T>
inline T* Singleton<T>::instance()
{
    if(!_instance)
        _instance = new T;
    return _instance;
}

template <class T>
void Singleton<T>::release()
{
    if (!_instance)
        return;
    delete _instance;
    _instance = 0;
}
//Class that will implement the singleton mode, 
//must use the macro in it's declare file
#define DECLARE_SINGLETON_CLASS(_Ty)	\
friend class Singleton<_Ty>;

// declare static member
#define DECLARE_SINGLETON_MEMBER(_Ty)	\
template <> _Ty* Singleton<_Ty>::_instance = NULL;

#endif//_GF_SINGLETON_H
