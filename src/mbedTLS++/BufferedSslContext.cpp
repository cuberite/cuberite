
// BufferedSslContext.cpp

// Implements the cBufferedSslContext class representing a SSL context with the SSL peer data backed by a cByteBuffer

#include "Globals.h"
#include "BufferedSslContext.h"





cBufferedSslContext::cBufferedSslContext(size_t a_BufferSize):
	m_OutgoingData(a_BufferSize),
	m_IncomingData(a_BufferSize)
{
}





size_t cBufferedSslContext::WriteIncoming(const void * a_Data, size_t a_NumBytes)
{
	size_t NumBytes = std::min(m_IncomingData.GetFreeSpace(), a_NumBytes);
	if (NumBytes > 0)
	{
		m_IncomingData.Write(a_Data, NumBytes);
		return NumBytes;
	}
	return 0;
}





size_t cBufferedSslContext::ReadOutgoing(void * a_Data, size_t a_DataMaxSize)
{
	size_t NumBytes = std::min(m_OutgoingData.GetReadableSpace(), a_DataMaxSize);
	if (NumBytes > 0)
	{
		m_OutgoingData.ReadBuf(a_Data, NumBytes);
		m_OutgoingData.CommitRead();
		return NumBytes;
	}
	return 0;
}





int cBufferedSslContext::ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes)
{
	// Called when mbedTLS wants to read encrypted data from the SSL peer
	// Read the data from the buffer inside this object, where the owner has stored them using WriteIncoming():
	size_t NumBytes = std::min(a_NumBytes, m_IncomingData.GetReadableSpace());
	if (NumBytes == 0)
	{
		return MBEDTLS_ERR_SSL_WANT_READ;
	}
	if (!m_IncomingData.ReadBuf(a_Buffer, NumBytes))
	{
		m_IncomingData.ResetRead();
		return MBEDTLS_ERR_NET_RECV_FAILED;
	}
	m_IncomingData.CommitRead();
	return static_cast<int>(NumBytes);
}





int cBufferedSslContext::SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes)
{
	// Called when mbedTLS wants to write encrypted data to the SSL peer
	// Write the data into the buffer inside this object, where the owner can later read them using ReadOutgoing():
	if (!m_OutgoingData.CanWriteBytes(a_NumBytes))
	{
		return MBEDTLS_ERR_SSL_WANT_WRITE;
	}
	if (!m_OutgoingData.Write(reinterpret_cast<const char *>(a_Buffer), a_NumBytes))
	{
		return MBEDTLS_ERR_NET_SEND_FAILED;
	}
	return static_cast<int>(a_NumBytes);
}




