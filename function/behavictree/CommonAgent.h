﻿// ---------------------------------------------------------------------
// This agent file is auto-generated by behaviac designer, but you should
// implement the methods of the agent class if necessary!
// ---------------------------------------------------------------------

#ifndef BEHAVIAC_COMMONAGENT_H
#define BEHAVIAC_COMMONAGENT_H
#include "core/Predefine.h"
#include "script/IScriptInstance.h"
#include "behaviac/behaviac.h"
#include "TreeTemplate.h"
using namespace behaviac;
namespace hc
{
	class IScriptInstance;
}
namespace hbtf
{
	class TreeTemplate;
}
class CommonAgent : public behaviac::Agent
{
public:
    CommonAgent();
    virtual ~CommonAgent();

    DECLARE_BEHAVIAC_AGENT(CommonAgent, behaviac::Agent)

public:
    void Method0();
    void Method1();
    void Method2();
    void Method3();
    void Method4();
    void Method5();
    void Method6();
    void Method7();
    void Method8();
    void Method9();
    void Method10();
    void Method11();
    void Method12();
    void Method13();
    void Method14();
    void Method15();
    void Method16();
    void Method17();
    void Method18();
    void Method19();
    void Method20();
    void Method21();
    void Method22();
    void Method23();
    void Method24();
    void Method25();
    void Method26();
    void Method27();
    void Method28();
    void Method29();
    void Method30();

    bool Condition0();
    bool Condition1();
    bool Condition2();
    bool Condition3();
    bool Condition4();
    bool Condition5();
    bool Condition6();
    bool Condition7();
    bool Condition8();
    bool Condition9();
    bool Condition10();
    bool Condition11();
    bool Condition12();
    bool Condition13();
    bool Condition14();
    bool Condition15();
    bool Condition16();
    bool Condition17();
    bool Condition18();
    bool Condition19();
    bool Condition20();
    bool Condition21();
    bool Condition22();
    bool Condition23();
    bool Condition24();
    bool Condition25();
    bool Condition26();
    bool Condition27();
    bool Condition28();
    bool Condition29();
    bool Condition30();

    EBTStatus Action0();
    EBTStatus Action1();
    EBTStatus Action2();
    EBTStatus Action3();
    EBTStatus Action4();
    EBTStatus Action5();
    EBTStatus Action6();
    EBTStatus Action7();
    EBTStatus Action8();
    EBTStatus Action9();
    EBTStatus Action10();
    EBTStatus Action11();
    EBTStatus Action12();
    EBTStatus Action13();
    EBTStatus Action14();
    EBTStatus Action15();
    EBTStatus Action16();
    EBTStatus Action17();
    EBTStatus Action18();
    EBTStatus Action19();
    EBTStatus Action20();
    EBTStatus Action21();
    EBTStatus Action22();
    EBTStatus Action23();
    EBTStatus Action24();
    EBTStatus Action25();
    EBTStatus Action26();
    EBTStatus Action27();
    EBTStatus Action28();
    EBTStatus Action29();
    EBTStatus Action30();

private:
	hc::IScriptInstance*	m_rpResponse;
	hbtf::TreeTemplate*		m_rpTreeTemplate;
private:
	HADES_FORCEINLINE hc::IFunctionInvoker* _GetFunction(uint index, hc::IScriptInstance* instance, const char* str);
public:
	HADES_FORCEINLINE void Setup(hc::IScriptInstance* instance, hbtf::TreeTemplate* tree);
};

//---------------------------------------------------------------------------------------------------------
HADES_FORCEINLINE hc::IFunctionInvoker* CommonAgent::_GetFunction(uint index, hc::IScriptInstance* instance, const char* str)
{
	hc::IFunctionInvoker* func = m_rpTreeTemplate->GetFunction(index);
	if ( NULL == func)
	{
		instance->GotError(str);
	}
	return func;
}
//---------------------------------------------------------------------------------------------------------
HADES_FORCEINLINE void CommonAgent::Setup(hc::IScriptInstance* instance, hbtf::TreeTemplate* tree)
{
	m_rpResponse = instance;
	m_rpTreeTemplate = tree;
}


BEHAVIAC_DECLARE_TYPE_VECTOR_HANDLER(CommonAgent*);

#endif
