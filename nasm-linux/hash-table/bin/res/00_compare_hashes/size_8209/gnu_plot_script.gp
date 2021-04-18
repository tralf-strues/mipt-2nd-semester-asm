set key noautotitle
set xlabel "Bucket" font ",38" offset 0, -1
set ylabel "Bucket size" font ",38" offset -5, 0
set size ratio 0.4
set datafile separator whitespace
set title font ",48"
set xtics font ",36"
set ytics font ",36"
set style fill solid border lt -1
set style line 1 lc rgb 0x6495ED
set xrange[0:8209]

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 63.0621" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output1.svg"
set title "Constant hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 0 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 20.2385" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output2.svg"
set title "String length hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 1 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 15.7289" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output3.svg"
set title "First character hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 2 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 2.56614" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output4.svg"
set title "Characters sum hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 3 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 1.3157" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output5.svg"
set title "Xor and right rotate hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 4 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 0.970515" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output6.svg"
set title "Xor and left rotate hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 5 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 0.922905" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output7.svg"
set title "Murmur3 hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 6 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 0.83498" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output8.svg"
set title "CRC32 hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 7 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 0.82497" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output9.svg"
set title "CRC32 optimized hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 8 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 0.821389" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output10.svg"
set title "Polynomial hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 9 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 0.831559" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_8209/gnuplot_test_output11.svg"
set title "Polynomial optimized hash function"
plot "res/00_compare_hashes/size_8209/plot_data.dat" using 1:2 index 10 with boxes ls 1

