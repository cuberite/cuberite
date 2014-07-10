#include "ModelComponent.h"

class cMobModel : public cModelComponent {
	int m_MobType;
	cMobModel(cEntity * a_Entity, int a_MobType): cModelComponent(a_Entity), m_MobType(a_MobType){}
}