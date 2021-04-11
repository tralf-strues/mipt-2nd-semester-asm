MAIN_LEVEL=1

# # ================================ Failed #1 ================================
# make -f 02_makefile_failed1
# echo "====Tests failed_1===="

# cd bin 
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.failed1 ./02_failed1.out -d res/potter.txt -o res/potter.html
# for value in {1..3}
# do
#     ./02_failed1.out -d res/potter.txt -o res/potter.html
# done
# cd ..
# # ================================ Failed #1 ================================

# # ================================ Failed #2 ================================
# make -f 02_makefile_failed2
# echo "====Tests failed_2===="

# cd bin 
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.failed2 ./02_failed2.out -d res/potter.txt -o res/potter.html
# for value in {1..3}
# do
#     ./02_failed2.out -d res/potter.txt -o res/potter.html
# done
# cd ..
# # ================================ Failed #2 ================================

OUTPUT_OPTIMIZATION=res/02_optimize/optimization_tests.txt
echo "" > bin/$OUTPUT_OPTIMIZATION

# ================================ Hash table only ================================
# echo "====Callgrind hash table only -O$MAIN_LEVEL===="
# make -f 02_makefile_optimized OptimizationLevel=-O$MAIN_LEVEL OptimizationVersion=HASH_TABLE_ONLY
# echo ""
# cd bin 
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.02_hash_table_only ./02_optimized.out
# cd ..
# echo ""

for level in 3
do
    echo "====Tests hash table only -O$level====" >> bin/$OUTPUT_OPTIMIZATION
    make -f 02_makefile_optimized OptimizationLevel=-O$level OptimizationVersion=HASH_TABLE_ONLY >> bin/$OUTPUT_OPTIMIZATION
    echo "" >> bin/$OUTPUT_OPTIMIZATION

    cd bin 
    for value in {1..3}
    do
        ./02_optimized.out >> $OUTPUT_OPTIMIZATION
    done
    cd ..
    echo "" >> bin/$OUTPUT_OPTIMIZATION
done
# ================================ Hash table only ================================

echo "" >> bin/$OUTPUT_OPTIMIZATION

# ================================ CRC32 optimized ================================
# echo "====Callgrind crc32 optimized -O$MAIN_LEVEL===="
# make -f 02_makefile_optimized OptimizationLevel=-O$MAIN_LEVEL OptimizationVersion=CRC32_OPTIMIZED
# echo ""
# cd bin
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.02_crc32_optimized ./02_optimized.out
# cd ..
# echo ""

for level in 3
do
    echo "====Tests crc32 optimized -O$level====" >> bin/$OUTPUT_OPTIMIZATION
    make -f 02_makefile_optimized OptimizationLevel=-O$level OptimizationVersion=CRC32_OPTIMIZED >> bin/$OUTPUT_OPTIMIZATION
    echo "" >> bin/$OUTPUT_OPTIMIZATION

    cd bin 
    for value in {1..3}
    do
        ./02_optimized.out >> $OUTPUT_OPTIMIZATION
    done
    cd ..
    echo "" >> bin/$OUTPUT_OPTIMIZATION
done
# ================================ CRC32 optimized ================================

echo "" >> bin/$OUTPUT_OPTIMIZATION

# ================================ CRC32 + find optimized ================================
# echo "====Callgrind crc32 + find optimized -O$MAIN_LEVEL===="
# make -f 02_makefile_optimized OptimizationLevel=-O$MAIN_LEVEL OptimizationVersion=CRC32_OPTIMIZED OptimizeFind=1
# echo ""
# cd bin
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.02_crc32_find_optimized ./02_optimized.out
# cd ..
# echo ""

for level in 3
do
    echo "====Tests crc32 + find optimized -O$level====" >> bin/$OUTPUT_OPTIMIZATION
    make -f 02_makefile_optimized OptimizationLevel=-O$level OptimizationVersion=CRC32_OPTIMIZED OptimizeFind=1 >> bin/$OUTPUT_OPTIMIZATION
    echo "" >> bin/$OUTPUT_OPTIMIZATION

    cd bin 
    for value in {1..3}
    do
        ./02_optimized.out >> $OUTPUT_OPTIMIZATION
    done
    cd ..
    echo "" >> bin/$OUTPUT_OPTIMIZATION
done
# ================================ CRC32 + find optimized ================================

echo "" >> bin/$OUTPUT_OPTIMIZATION

# ================================ CRC32 + AVX optimized ================================
# echo "====Callgrind crc32 + avx2 optimized -O$MAIN_LEVEL===="
# make -f 02_makefile_optimized OptimizationLevel=-O$MAIN_LEVEL OptimizationVersion=CRC32_OPTIMIZED OptimizeAVX=1
# echo ""
# cd bin
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.02_crc32_avx2_optimized ./02_optimized.out
# cd ..
# echo ""

for level in 3
do
    echo "====Tests crc32 + avx2 optimized -O$level====" >> bin/$OUTPUT_OPTIMIZATION
    make -f 02_makefile_optimized OptimizationLevel=-O$level OptimizationVersion=CRC32_OPTIMIZED OptimizeAVX=1 >> bin/$OUTPUT_OPTIMIZATION
    echo "" >> bin/$OUTPUT_OPTIMIZATION

    cd bin 
    for value in {1..3}
    do
        ./02_optimized.out >> $OUTPUT_OPTIMIZATION
    done
    cd ..
    echo "" >> bin/$OUTPUT_OPTIMIZATION
done
# ================================ CRC32 + AVX optimized ================================

echo "" >> bin/$OUTPUT_OPTIMIZATION

# ================================ CRC32 + find + AVX optimized ================================
# echo "====Callgrind crc32 + find + avx2 optimized -O$MAIN_LEVEL===="
# make -f 02_makefile_optimized OptimizationLevel=-O$MAIN_LEVEL OptimizationVersion=CRC32_OPTIMIZED OptimizeFind=1 OptimizeAVX=1
# echo ""
# cd bin
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.02_crc32_find_avx2_optimized ./02_optimized.out
# cd ..
# echo ""

for level in 3
do
    echo "====Tests crc32 + find + avx2 optimized -O$level====" >> bin/$OUTPUT_OPTIMIZATION
    make -f 02_makefile_optimized OptimizationLevel=-O$level OptimizationVersion=CRC32_OPTIMIZED OptimizeFind=1 OptimizeAVX=1 >> bin/$OUTPUT_OPTIMIZATION
    echo "" >> bin/$OUTPUT_OPTIMIZATION

    cd bin 
    for value in {1..3}
    do
        ./02_optimized.out >> $OUTPUT_OPTIMIZATION
    done
    cd ..
    echo "" >> bin/$OUTPUT_OPTIMIZATION
done
# ================================ CRC32 + find + AVX optimized ================================