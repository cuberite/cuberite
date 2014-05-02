
// BufferedSslContext.cpp

// Implements the cBufferedSslContext class representing a SSL context with the SSL peer data backed by a cByteBuffer

#include "Globals.h"
#include "BufferedSslContext.h"





cBufferedSslContext::cBufferedSslContext(size_t a_BufferSize):
	m_OutgoingData(a_BufferSize),
	m_IncomingData(a_BufferSize)
{
}





int cBufferedSslContext::ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes)
{
	// Called when PolarSSL wants to read encrypted data from the SSL peer
	// Read the data from the buffer inside this object, where the owner has stored them using WriteIncoming():
	size_t NumBytes = std::min(a_NumBytes, m_IncomingData.GetReadableSpace());
	if (NumBytes == 0)
	{
		return POLARSSL_ERR_NET_WANT_READ;
	}
	if (!m_IncomingData.ReadBuf(a_Buffer, NumBytes))
	{
		m_IncomingData.ResetRead();
		return POLARSSL_ERR_NET_RECV_FAILED;
	}
	m_IncomingData.CommitRead();
	return (int)NumBytes;
}





int cBufferedSslContext::SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes)
{
	// Called when PolarSSL wants to write encrypted data to the SSL peer
	// Write the data into the buffer inside this object, where the owner can later read them using ReadOutgoing():
	if (!m_OutgoingData.CanWriteBytes(a_NumBytes))
	{
		return POLARSSL_ERR_NET_WANT_WRITE;
	}
	if (!m_OutgoingData.Write((const char *)a_Buffer, a_NumBytes))
	{
		return POLARSSL_ERR_NET_SEND_FAILED;
	}
	return (int)a_NumBytes;
}




