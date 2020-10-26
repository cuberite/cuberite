target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	EnvelopeParser.cpp
	HTTPFormParser.cpp
	HTTPMessage.cpp
	HTTPMessageParser.cpp
	HTTPServer.cpp
	HTTPServerConnection.cpp
	MultipartParser.cpp
	NameValueParser.cpp
	SslHTTPServerConnection.cpp
	TransferEncodingParser.cpp
	UrlClient.cpp
	UrlParser.cpp

	EnvelopeParser.h
	HTTPFormParser.h
	HTTPMessage.h
	HTTPMessageParser.h
	HTTPServer.h
	HTTPServerConnection.h
	MultipartParser.h
	NameValueParser.h
	SslHTTPServerConnection.h
	TransferEncodingParser.h
	UrlClient.h
	UrlParser.h
)
