
// CallbackSslContext.cpp

// Declares the cCallbackSslContext class representing a SSL context wrapper that uses callbacks to read and write SSL peer data

#include "Globals.h"
#include "CallbackSslContext.h"






cCallbackSslContext::cCallbackSslContext(void) :
	m_Callbacks(nullptr)
{
	// Nothing needed, but the constructor needs to exist so
}





cCallbackSslContext::cCallbackSslContext(cCallbackSslContext::cDataCallbacks & a_Callbacks) :
	m_Callbacks(&a_Callbacks)
{
}





int cCallbackSslContext::ReceiveEncrypted(unsigned char * a_Buffer, size_t a_NumBytes)
{
	if (m_Callbacks == nullptr)
	{
		LOGWARNING("SSL: Trying to receive data with no callbacks, aborting.");
		return MBEDTLS_ERR_NET_RECV_FAILED;
	}
	return m_Callbacks->ReceiveEncrypted(a_Buffer, a_NumBytes);
}





int cCallbackSslContext::SendEncrypted(const unsigned char * a_Buffer, size_t a_NumBytes)
{
	if (m_Callbacks == nullptr)
	{
		LOGWARNING("SSL: Trying to send data with no callbacks, aborting.");
		return MBEDTLS_ERR_NET_SEND_FAILED;
	}
	return m_Callbacks->SendEncrypted(a_Buffer, a_NumBytes);
}





