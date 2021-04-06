# make -f 02_makefile_failed1
# echo "====Tests failed_1===="

# cd bin 
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.failed1 ./02_failed1.out -d res/potter.txt -o res/potter.html
# for value in {1..3}
# do
#     ./02_failed1.out -d res/potter.txt -o res/potter.html
# done
# cd ..

# make -f 02_makefile_failed2
# echo "====Tests failed_2===="

# cd bin 
# valgrind --tool=callgrind --callgrind-out-file=callgrind.out.failed2 ./02_failed2.out -d res/potter.txt -o res/potter.html
# for value in {1..3}
# do
#     ./02_failed2.out -d res/potter.txt -o res/potter.html
# done
# cd ..

make -f 02_makefile_hash_table_only
echo "====Tests hash table only===="

cd bin 
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.hash_table_only ./02_hash_table_only.out
for value in {1..3}
do
    ./02_hash_table_only.out
done
cd ..






