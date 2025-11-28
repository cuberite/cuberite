# Build stage
FROM debian:bookworm-slim AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    git \
    make \
    cmake \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

# Copy source code
COPY . .

# Configure and build
RUN cmake . -DCMAKE_BUILD_TYPE=Release && \
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
COPY --from=builder /build/Server .

# Setup server directory
WORKDIR /server
VOLUME /server

# Copy entrypoint
COPY entrypoint.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/entrypoint.sh

# Expose ports
# 25565: Minecraft Server
# 8080: WebAdmin
EXPOSE 25565 8080

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
CMD ["./Cuberite", "--detached", "--no-output-buffering"]
