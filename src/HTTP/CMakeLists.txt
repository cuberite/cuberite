project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
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
)

SET (HDRS
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

if(NOT MSVC)
	add_library(HTTPServer ${SRCS} ${HDRS})
	target_link_libraries(HTTPServer fmt::fmt)
endif()
