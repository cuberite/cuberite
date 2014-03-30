/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once





// Changes endianness
inline unsigned long long HostToNetwork8(const void* a_Value )
{
	unsigned long long __HostToNetwork8;
	memcpy( &__HostToNetwork8, a_Value, sizeof( __HostToNetwork8 ) );
	__HostToNetwork8 = (( ( (unsigned long long)htonl((u_long)__HostToNetwork8) ) << 32) + htonl(__HostToNetwork8 >> 32));
	return __HostToNetwork8;
}





inline unsigned int HostToNetwork4(const void* a_Value )
{
	unsigned int __HostToNetwork4;
	memcpy( &__HostToNetwork4, a_Value, sizeof( __HostToNetwork4 ) );
	__HostToNetwork4 = ntohl( __HostToNetwork4 );
	return __HostToNetwork4;
}





inline double NetworkToHostDouble8(const void* a_Value )
{
#define ntohll(x)   ((((unsigned long long)ntohl((u_long)x)) << 32) + ntohl(x >> 32))
	unsigned long long buf = 0;//(*(unsigned long long*)a_Value);
	memcpy( &buf, a_Value, 8 );
	buf = ntohll(buf);
	double x;
	memcpy(&x, &buf, sizeof(double));
	return x;
}





inline long long NetworkToHostLong8(const void * a_Value )
{
	unsigned long long buf = *(unsigned long long*)a_Value;
	buf = ntohll(buf);
	return *reinterpret_cast<long long *>(&buf);
}





inline float NetworkToHostFloat4(const void* a_Value )
{
	u_long buf = *(u_long*)a_Value;
	buf = ntohl( buf );
	float x = 0;
	memcpy( &x, &buf, sizeof(float) );
	return x;
}




