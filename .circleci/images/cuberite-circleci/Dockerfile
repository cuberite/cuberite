FROM ubuntu:bionic

# Install build dependencies
RUN apt-get update && \
	apt-get install -y git lua5.1 cmake clang-6.0 clang-tidy-6.0 python-pip && \
	pip install PyYAML 

RUN update-alternatives --install /usr/bin/clang             clang             /usr/bin/clang-6.0             600 && \
	update-alternatives --install /usr/bin/clang++           clang++           /usr/bin/clang++-6.0           600 && \
	update-alternatives --install /usr/bin/clang-tidy        clang-tidy        /usr/bin/clang-tidy-6.0        600 && \
	update-alternatives --install /usr/bin/run-clang-tidy.py run-clang-tidy.py /usr/bin/run-clang-tidy-6.0.py 600
