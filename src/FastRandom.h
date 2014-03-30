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

// FastRandom.h

// Declares the cFastRandom class representing a fast random number generator

/*
The cFastRandom aims to provide a very fast, although not very cryptographically secure, random generator.
It is fast to instantiate, fast to query next, and partially multi-thread-safe.
It is multi-thread-safe in the sense that it can be accessed from multiple threads without crashing, but it may
yield duplicate numbers in that case.

Internally, this works similar to cNoise's integral noise generation, with some predefined inputs: the seed is
taken from a global counter and the random is calculated using a counter that is incremented on each use (hence
the multi-thread duplication). Two alternatives exists for each function, one that takes a range parameter,
and another that takes an additional "salt" parameter; this salt is used as an additional input to the random,
in order to avoid multi-thread duplication. If two threads both use the class at the same time with different
salts, the values they get will be different.
*/





#pragma once





class cFastRandom
{
public:
	cFastRandom(void);
	
	/// Returns a random int in the range [0 .. a_Range - 1]; a_Range must be less than 1M
	int NextInt(int a_Range);

	/// Returns a random int in the range [0 .. a_Range - 1]; a_Range must be less than 1M; a_Salt is additional source of randomness
	int NextInt(int a_Range, int a_Salt);
	
	/// Returns a random float in the range [0 .. a_Range]; a_Range must be less than 1M
	float NextFloat(float a_Range);

	/// Returns a random float in the range [0 .. a_Range]; a_Range must be less than 1M; a_Salt is additional source of randomness
	float NextFloat(float a_Range, int a_Salt);
	
protected:
	int m_Seed;
	int m_Counter;
	
	/// Counter that is used to initialize the seed, incremented for each object created
	static int m_SeedCounter;
} ;




