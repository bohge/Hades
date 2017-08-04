#include "memory/IMemorySystem.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "LuaVirtualMachine.h"
#include "file/IFileSystem.h"
#include "ScriptCallBridge.h"
#include "LuaRootInstance.h"
#include "Scriptfilecache.h"
#include "core/Configure.h"
#include "log/ILogSystem.h"



extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
	int luaopen_socket_core(lua_State *L);
	int luaopen_mime_core(lua_State *L);
}
#include "lua_tinker.hpp"


#define DEFAULT_SELF_PTR_NAME "LuaVirtualMachineSelf"
using namespace hc;
using namespace eastl;
namespace hlss
{
	static void *my_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
	{
		if (nsize == 0)
		{
			IMemorySystem::ReleaseMemory(ptr);
			return NULL;
		}
		else
			return IMemorySystem::ReallocMemory(ptr, nsize);
	}
	int cLoadNativeLibrary(lua_State* l)
	{
		size_t len = 0;
		const char* pname = lua_tolstring((lua_State*)l, -1, &len);
		eastl::string sname(pname, len);
		lua_getglobal(l, DEFAULT_SELF_PTR_NAME);
		LuaVirtualMachine* self = (LuaVirtualMachine*)lua_touserdata(l, -1);
		lua_pop(l, 2);
		self->LoadNativeLibrary(sname);
		ASSERT(0 == lua_gettop(l));
		return 0;
	}


