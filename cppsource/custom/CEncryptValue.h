#ifndef _H_DYNAMICVALUE_H_
#define _H_DYNAMICVALUE_H_

template<class T>
class CEncryptValue
{
public:
    CEncryptValue()
    : m_Value(0)
    {
    }
    ~CEncryptValue()
    {
    }
    
    T getValue()
    {
//        LOGE("%d", m_EncryptKey);
        return m_Value ^ m_EncryptKey;
    }
    
    void setValue(T value)
    {
        m_Value = value;
        m_EncryptKey = rand();
        m_Value ^= m_EncryptKey;
    }
    
    void offset(T value)
    {
        setValue(getValue() + value);
    }
    
public:
    T m_Value;
    int m_EncryptKey;
};

typedef CEncryptValue<int> CDynamicValueInt;
typedef CEncryptValue<float> CDynamicValueFloat;
typedef CEncryptValue<bool> CDynamicValueBool;

#endif