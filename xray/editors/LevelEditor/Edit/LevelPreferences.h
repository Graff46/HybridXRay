//---------------------------------------------------------------------------
#ifndef LevelPreferencesH
#define LevelPreferencesH

#include "../ECore/Editor/EditorPreferences.h"

class CLevelPreferences: public CCustomPreferences
{
	typedef CCustomPreferences			inherited;
	void			OnEnabledChange		(PropValue* prop);
	void			OnReadonlyChange	(PropValue* prop);
protected:
    virtual void 	Load				(CInifile*);
    virtual void 	Save				(CInifile*);
public:
    virtual void	FillProp          	(PropItemVec& items);
};
//---------------------------------------------------------------------------
#endif
