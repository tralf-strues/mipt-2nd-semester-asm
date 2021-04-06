# Purpose
This project comprises of three different parts:
1. Comparing several hash functions in a dictionary-based application in terms of value distribution. 
2. Writing a terminal application that can print a definition to a single word or convert a .txt file into an html page with the same text, but where definition pops up on mouse hover of each word.
3. Analyzing the performance of this application and optimizing it with the use of assembly.  

# Contents
- **[Comparing hash functions](#comparing-hash-functions)**
  - [1. Constant](#1-constant)
  - [2. String length](#2-string-length)
  - [3. First character](#3-first-character)
  - [4. Characters sum](#4-characters-sum)
  - [5. Xor and right rotate](#5-xor-and-right-rotate)
  - [6. Xor and left rotate](#6-xor-and-left-rotate)
  - [7. Murmur3](#7-murmur3-smirk_cat)
  - [Conclusion](#conclusion)
- **[Application](#application)**
  - [Chosen algorithm and HashTable size](#chosen-algorithm-and-hashtable-size)
  - [Functionality](#functionality)
    - [Giving definition to a single word](#giving-definition-to-a-single-word)
    - [Creating an html page from a txt document](#creating-an-html-page-from-a-txt-document)
- **[Optimization](#optimization)**

# Comparing hash functions
A shortened dictionary database has been used with the total number of words equal to 5608. For higher objectiveness 5 hash table sizes have been considered (521, 1031, 2053, 4099, 8209). The hash functions being compared are the following:

## 1. Constant 
### Definition
Simply `H(string) = 0`.

### HashTable size = 1031
![](bin/res/00_compare_hashes/size_1031/gnuplot_test_output1.svg)

Obviously, this <del>hash function</del> is not applicable to anything.

## 2. String length 
### Definition
`H(string) = length(string)`

### HashTable size = 1031
![](bin/res/00_compare_hashes/size_1031/gnuplot_test_output2.svg)

The flaw of this hash function is that most of the elements are concentrated in the first few buckets of the hash table. This is due to the fact that the average English word's length is 4.7 characters, and as the number of buckets increases the distribution remains the same.

## 3. First character 
### Definition
`H(string) = string[0]`

### HashTable size = 1031
![](bin/res/00_compare_hashes/size_1031/gnuplot_test_output3.svg)

Almost the same as the previous one - the distribution doesn't change and no sign of homogeneity can be seen.

## 4. Characters sum 
### Definition
`H(string) = sum for i from 0 to length(string) - 1 of string[i]`

### HashTable size = 1031
![](bin/res/00_compare_hashes/size_1031/gnuplot_test_output4.svg)

### HashTable size = 4099
![](bin/res/00_compare_hashes/size_4099/gnuplot_test_output4.svg)

And again the distribution stays almost the same, though this time it's much better due to words with large hashes not going to the beginning because of being greater than the hash table's size. The range of elements convergence can be estimated as [97 * 4.7, 122 * 4.7] = [455.9, 573.4] (*97 - ASCII 'a', 122 - ASCII 'z' and 4.7 - average length of an English word*). The plot clearly shows a local peak in this interval, although there are others, the highest one being located at about [700, 800]. This can be explained by the fact that the dictionary database includes a plethora of long archaic words, which shift the distribution to the right.  

## 5. Xor and right rotate
### Definition
`H_0(string) = 0; H_i+1(string) = rightRotate(H_i(string)) xor string[i]`

### HashTable size = 1031
![](bin/res/00_compare_hashes/size_1031/gnuplot_test_output5.svg)

### HashTable size = 4099
![](bin/res/00_compare_hashes/size_4099/gnuplot_test_output5.svg)

## 6. Xor and left rotate
### Definition
`H_0(string) = 0; H_i+1(string) = leftRotate(H_i(string)) xor string[i]`

### HashTable size = 1031
![](bin/res/00_compare_hashes/size_1031/gnuplot_test_output6.svg)

### HashTable size = 4099
![](bin/res/00_compare_hashes/size_4099/gnuplot_test_output6.svg)

As can be seen from the diagrams, left rotation does a better job compared to right rotation. This can probably be explained by the nature of data we are analyzing the hash table on. Each byte is an ASCII character and less than 128. This means the most significant bit of each byte is 0. Let us consider an example: take the first character be 'k' or 107 in decimal or 01101011 in binary. For simplicity suppose the hash is 16 bit, not 32.

```
                             Not extended ASCII range
                               |=================|
15 14 13 12 11 10  9  8     7  6  5  4  3  2  1  0
-----------------------     ----------------------
 0  0  0  0  0  0  0  0     0  1  1  0  1  0  1  1    hash = 'k' 
                            ^
 1  0  0  0  0  0  0  0     0  0  1  1  0  1  0  1    rightRotate(hash, 1)
                            ^  
                        ...
 *  *  *  *  *  *  *  1     0  *  *  *  *  *  *  *
                            ^
```

During the first 8 iterations of the hash function, 7th bit is always 0, i.e. it doesn't influence the result hash at all which leads to a more uneven distribution than that of the left rotate. If we now look at it

```
                             Not extended ASCII range
                               |=================|
15 14 13 12 11 10  9  8     7  6  5  4  3  2  1  0
-----------------------     ----------------------
 0  0  0  0  0  0  0  0     0  1  1  0  1  0  1  1    hash = 'k' 
                            
 0  0  0  0  0  0  0  0     1  1  0  1  0  1  1  0    leftRotate(hash, 1)
```

it's clear that all 8 least significant bits influence the result. 
## 7. Murmur3 :smirk_cat:
### Definition
32-bit third version of [aappleby](https://github.com/aappleby)'s hash function. See [this](http://www.self.gutenberg.org/articles/MurmurHash) article for more information on the subject. 

### HashTable size = 1031
![](bin/res/00_compare_hashes/size_1031/gnuplot_test_output7.svg)

### HashTable size = 4099
![](bin/res/00_compare_hashes/size_4099/gnuplot_test_output7.svg)

Unsurprisingly, this hashing algorithm has the best bucket distribution, which is expected for it involves significantly more operations and has been invented with multiple tests and research.

>More diagrams for sizes 521, 2053, 8209 as well as the shortened dictionary used can be found in "bin/res/00_compare_hashes/".

## Conclusion

Here are the standard deviation for all the tests:
Algorithm|521|1031|2053|4099|8209
---------|---|----|----|----|----
Constant|245.28|174.45|123.65|87.52|61.85
String length|76.54|54.67|38.84|27.52|19.46
First character|60.02|43|30.6|21.7|15.35
Characters sum|7.41|4.98|4.36|3.36|2.47
Xor and right rotate|8.2|4.18|2.28|1.74|1.25
Xor and left rotate|3.57|2.53|1.88|1.35|0.95
Murmur3|3.52|2.65|1.79|1.27|0.9

Murmur3 and the algorithm using exclusive or alongside left bitwise rotation have shown the best results. Between them, Murmur3's standard deviation (*1.266 on 4099 HashTable size*) is less than left rotation based algorithm's (*1.35 on 4099 HashTable size*). In addition Murmur3 has a more homogeneous distribution. But it comes with a higher cost.

# Application
## Chosen algorithm and HashTable size 
The full dictionary contains 121199 words. Below are the tests for [load factors](https://en.wikipedia.org/wiki/Hash_table#Key_statistics) equal to 0.75 and 0.95.

>size1 = 121,199 / 0.75 = 161,598.67 or 161,599 (the nearest prime number to it)\
>size2 = 121,199 / 0.95 = 127,577.89 or 127,579 (the nearest prime number to it)

### Load factor = 0.75

![](bin/res/00_compare_hashes/size_161599/gnuplot_test_output6.svg)
![](bin/res/00_compare_hashes/size_161599/gnuplot_test_output7.svg)

### Load factor = 0.95

![](bin/res/00_compare_hashes/size_127579/gnuplot_test_output6.svg)
![](bin/res/00_compare_hashes/size_127579/gnuplot_test_output7.svg)

Left rotate algorithm shows itself quite badly here. In addition, the difference between load factors isn't that drastic for Murmur3, so 0.95 has been chosen in order to save some memory.

## Functionality
Help message looks like the following
```
[Help]
These are commands for define:
  -h, --help   [show this message]
  -w, --word   [define just one word]
  -d, --doc    [specify input document]
  -o, --output [specify output file]
```
As can be seen the program supports two different modes:

### Giving definition to a single word
Example input
```
$ ./define.out -w Surprise
```
Example output
```
Definitions for 'surprise'
1. [n] The act of coming upon, or taking, unawares; the act of seizing unexpectedly; surprisal; as, the fort was taken by surprise.
2. [n] The state of being surprised, or taken unawares, by some act or event which could not reasonably be foreseen; emotion excited by what is sudden and strange; a suddenly excited feeling of wonder or astonishment.
3. [n] Anything that causes such a state or emotion.
4. [n] A dish covered with a crust of raised paste, but with no other contents.
```

### Creating an html page from a txt document 
The idea is you can hover over words and see their definitions. For example if we consider the first chapter (taken from [here](https://genius.com/J-r-r-tolkien-the-hobbit-chap-1-an-unexpected-party-annotated)) of J.R.R. Tolkien's [Hobbit](https://en.wikipedia.org/wiki/The_Hobbit), then the result will be something like this (note that there might be visual differences in different web browsers):
```
$ ./define.out --doc hobbit_chapter1.txt -o hobbit_chapter1.html
```
![zoom=](images/hobbit_screenshot.png)

The word hovered on is 'door'.

# Optimization
TODO:
## Choosing testing strategy
### Failed attempt #1
Let's first try to analyze performance of the program on a txt file containing books "Harry Potter and the Chamber of Secrets", "Harry Potter and the Goblet of Fire", "Harry Potter and the Half-blood Prince", "Harry Potter and the Deathly Hallows" (30,684 lines and 2,646,069 characters total).

Test time in average is TODO:
```
Time: 2843.53 ms
Time: 2699.05 ms
Time: 2608.24 ms
```

Let's look at the profiler

![](bin/res/../../images/callgrind_failed1.png)

It's clear that the majority of CPU time is taken by io functions, which makes the test not objective. 

### Failed attempt #2
Here I have removed writing to output file.

Test time in average is TODO:
``` 
Time: 2843.53 ms
Time: 2699.05 ms
Time: 2608.24 ms
```

Let's look at the profiler

![](bin/res/../../images/callgrind_failed2.png)

Still there functions at the top which aren't related to the Hash Table.

### Final testing program
In order to minimize the time required by loading and parsing input file, I have found a [file](https://github.com/dwyl/english-words) containing just 466,472 words (with no definition whatsoever). 

Test time in average is TODO:
``` 
Time: 2843.53 ms
Time: 2699.05 ms
Time: 2608.24 ms
```

Let's look at the profiler

![](bin/res/../../images/callgrind_hash_table_only.png)

This is much better! Now we can start optimizing the Hash Table.

## Hash function optimization
