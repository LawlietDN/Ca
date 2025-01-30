
# **Caching Proxy**

A CLI tool for running a caching proxy server that supports both HTTP and HTTPS. The proxy listens for incoming requests, forwards them to the specified origin server, caches the responses, and serves cached responses for repeated requests.

# **Requirements**
This project depends on the following libraries:

**Boost**: For networking and command-line parsing.
**nlohmann JSON**: For handling JSON-based caching.
**OpenSSL: For handling HTTPS requests.**
The instructions below are for Debian/Ubuntu. For other platforms, refer to their official documentation:

[Boost](https://www.boost.org/users/download)
[Nlohmann JSON](https://github.com/nlohmann/json)
[OpenSSL](https://openssl-library.org/source/)

# **Installing The Libraries**
**Nlohmann JSON**:
```bash
sudo apt install nlohmann-json3-dev
```
**Boost**:
```bash
sudo apt install libboost-all-dev
```
**OpenSSL**:
```bash
sudo apt install libssl-dev
```
## **Building the Project**

1. Clone the repository:
   ```bash
   git clone https://github.com/LawlietDN/Ca
   cd Ca
   ```

2. Create a build directory and compile:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Move the compiled binary to a system-wide location (optional):
   ```bash
   sudo mv ca /usr/bin
   ```
After completing these steps, there isn't anything left except for testing the tool.

# **What to know**
The proxy does not handle redirects yet. It only works with HTTP/HTTPS origins that do not redirect.

## **Usage**

Run the tool with the following command:
```bash
ca --port <port> --origin <origin>
```

### **Options**:

- `--port`: The port number on which the proxy server will run (e.g., `12345`).
- `--origin`: The URL of the origin server to forward requests to (e.g., `http://example.com`).
- `--clear-cache`: Clears all cached responses.

### **Examples**:

1. **Start the proxy**:
   **For HTTP**:
   ```bash
   ca --port 3000 --origin http://httpbin.org
   ```
   **For HTTPS**:
   ```bash
   ca --port 3000 --origin https://roadmap.sh/
   ```
   This starts the proxy on `http://localhost:3000`. Requests to `http://localhost:3000/something` will be forwarded to `http://httpbin.org/get` or `https://roadmap.sh/something`.

3. **Clear the cache**:
   ```bash
   ca --clear-cache
   ```



## **Features**
1. **HTTPS Support**:
   The proxy now supports TLS and can forward requests to HTTPS origins.
   It also handles TLS handshakes properly.
1. **Request Forwarding**:  
   Forwards all incoming requests to the specified origin server and returns the origin’s response to the client.

2. **Caching**:  
   Responses are stored locally in a `cache.json` file. If the same request is made again, the cached response is returned, significantly reducing latency.

3. **Cache Headers**:  
   Adds the following headers to the responses:
   - `X-Cache: HIT` – Indicates the response came from the cache.
   - `X-Cache: MISS` – Indicates the response was fetched from the origin server.

4. **CLI Commands**:  
   - Start the proxy server with a custom port and origin.
   - Clear the cache using a simple command.


Solution for https://roadmap.sh/projects/caching-server.
