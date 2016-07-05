
// VerticalLimit.h

// Declares the public interface for cPiece's cVerticalLimit implementations





#pragma once

#include "PieceGenerator.h"





/** Returns a new cPiece::cVerticalLimit descendant based on the specified description.
a_LimitDesc is in the format "<LimitClass>|<Params>". The params and the pipe may be omitted.
If an unknown class is requested or the param parsing fails, nullptr is returned.
If a_LogWarnings is true, any problem is reported into the server console. */
cPiece::cVerticalLimitPtr CreateVerticalLimitFromString(const AString & a_LimitDesc, bool a_LogWarnings);




