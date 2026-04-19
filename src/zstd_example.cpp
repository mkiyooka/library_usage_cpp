#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <zstd.h>

int main() {
    std::cout << "=== Zstandard (zstd) Example ===\n\n";
    std::cout << "zstd version: " << ZSTD_versionString() << "\n\n";

    // 圧縮対象データ
    std::string original(10000, '\0');
    for (size_t i = 0; i < original.size(); ++i) {
        original[i] = static_cast<char>('A' + (i % 26));
    }

    // --- 圧縮 ---
    const size_t bound = ZSTD_compressBound(original.size());
    std::vector<char> compressed(bound);

    const size_t compressed_size =
        ZSTD_compress(compressed.data(), bound, original.data(), original.size(), 3);
    assert(!ZSTD_isError(compressed_size));
    compressed.resize(compressed_size);

    std::cout << "Original size:    " << original.size() << " bytes\n";
    std::cout << "Compressed size:  " << compressed_size << " bytes\n";
    std::cout << "Ratio:            " << (100.0 * compressed_size / original.size()) << "%\n\n";

    // --- 解凍 ---
    const unsigned long long decompressed_bound =
        ZSTD_getFrameContentSize(compressed.data(), compressed.size());
    std::vector<char> decompressed(decompressed_bound);

    const size_t decompressed_size = ZSTD_decompress(
        decompressed.data(), decompressed_bound, compressed.data(), compressed.size());
    assert(!ZSTD_isError(decompressed_size));

    assert(decompressed_size == original.size());
    assert(std::memcmp(decompressed.data(), original.data(), original.size()) == 0);
    std::cout << "Decompression OK, size = " << decompressed_size << " bytes\n\n";

    // 圧縮レベル別比較
    std::cout << "Compression level comparison:\n";
    for (int level : {1, 3, 9, 19}) {
        std::vector<char> buf(bound);
        size_t sz = ZSTD_compress(buf.data(), bound, original.data(), original.size(), level);
        std::cout << "  level " << level << ": " << sz << " bytes ("
                  << (100.0 * sz / original.size()) << "%)\n";
    }

    return 0;
}
