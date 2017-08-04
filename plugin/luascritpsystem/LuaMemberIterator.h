#pragma once
#include "script/IMemberIterator.h"


#include <EASTL/vector.h>

struct lua_State;
namespace hlss
{
	class LuaMemberIterator : public hc::IMemberIterator
	{
	private:
		typedef eastl::vector< hc::IMemberIterator* > ChiledArray;
	protected:
		mutable int			m_ChiledIndex;
		int					m_InstanceHandle;
		lua_State**			m_rppLuaState;
		mutable ChiledArray	m_ChiledArray;
	public:
		LuaMemberIterator(int index, lua_State** l);
		virtual ~LuaMemberIterator();
	public:
		virtual void Being() const;
		virtual bool Next() const;
		virtual const char* GetStringField(uint* len) const;
		virtual int GetIntField() const;
		virtual uint64 GetUInt64Field() const;
		virtual hc::DataType GetKeyType() const;
		virtual hc::DataType GetValueType() const;
		virtual bool AsBool() const;
		virtual short AsShort() const;
		virtual ushort AsUShort() const;
		virtual int AsInt() const;
		virtual uint AsUInt() const;
		virtual float AsFloat() const;
		virtual double AsDouble() const;
		virtual int64 AsInt64() const;
		virtual uint64 AsUInt64() const;
		virtual const char* AsString(uint* len) const;
		virtual const Object* AsNative() const;
		virtual const hc::IMemberIterator& AsClass() const;
		virtual void End() const;
	};
}
