#pragma once
#include "core/Object.hpp"


#include <EASTL/string.h>

namespace hc
{
	class ICore;
	class HADES_CORE_API IApplication : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IApplication);
	private:
		ICore*		m_pICore;
	public:
		IApplication(void);
		virtual ~IApplication(void);
	private:
		virtual eastl::string _Configurepath() = 0;
		virtual void _OnInitialize(const eastl::string& args) = 0;
		virtual bool _OnDowork() = 0;
		virtual void _OnExit() = 0;
	public:
		void Initialize(const eastl::string& root, const eastl::string& args);
		void Dowork();
		void Exit();
	};
}