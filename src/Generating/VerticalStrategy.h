
// VerticalStrategy.h

// Declares the public interface for cPiece's cVerticalStrategy implementations





#pragma once

#include "PieceGenerator.h"





/** Returns a new cPiece::cVerticalStrategy descendant based on the specified description.
a_StrategyDesc is in the format "<StrategyClass>|<Params>". The params and the pipe may be omitted.
If an unknown class is requested or the param parsing fails, nullptr is returned. */
cPiece::cVerticalStrategyPtr CreateVerticalStrategyFromString(const AString & a_StrategyDesc, bool a_LogWarnings);




