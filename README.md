
# **Caching Proxy**

A caching proxy server built with **C++** and **Boost.Asio**. It forwards HTTP requests to an origin server, caches the responses, and returns the cached responses for repeated requests.

# **Requirements**
This project depends on the following libraries:

Boost: For networking and command-line parsing.
nlohmann JSON: For handling JSON-based caching.
The instructions below are for Debian/Ubuntu. For other platforms, refer to their official documentation:
[Boost](https://www.boost.org/users/download)
[Nlohmann JSON](https://github.com/nlohmann/json)

# **Installing The Libraries**
**Nlohmann JSON**:
```shell
sudo apt install nlohmann-json3-dev
```
**Boost**:
```shell
sudo apt install libboost-all-dev
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

# **Thinsg to know**
The proxy doesn't handle redirects nor any SSL/TLS support for HTTPS protocols. It will work perfectly fine if ran with a origin that doesn't use HTTPS or doesn't redirect.

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
   ```bash
   ca --port 3000 --origin http://httpbin.org
   ```

   This starts the proxy on `http://localhost:3000`. Requests to `http://localhost:3000/get` will be forwarded to `http://httpbin.org/get`.

2. **Clear the cache**:
   ```bash
   ca --clear-cache
   ```



## **Features**

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
