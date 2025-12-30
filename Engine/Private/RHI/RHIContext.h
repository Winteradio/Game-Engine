#ifndef __WTR_RHICONTEXT_H__
#define __WTR_RHICONTEXT_H__

namespace wtr
{
	class RHIContext
	{
		public :
			RHIContext() = default;
			virtual ~RHIContext() = default;

		public :
			virtual bool Init() = 0;
			virtual void ShutDown() = 0;
	};
};

#endif // __WTR_RHICONTEXT_H__