	//---------------------------------------------------------------------------------------------------------
	LuaVirtualMachine::LuaVirtualMachine(uint index)
		:IVirtualMachine(index)
		, m_pLua_State(NULL)
		, m_ValueHolderIndex(0)
		, m_CurrentIndex(0)
		, m_LastSize(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LuaVirtualMachine::~LuaVirtualMachine()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_DoUpdate()
	{
		//uint size = lua_gc(m_pLua_State, LUA_GCCOUNT, 0);
		//lua_gc(m_pLua_State, LUA_GCSTEP, size - m_LastSize);
		//m_LastSize = lua_gc(m_pLua_State, LUA_GCCOUNT, 0);
		//lua_gc(m_pLua_State, LUA_GCCOLLECT, 0);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::Initialize()
	{
		IVirtualMachine::Initialize();
		m_pLua_State = luaL_newstate();

		//lua_setallocf(m_pLua_State, &my_alloc, NULL);

		luaL_openlibs(m_pLua_State);

		//设置加载native库函数
		lua_pushlightuserdata(m_pLua_State, this);
		lua_setglobal(m_pLua_State, DEFAULT_SELF_PTR_NAME);
		lua_pushcfunction(m_pLua_State, cLoadNativeLibrary);
		lua_setglobal(m_pLua_State, "nativelibrary");

		//删除旧的路径
		_EraseOldPaths();

		//创建holder
		lua_newtable(m_pLua_State);
		m_ValueHolderIndex = luaL_ref(m_pLua_State, LUA_REGISTRYINDEX);
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::Exit()
	{
#ifdef _PRINT_LUA_CLASS_INFO
		PrintScriptClassInfo(m_ScriptClassMap);
		m_ScriptClassMap.clear();
#endif
		FOR_EACH(ScriptClassList, it, m_ScriptClassList)
		{
			SAFE_DELETE(*it);
		}
		m_ScriptClassList.clear();
		lua_close(m_pLua_State);
		m_pLua_State = NULL;
		IVirtualMachine::Exit();
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::LoadNativeLibrary(const eastl::string& name)
	{
		uint hash = Utility::HashCode(name);
		LoadedLibrarySet::iterator it = m_LoadedLibrarySet.find(hash);
		if (m_LoadedLibrarySet.end() == it)
		{
			m_LoadedLibrarySet.insert(hash);
			const IScriptSystem::ClassRegisterGlobal* crg = IScriptSystem::Instance()->GetNativeLibrary(name);
			if (NULL == crg)
			{
				HADESERROR("nativelibrary(\"%s\") load fail!\r\n", name.c_str());
			}
			else
			{
				FOR_EACH_CONST(IScriptSystem::ClassRegisterList, clr, crg->ClassList)
				{
					ReferenceClass(*clr);
				}
				FOR_EACH_CONST(IScriptSystem::GlobalPairList, glo, crg->GlobalList)
				{
					SetGlobal(glo->first.c_str(), glo->second, (void*)NULL, &ScriptValue::NullCleaner);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_EraseOldPaths()
	{
		string cur_path;
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_getglobal(m_pLua_State, "package");
		lua_pushlstring(m_pLua_State, cur_path.c_str(), cur_path.size()); // push the new one
		lua_setfield(m_pLua_State, -2, "path"); // set the field "path" in table at -2 with value at top of stack
		lua_pushlstring(m_pLua_State, cur_path.c_str(), cur_path.size()); // push the new one
		lua_setfield(m_pLua_State, -2, "cpath"); // set the field "path" in table at -2 with value at top of stack
		lua_pop(m_pLua_State, 1); // get rid of package table from top of stack
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::AddScriptPath(const eastl::string& path)
	{
		string scriptpath = IFileSystem::Instance()->PathAssembly(path);
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_getglobal(m_pLua_State, "package");
		lua_getfield(m_pLua_State, -1, "path"); // get field "path" from table at top of stack (-1)
		size_t len;
		const char* pstr = lua_tolstring(m_pLua_State, -1, &len); // grab path string from top of stack
		string cur_path(pstr, len);
		lua_pop(m_pLua_State, 1);
		cur_path.append(scriptpath);
		cur_path.append("?.lua;");
		cur_path.append(scriptpath);
		cur_path.append("?/init.lua;");
		lua_pushlstring(m_pLua_State, cur_path.c_str(), cur_path.size()); // push the new one
		lua_setfield(m_pLua_State, -2, "path"); // set the field "path" in table at -2 with value at top of stack
		lua_pop(m_pLua_State, 1); // get rid of package table from top of stack
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::AddNativePath(const eastl::string& path)
	{
		string funcpath = IFileSystem::Instance()->PathAssembly(path);
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_getglobal(m_pLua_State, "package");
		lua_getfield(m_pLua_State, -1, "cpath"); // get field "path" from table at top of stack (-1)
		size_t len;
		const char* pstr = lua_tolstring(m_pLua_State, -1, &len);
		string cur_path(pstr, len); // grab path string from top of stack
		lua_pop(m_pLua_State, 1);
		cur_path.append(funcpath);
#if defined(_WIN32)
		cur_path.append("?.dll;");
#else
		cur_path.append("?.so;");
#endif
		lua_pushlstring(m_pLua_State, cur_path.c_str(), cur_path.size()); // push the new one
		lua_setfield(m_pLua_State, -2, "cpath"); // set the field "path" in table at -2 with value at top of stack
		lua_pop(m_pLua_State, 1); // get rid of package table from top of stack
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaVirtualMachine::isRegisteredClass(const eastl::string& name)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_getglobal(m_pLua_State, name.c_str());//得到类的类型
		bool res = lua_istable(m_pLua_State, -1);
		lua_pop(m_pLua_State, 1);
		ASSERT(0 == lua_gettop(m_pLua_State));
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_DoReferenceClass(ClassRegister* c)
	{
		_DoRegisterClass(c->GetClassTypeName());
		_DoRegisterInheritance(c);
		_DoRegisterFunction(c);
		_DoRegisterConstant(c);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::SetGlobal(const char* name, Object* obj, void* rec, hc::ScriptValue::ClearFunction func)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		if (NULL == obj)
		{
			lua_pushnil(m_pLua_State);
			lua_setglobal(m_pLua_State, name);
		}
		else
		{
			lua_getglobal(m_pLua_State, name);
			if (LUA_TUSERDATA == lua_type(m_pLua_State, -1))
			{
				ScriptValue* userdata = (ScriptValue*)lua_touserdata(m_pLua_State, -1);
				userdata->Clear();
				userdata->SetValue(obj, rec, func);
				lua_pop(m_pLua_State, 1);
			}
			else
			{
				lua_pop(m_pLua_State, 1);
				ScriptValue* userdata = (ScriptValue*)lua_newuserdata(m_pLua_State, sizeof(ScriptValue));
				userdata->SetValue(obj, rec, func);
				lua_getglobal(m_pLua_State, obj->GetTypeName());
				if (lua_istable(m_pLua_State, -1))
				{
					lua_setmetatable(m_pLua_State, -2);
				}
				else
				{
					lua_pop(m_pLua_State, 1);
				}
				lua_setglobal(m_pLua_State, name);
			}
		}
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::SetGlobal(const char* name)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_pushnil(m_pLua_State);
		lua_setglobal(m_pLua_State, name);
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::SetGlobal(const char* name, float obj)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_pushnumber(m_pLua_State, obj);
		lua_setglobal(m_pLua_State, name);
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::SetGlobal(const char* name, int obj)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_pushinteger(m_pLua_State, obj);
		lua_setglobal(m_pLua_State, name);
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::SetGlobal(const char* name, uint64 obj)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		if (hc::nullid == obj)
		{
			lua_pushnil(m_pLua_State);
		}
		else
		{
			uint64* ud = (uint64*)luaL_pushcdata(m_pLua_State, MY_CTID_UINT64, sizeof(uint64));
			*ud = obj;
		}
		lua_setglobal(m_pLua_State, name);
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::SetGlobal(const char* name, bool obj)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_pushboolean(m_pLua_State, obj);
		lua_setglobal(m_pLua_State, name);
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::SetGlobal(const char* name, const eastl::string& obj)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_pushlstring(m_pLua_State, obj.c_str(), obj.size());
		lua_setglobal(m_pLua_State, name);
		ASSERT(0 == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	Object* LuaVirtualMachine::GetGlobal(const char* name)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_getglobal(m_pLua_State, name);
		Object* r = ((ScriptValue*)lua_touserdata(m_pLua_State, -1))->GetObject();
		lua_pop(m_pLua_State, 1);
		ASSERT(0 == lua_gettop(m_pLua_State));
		return r;
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaVirtualMachine::SetGlobalReference(uint64 id)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		uint64* ud = (uint64*)luaL_pushcdata(m_pLua_State, MY_CTID_UINT64, sizeof(uint64));
		*ud = id;
		int index = luaL_ref(m_pLua_State, LUA_REGISTRYINDEX);//将该函数绑定到索引列表
		ASSERT(0 == lua_gettop(m_pLua_State));
		return index;
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LuaVirtualMachine::GetGlobalReference(int index)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_rawgeti(m_pLua_State, LUA_REGISTRYINDEX, index);
		ASSERT(LUA_TCDATA == lua_type(m_pLua_State, -1));
		ASSERT(MY_CTID_UINT64 == luaL_cdatatype(m_pLua_State, -1));
		uint64 res = *(uint64*)luaL_tocdata(m_pLua_State, -1);
		lua_pop(m_pLua_State, 1);
		ASSERT(0 == lua_gettop(m_pLua_State));
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaVirtualMachine::_DoRegisteInstance(uint hash, int scriptdata)
	{
		LuaRootInstance* instance = NEW LuaRootInstance(GetIndex(), hash, &m_pLua_State, this);
		instance->Initialize(scriptdata);
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaVirtualMachine::_DoCreateInstance()
	{
		DEBUGCODE(int t = lua_gettop(m_pLua_State));
		lua_newtable(m_pLua_State);
		int ref = luaL_ref(m_pLua_State, LUA_REGISTRYINDEX);
		LuaRootInstance* instance = NEW LuaRootInstance(GetIndex(), nullhandle, &m_pLua_State, this);
		instance->Initialize(ref);
		ASSERT(t == lua_gettop(m_pLua_State));
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaVirtualMachine::_DoCreateInstance(uint hash, const eastl::string& name)
	{
		ASSERT(0 == lua_gettop(m_pLua_State));
		lua_getglobal(m_pLua_State, name.c_str());
		if (!lua_istable(m_pLua_State, -1))
		{
			lua_pop(m_pLua_State, 1);
			lua_newtable(m_pLua_State);
			lua_setglobal(m_pLua_State, name.c_str());
			lua_getglobal(m_pLua_State, name.c_str());
		}
		int ref = luaL_ref(m_pLua_State, LUA_REGISTRYINDEX);
		LuaRootInstance* instance = NEW LuaRootInstance(GetIndex(), hash, &m_pLua_State, this);
		instance->Initialize(ref);
		ASSERT(0 == lua_gettop(m_pLua_State));
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string LuaVirtualMachine::_GetRealPath(const eastl::string& path)
	{
		bool iscon;
		string realpath = path;
		if (!IFileSystem::Instance()->isFileExist(realpath))
		{
			eastl::string replacestring = path;
			if (string::npos != path.find(".lua"))
			{
				Utility::RemoveTargetString(replacestring, ".lua");
			}
			Utility::ReplaceAllString(replacestring, ".", "/");
			realpath = replacestring + ".lua";
			if (!IFileSystem::Instance()->isFileExist(realpath))
			{
				realpath = Configure::FILE_PATH_SCRIPT;
				realpath += replacestring + ".lua";
				if (!IFileSystem::Instance()->isFileExist(realpath))
				{
					realpath = Configure::FILE_PATH_COMMON_SCRIPT;
					realpath += replacestring + ".lua";
					//这个没有就真没有了
				}
			}

		}
		return realpath;
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptInstance* LuaVirtualMachine::_DoLoadScript(uint hash, const eastl::string& path)
	{
		string realpath = _GetRealPath(path);
		const string& script = Scriptfilecache::Instance()->GetScriptFile(hash, realpath);
		LuaRootInstance* instance = NEW LuaRootInstance(GetIndex(), hash, &m_pLua_State, this);
		if (!instance->Initialize(realpath, script))
		{
			_DoReleaseScript(instance);
			instance = NULL;
		}
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_DoReleaseScript(hc::IScriptInstance* instance)
	{
		LuaInstance* li = static_cast<LuaInstance*>(instance);
		li->Exit();
		SAFE_DELETE(li);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_DoRegisterClass(const char* cn)
	{
		lua_newtable(m_pLua_State);

		//注册类名称
		lua_pushstring(m_pLua_State, "__name");
		lua_pushstring(m_pLua_State, cn);
		lua_rawset(m_pLua_State, -3);

		//元表
		lua_pushstring(m_pLua_State, "__index");
		lua_pushcclosure(m_pLua_State, lua_tinker::meta_get, 0);
		lua_rawset(m_pLua_State, -3);

		lua_pushstring(m_pLua_State, "__newindex");
		lua_pushcclosure(m_pLua_State, lua_tinker::meta_set, 0);
		lua_rawset(m_pLua_State, -3);

		lua_pushstring(m_pLua_State, "__gc");
		lua_pushcclosure(m_pLua_State, &ClassDestruct, 0);
		lua_rawset(m_pLua_State, -3);

		//设置全局名称
		lua_setglobal(m_pLua_State, cn);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_DoRegisterInheritance(hc::ClassRegister* sc)
	{
		if (sc->isDriverType())//注册继承类
		{
			ASSERT(0 == lua_gettop(m_pLua_State));
			lua_getglobal(m_pLua_State, sc->GetClassTypeName());
			lua_pushstring(m_pLua_State, "__parent");
			lua_getglobal(m_pLua_State, sc->GetBaseClassTypeName());
			lua_rawset(m_pLua_State, -3);
			lua_pop(m_pLua_State, 1);
			ASSERT(0 == lua_gettop(m_pLua_State));
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_RegisterClassFunction(hc::ClassRegister* c, int index, hc::ScriptProperty::FunctionType ft, const eastl::string& name)
	{
		DEBUGCODE(int t = lua_gettop(m_pLua_State));
		lua_CFunction func = NULL;
		bool isCon = string("__call") == name ? true : false;
		lua_getglobal(m_pLua_State, c->GetClassTypeName());//得到类的表名
		ASSERT(lua_istable(m_pLua_State, -1));

		if (isCon)lua_newtable(m_pLua_State);//如果是构造需要新加元表

		lua_pushlstring(m_pLua_State, name.c_str(), name.size());//将方法名称推入堆栈
		lua_pushlightuserdata(m_pLua_State, c);//将ScriptClass信息推入堆栈
		lua_pushinteger(m_pLua_State, index);//将函数在数组中的位置推入堆栈
		lua_pushlightuserdata(m_pLua_State, this);//将虚拟机放入堆栈
		if (c->isOverrideFunction(index))
		{
			if (isCon || ScriptProperty::FT_STATIC == ft)
				func = &OverrideClassConstructStatic;
			else
				func = &OverrideClassFunction;
		}
		else
		{
			if (isCon || ScriptProperty::FT_STATIC == ft)
				func = &SingleClassConstructStatic;
			else
				func = &SingleClassFunction;
		}
		lua_pushcclosure(m_pLua_State, func, 3);//注册函数，通知lua函数拥有两个upvalue
		lua_rawset(m_pLua_State, -3);

		if (isCon)lua_setmetatable(m_pLua_State, -2);

		lua_pop(m_pLua_State, 1);
		ASSERT(t == lua_gettop(m_pLua_State));
	}
	//---------------------------------------------------------------------------------------------------------
	const char* LuaVirtualMachine::_FunctionNameMapping(const eastl::string& name)
	{
		if (name == ScriptProperty::__ADD)
		{
			return "__add";
		}
		else if (name == ScriptProperty::__SUB)
		{
			return "__sub";
		}
		else if (name == ScriptProperty::__MUL)
		{
			return "__mul";
		}
		else if (name == ScriptProperty::__DIV)
		{
			return "__div";
		}
		else if (name == ScriptProperty::__CON)
		{
			return "__call";
		}
		else if (name == ScriptProperty::__EQU)
		{
			return "__eq";
		}
		else if (name == ScriptProperty::__LES)
		{
			return "__lt";
		}
		else if (name == ScriptProperty::__LEQ)
		{
			return "__le";
		}
		else if (name == ScriptProperty::__STR)
		{
			return "__tostring";
		}
		else if (name == ScriptProperty::__CAT)
		{
			return "__concat";
		}
		//else if (name == ScriptProperty::__GET)
		//{
		//	return "__index";
		//}
		//else if (name == ScriptProperty::__SET)
		//{
		//	return "__newindex";
		//}
		return name.c_str();
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_DoRegisterFunction(hc::ClassRegister* sc)
	{
		uint funcCount = sc->GetFunctionListCount();
		for (int i = 0; i < funcCount; ++i)
		{
			ASSERT(i == sc->GetFunction(i)->GetIndex());
			_RegisterClassFunction(sc, i, sc->GetFunctionListType(i), _FunctionNameMapping(sc->GetFunctionListName(i)));
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaVirtualMachine::_DoRegisterConstant(hc::ClassRegister* sc)
	{
		DEBUGCODE(int t = lua_gettop(m_pLua_State));
		uint funcCount = sc->GetClassConstantCount();
		for (int i = 0; i < funcCount; ++i)
		{
			CLassConstant cc = sc->GetClassConstant(i);
			lua_getglobal(m_pLua_State, sc->GetClassTypeName());//得到类的表名
			ASSERT(lua_istable(m_pLua_State, -1));
			lua_pushlstring(m_pLua_State, cc.first.c_str(), cc.first.size());//将名称推入堆栈

			ASSERT(BOOL_RTTI_ID == Bool::SID());
			ASSERT(STRING_RTTI_ID == String::SID());
			ASSERT(DOUBLE_RTTI_ID == Double::SID());

			switch (cc.second->GetTypeID())
			{
			case BOOL_RTTI_ID:
			{
								 lua_pushboolean(m_pLua_State, static_cast<Bool*>(cc.second)->GetData());
			}break;
			case STRING_RTTI_ID:
			{
								   lua_pushlstring(m_pLua_State, static_cast<String*>(cc.second)->GetData().c_str(), static_cast<String*>(cc.second)->GetData().size());
			}break;
			case INT_RTTI_ID:
			{
								lua_pushnumber(m_pLua_State, static_cast<Int*>(cc.second)->GetData());
			}break;
			case UINT_RTTI_ID:
			{
								 lua_pushnumber(m_pLua_State, static_cast<UInt*>(cc.second)->GetData());
			}break;
			case FLOAT_RTTI_ID:
			{
								  lua_pushnumber(m_pLua_State, static_cast<Float*>(cc.second)->GetData());
			}break;
			case DOUBLE_RTTI_ID:
			{
								   lua_pushnumber(m_pLua_State, static_cast<Double*>(cc.second)->GetData());
			}break;
			case INT64_RTTI_ID:
			case SHORT_RTTI_ID:
			case USHORT_RTTI_ID:
			case UINT64_RTTI_ID:
			case BUFFER_RTTI_ID:
			case FUNCTION_RTTI_ID:
			case INSTANCE_RTTI_ID: ASSERT(false); break;
			default:
			{
					   ScriptValue* userdata = (ScriptValue*)lua_newuserdata(m_pLua_State, sizeof(ScriptValue));
					   userdata->SetValue(cc.second, &ScriptValue::NullCleaner);
			}break;
			}
			lua_rawset(m_pLua_State, -3);
			lua_pop(m_pLua_State, 1);
		}
		ASSERT(t == lua_gettop(m_pLua_State));
	}
}