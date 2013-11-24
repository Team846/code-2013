#include "ValueChangeEvent.h"

template<typename T> ValueChangeEvent<T>::ValueChangeEvent(T* variable, T value)
{
	m_variable = variable;
	m_value = value;
}

template<typename T> ValueChangeEvent<T>::~ValueChangeEvent()
{
}

template<typename T> bool ValueChangeEvent<T>::Fired()
{
	return m_lastValue != m_value && *m_variable == m_value;
}

template<typename T> void ValueChangeEvent<T>::Update()
{
	m_lastValue = *m_variable;
}

