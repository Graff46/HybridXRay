#ifndef dxObjectSpaceRender_included
#define dxObjectSpaceRender_included
#pragma once

#ifdef DEBUG

#include "ObjectSpaceRender.h"
class dxObjectSpaceRender : public IObjectSpaceRender
{
public:
	dxObjectSpaceRender();
	virtual ~dxObjectSpaceRender();
	virtual void Copy (IObjectSpaceRender &_in);

	virtual void dbgRender();
	virtual void dbgAddSphere(const Fsphere &sphere, u32 colour);
	virtual void SetShader();

private:
	ref_shader							m_shDebug;
	xr_vector<std::pair<Fsphere,u32> >	dbg_S;				// MT: dangerous
};

#endif // DEBUG

#endif	//	ObjectSpaceRender_included