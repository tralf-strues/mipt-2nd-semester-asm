MAIN_LEVEL=1

###################################
echo ""
echo "====Testing distributions of hash functions===="
make -f 00_makefile OptimizationLevel=-O$MAIN_LEVEL
echo ""

cd bin
./00_compare_hashes.out
cd ..
###################################

##################################
OUTPUT_SPEED_TEST=res/00_compare_hashes/speed_comparison.txt

echo "" > bin/$OUTPUT_SPEED_TEST

for level in {0..3}
do
    echo "====Testing speed of hash functions -O$level====" >> bin/$OUTPUT_SPEED_TEST
    make -f 00_makefile_speed_tests OptimizationLevel=-O$level >> bin/$OUTPUT_SPEED_TEST

    cd bin
    ./00_speed_tests.out >> $OUTPUT_SPEED_TEST
    cd ..
    echo "" >> bin/$OUTPUT_SPEED_TEST
    echo "" >> bin/$OUTPUT_SPEED_TEST
done
##################################