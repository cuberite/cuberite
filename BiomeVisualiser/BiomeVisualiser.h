
// BiomeVisualiser.h

// Declares the cBiomeVisualiser class representing the entire application





#include "BiomeViewWnd.h"





class cBiomeVisualiser
{
public:
	cBiomeVisualiser(void);
	
	int Run(void);
	
protected:
	cBiomeViewWnd m_MainWnd;
	
	cMCLogger m_Logger;
} ;




