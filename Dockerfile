# Build stage
FROM debian:bookworm-slim AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    git \
    make \
    cmake \
    g++ \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

# Clone and build
RUN git clone --depth 1 https://github.com/cuberite/cuberite.git . && \
    git submodule sync && \
    git submodule update --init --recursive && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Runtime stage
FROM debian:bookworm-slim

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    ca-certificates \
    gosu \
    rsync \
    && rm -rf /var/lib/apt/lists/*

# Create a non-root user
RUN useradd -m -U -d /server cuberite

# Create app directory for template files
WORKDIR /app
# Copy the actual data files from source Server/ (not symlinked build/Server/)
COPY --from=builder /build/Server/ /app/
# Copy the compiled binary
COPY --from=builder /build/build/Server/Cuberite /app/Cuberite

# Setup server directory
WORKDIR /server
VOLUME /server

# Copy entrypoint
COPY entrypoint.sh /usr/local/bin/
RUN sed -i 's/\r$//' /usr/local/bin/entrypoint.sh && \
    chmod +x /usr/local/bin/entrypoint.sh

# Expose ports
# 25565: Minecraft Server
# 8080: WebAdmin
EXPOSE 25565 8080

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
CMD ["./Cuberite", "--detached", "--no-output-buffering"]
