#pragma once
#include <memory>

template <typename T>
class Singleton
{
public:
	virtual ~Singleton() = default;
	static std::shared_ptr<T> getOrCreateInstance()
	{
		if (!m_pInstance)
			m_pInstance.reset(new T());    
		return m_pInstance;
	}

protected:								  
	Singleton() = default;

private:
	static std::shared_ptr<T> m_pInstance;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::m_pInstance = nullptr;