#pragma once
#include <mutex>

using namespace std;

template <typename T>
class Singleton
{
protected:
	explicit Singleton()
	{
		if (m_pInstance != nullptr)
		{
			MessageBoxA(nullptr, "Can't Create Singleton Instance any more.", 0, 0);
			delete m_pInstance.get();
		}
	}
private:
	Singleton(const Singleton<T>& _src) = delete;
	Singleton<T> operator= (const Singleton<T>& _rhs) = delete;
public:
	static T* GetInstance()
	{
		// Call Once flag
		call_once(m_onceFlag, []()
		{
			m_pInstance = make_unique<T>();
		});

		return m_pInstance.get();
	}


private:
	static unique_ptr<T> m_pInstance;
	static once_flag m_onceFlag;
};

template <typename T>
unique_ptr<T> Singleton<T>::m_pInstance;

template <typename T>
once_flag Singleton<T>::m_onceFlag;