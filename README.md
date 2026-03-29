# bitcoinsolar-opensea-client
# BitcoinSolar OpenSea C++ Client

Production-style C++ client for:
- NFT gallery
- Rarity explorer
- Metadata viewer
- Marketplace (read-only)
- Minting dashboard (monitoring)

## Build

```bash
mkdir build && cd build
cmake ..
make


Requires:

• libcurl
• C++17
• nlohmann::json (included as third_party/json.hpp)


Set your MCP token:

export OPENSEA_MCP_TOKEN=your_token_here
./bitcoinsolar_opensea_client
