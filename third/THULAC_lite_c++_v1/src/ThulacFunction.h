#pragma once
#include "core/Singleton.hpp"
#include "core/IFunction.h"
//#include "thulac.h"

#include "concurrentqueue.h"
#include <string>

class THULAC;

namespace htf
{
	class ThulacFunction : public hc::IFunction
	{
		SYSTEM_SINGLETON_DECLEAR(ThulacFunction);
		RTTI_DRIVER_TYPE_DECLEAR(ThulacFunction, hc::IFunction);
	private:
		typedef moodycamel::ConcurrentQueue< THULAC* > THULACQueue;
	private:
		std::string			m_ModelPath;
		std::string			m_UserPath;
		bool				m_isSegOnly;
		bool				m_isT2S;
		bool				m_isUfilter;
		THULACQueue*		m_pTHULACQueue;
		char				m_Separator;
	public:
		ThulacFunction();
		~ThulacFunction();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	private:
		THULAC* _GetObject();
		void _FreeObject(THULAC* obj);
	public:
		void std::string Split(const std::string& input);
	public:
		//user_path           设置用户词典，用户词典中的词会被打上uw标签。词典中每一个词一行，UTF8编码
		//t2s                 默认False, 是否将句子从繁体转化为简体
		//just_seg            默认False, 时候只进行分词，不进行词性标注
		//ufilter             默认False, 是否使用过滤器去除一些没有意义的词语，例如“可以”。
		//model_path          设置模型文件所在文件夹，默认为models/
		//separator           默认为‘_’, 设置词与词性之间的分隔符 
		HADES_FORCEINLINE void InitializeThulac(const char* model_path, const char* user_path, int just_seg, int t2s, int ufilter, char separator);
	};





	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ThulacFunction::InitializeThulac(const char* model_path, const char* user_path, int just_seg, int t2s, int ufilter, char separator)
	{
		m_ModelPath = model_path;
		m_UserPath = user_path;
		m_isSegOnly = just_seg;
		m_isT2S = t2s;
		m_isUfilter = ufilter;
		m_Separator = separator;
	}
}