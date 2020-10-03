#pragma once

/** If set to true, the protocols will log each player's incoming (C->S) communication to a per-connection logfile. */
extern bool g_ShouldLogCommIn;

/** If set to true, the protocols will log each player's outgoing (S->C) communication to a per-connection logfile. */
extern bool g_ShouldLogCommOut;

/** If set to true, binary will attempt to run as a service. */
extern bool g_RunAsService;
