emcc -O3 -s TOTAL_MEMORY=50331648 -s EXPORTED_FUNCTIONS="['_ZSTD_decompress', '_ZSTD_compress', '_ZSTD_compressBound', '_ZSTD_isError', '_ZSTD_getErrorName']" zbuff.c divsufsort.c divsufsort.c fse.c huff0.c zbuff.c zdict.c zstd_compress.c zstd_decompress.c -o zstd.js