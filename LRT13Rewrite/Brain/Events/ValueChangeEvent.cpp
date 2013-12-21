#include "ValueChangeEvent.h"

template<typename T> ValueChangeEvent<T>::ValueChangeEvent(T* variable, T value)
{
	m_variable = variable;
	m_value = value;
}

template<typename T> ValueChangeEvent<T>::~ValueChangeEvent()
{
}

template<typename T> bool ValueChangeEvent<T>::CheckCondition()
{
	return *m_variable == m_value;
}

