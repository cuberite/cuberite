FROM ubuntu:xenial
 
# Add LLVM 6.0 repository
ADD https://apt.llvm.org/llvm-snapshot.gpg.key /root/gpg/llvm-snapshot.gpg.key
RUN apt-key add /root/gpg/llvm-snapshot.gpg.key && \
    echo "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main" >> /etc/apt/sources.list
 
# Install build dependencies
RUN apt-get update && \
    apt-get install -y git lua5.1 cmake clang-6.0 clang-tidy-6.0 python-yaml

RUN update-alternatives --install /usr/bin/clang      clang      /usr/bin/clang-6.0      600 && \
    update-alternatives --install /usr/bin/clang++    clang++    /usr/bin/clang++-6.0    600 && \
    update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-6.0 600 && \
    update-alternatives --install /usr/bin/run-clang-tidy.py run-clang-tidy.py \
        /usr/bin/run-clang-tidy-6.0.py 600 && \
    update-alternatives --install /usr/bin/clang-apply-replacements clang-apply-replacements \
        /usr/bin/clang-apply-replacements-6.0 600
