#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/warp/script/IFunctionInvokerWarp.hpp"
#include "script/warp/script/IScriptInstanceWarp.hpp"
#include "script/warp/script/IVirtualMachineWarp.hpp"
#include "script/warp/script/IScriptSystemWarp.hpp"

#include "script/warp/server/MessageWarp.hpp"
#include "script/warp/server/ServerProxyWarp.hpp"
#include "script/warp/server/IServerSystemWarp.hpp"

#include "script/warp/socket/ISocketSystemWarp.hpp"
#include "script/warp/socket/SocketProxyWarp.hpp"

#include "script/warp/timer/ITimersystemWarp.hpp"

#include "script/warp/utility/UtilityWarp.hpp"

#include "script/warp/file/IFileSystemWarp.hpp"

#include "script/warp/log/ILogSystemWarp.hpp"

#include "script/warp/services/IServicesSystemWarp.hpp"
#include "script/warp/services/IResumerWarp.hpp"
#include "script/warp/services/ContextWarp.hpp"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_System(IScriptSystem& sys)
	{
		Register_IScriptInstance(sys);
		Register_IFunctionInvoker(sys);
		Register_IScriptSystem(sys);
		Register_IMessage(sys);
		Register_ServerProxy(sys);
		Register_IServerSystem(sys);
		Register_ITimerSystem(sys);
		Register_Utility(sys);
		Register_IFileSystem(sys);
		Register_SocketProxy(sys);
		Register_ISocketSystem(sys);
		Register_ILogSystem(sys);
		Register_IServicesSystem(sys);
		Register_IResumer(sys);
		Register_Context(sys);
	}
}