
# **Caching Proxy**

A caching proxy server built with **C++** and **Boost.Asio**. It forwards HTTP requests to an origin server, caches the responses, and returns the cached responses for repeated requests.

# **Requirements**
You need to have the Boost library and the Nlohmann JSON header file alone installed.
I'll be showing how to get these in Debian/Ubuntu, for other platforms, visit their respective documentations: https://github.com/nlohmann and https://www.boost.org/users/download/

# **Installing The Libraries**
Nlohmann JSON:
```shell
sudo apt install nlohmann-json3-dev
```
Boost:
```shell
sudo apt install libboost-all-dev
```

# **Building The Repository**
```shell
git clone https://github.com/LawlietDN/Ca
cd Ca
mkdir build
cd build
cmake ..
make
sudo mv ca /usr/bin
```
After completing these steps, there isn't anything left except for testing the tool.

# **Thinsg to know**
The proxy doesn't handle redirects nor any SSL/TLS support for HTTPS protocols. It will work perfectly fine if ran with a origin that doesn't use HTTPS or doesn't redirect.

# **Usage**
ca --port <port> --origin <origin>
  --port:           The port number on which the proxy server will 
                        run(e.g., 12345).
  --origin:         The URL of the server to which the requests will be 
                        forwarded.(e.g., https://example.com).
  --clear-cache         To clear all cached responses.
  
# **The Project**
It has several features:
1. It forwards all incoming requests to the specified origin server. For example, if you run the tool with ca --port 12345 --origin http://httpbin.org the proxy will be running on http://localhost:12345 (127.0.0.1:12345) and will be forwarding any requests to  http://httpbin.org. If a requets to http://localhost:12345/get is made it'll forward that request to  http://httpbin.org/get cache the response then return it.
3. It stores the server's responses locally in a cache.json file. If the same request is made again, the cached response is returned immediately.
4. It adds X-Cache: HIT or X-Cache: MISS to indicate whether the response came from the cache or the origin server.
5. CLI Commands:
Start the proxy on a custom port.
Specify the origin server.
Clear the cache when needed.
