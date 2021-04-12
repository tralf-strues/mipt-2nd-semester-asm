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
set xrange[0:1031]

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 177.869" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output1.svg"
set title "Constant hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 0 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 56.8721" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output2.svg"
set title "String length hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 1 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 44.0791" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output3.svg"
set title "First character hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 2 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 5.16853" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output4.svg"
set title "Characters sum hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 3 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 4.44651" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output5.svg"
set title "Xor and right rotate hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 4 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 2.57654" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output6.svg"
set title "Xor and left rotate hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 5 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 2.64562" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output7.svg"
set title "Murmur3 hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 6 with boxes ls 1

set term svg dynamic size 5000 2000 background rgb "white"
set key title "Standard deviation = 2.27476" font ",40" offset 0, -3
set output "res/00_compare_hashes/size_1031/gnuplot_test_output8.svg"
set title "CRC32 hash function"
plot "res/00_compare_hashes/size_1031/plot_data.dat" using 1:2 index 7 with boxes ls 1

