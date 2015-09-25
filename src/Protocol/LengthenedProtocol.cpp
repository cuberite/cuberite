#include "Globals.h"
#include "LengthenedProtocol.h"

cProtocol::cProtocolError cLengthenedProtocol::DataReceived(const char * a_Data, size_t a_Size, std::vector<std::unique_ptr<cClientAction>> & a_Actions)
{
	a_Actions.clear();
	if (m_IsEncrypted)
	{
		ASSERT((a_Size % 16) == 0); // AES requirement.
		Byte Decrypted[512];
		while (a_Size > 0)
		{
			size_t NumBytes = (a_Size > sizeof(Decrypted)) ? sizeof(Decrypted) : a_Size;
			m_Decryptor.ProcessData(Decrypted, reinterpret_cast<const Byte *>(a_Data), NumBytes);
			auto success = AddReceivedData(reinterpret_cast<const char *>(Decrypted), NumBytes, a_Actions);
			if (success != cProtocolError::Success)
			{
				return success;
			}
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
		return cProtocolError::Success;
	}
	else
	{
		return AddReceivedData(a_Data, a_Size, a_Actions);
	}
}

cProtocol::cProtocolError cLengthenedProtocol::AddReceivedData(const char * a_Data, size_t a_Size, std::vector<std::unique_ptr<cClientAction>> & a_Actions)
{
	// Write the incoming data into the comm log file:
	if (g_ShouldLogCommIn)
	{
		if (m_ReceivedData.GetReadableSpace() > 0)
		{
			AString AllData;
			size_t OldReadableSpace = m_ReceivedData.GetReadableSpace();
			m_ReceivedData.ReadAll(AllData);
			m_ReceivedData.ResetRead();
			m_ReceivedData.SkipRead(m_ReceivedData.GetReadableSpace() - OldReadableSpace);
			ASSERT(m_ReceivedData.GetReadableSpace() == OldReadableSpace);
			AString Hex;
			CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
			m_CommLogFile.Printf("Incoming data, " SIZE_T_FMT " (0x" SIZE_T_FMT_HEX ") unparsed bytes already present in buffer:\n%s\n",
				AllData.size(), AllData.size(), Hex.c_str()
			);
		}
		AString Hex;
		CreateHexDump(Hex, a_Data, a_Size, 16);
		m_CommLogFile.Printf("Incoming data: %u (0x%x) bytes: \n%s\n",
			static_cast<unsigned>(a_Size), static_cast<unsigned>(a_Size), Hex.c_str()
		);
		m_CommLogFile.Flush();
	}

	if (!m_ReceivedData.Write(a_Data, a_Size))
	{
		return cProtocolError::BufferFull;
	}

	auto status = OnDataAddedToBuffer(m_ReceivedData, a_Actions);
	if (status != cProtocolError::Success)
	{
		return status;
	}

	
	// Log any leftover bytes into the logfile:
	if (g_ShouldLogCommIn && (m_ReceivedData.GetReadableSpace() > 0))
	{
		AString AllData;
		size_t OldReadableSpace = m_ReceivedData.GetReadableSpace();
		m_ReceivedData.ReadAll(AllData);
		m_ReceivedData.ResetRead();
		m_ReceivedData.SkipRead(m_ReceivedData.GetReadableSpace() - OldReadableSpace);
		ASSERT(m_ReceivedData.GetReadableSpace() == OldReadableSpace);
		AString Hex;
		CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
		m_CommLogFile.Printf("There are " SIZE_T_FMT " (0x" SIZE_T_FMT_HEX ") bytes of non-parse-able data left in the buffer:\n%s",
			m_ReceivedData.GetReadableSpace(), m_ReceivedData.GetReadableSpace(), Hex.c_str()
		);
		m_CommLogFile.Flush();
	}
	return cProtocolError::Success;
}
