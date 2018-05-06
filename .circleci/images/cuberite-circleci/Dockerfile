FROM ubuntu:trusty

# Add LLVM 6.0 repository
RUN apt-get update && \
	apt-get install -y software-properties-common wget && \
	wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|apt-key add - && \
	apt-add-repository "deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-6.0 main" && \
	apt-add-repository ppa:ubuntu-toolchain-r/test

# Install build dependencies
RUN apt-get update && \
	apt-get install -y git lua5.1 cmake clang-6.0 clang-tidy-6.0 python-pip && \
	pip install PyYAML 

RUN update-alternatives --install /usr/bin/clang             clang             /usr/bin/clang-6.0          600 && \
	update-alternatives --install /usr/bin/clang++           clang++           /usr/bin/clang++-6.0        600 && \
	update-alternatives --install /usr/bin/clang-tidy        clang-tidy        /usr/bin/clang-tidy-6.0     600 && \
	update-alternatives --install /usr/bin/run-clang-tidy.py run-clang-tidy.py /usr/bin/run-clang-tidy-6.0 600
