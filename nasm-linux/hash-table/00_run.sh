###################################
echo ""
echo "====Testing distributions of hash functions===="
make -f 00_makefile OptimizationLevel=-O3
echo ""

cd bin
./00_compare_hashes.out
cd ..
###################################

###################################
echo ""
echo ""

for level in {0..1}
do
    echo "====Testing speed of hash functions -O$level===="
    make -f 00_makefile_speed_tests OptimizationLevel=-O$level

    cd bin
    ./00_speed_tests.out
    cd ..
    echo ""
    echo ""
done
###################################