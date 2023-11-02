----------------------------------------------------------------------------------------------------------------------------------
This is a Cache Simulator Project made for the 
Computer Architecture course at the University of Central Florida

Professor Suboh Suboh
Due: 11/02/2023
Student: Andrew Fugate, CpE undergrad

This code and associated files will be apart of a final report. 

This code delivers the LRU and FIFO policies and will run with provided
test inputs in the format of:
    W 0x7fff6c5b7b80
    R 0x7fff6c5b7c48

Some files are found on GitHub at the link provided below,
but due to size constraints not all test files could be uploaded.
Please see this link:
https://ucf-my.sharepoint.com/:f:/g/personal/an949235_ucf_edu/ElzYl_F5deRJjxjYwlBQnDYB3Xi7q9zlmxQdi-9i3qYsqw?e=75pm7e 
for access to all test files on all platforms. 
**- Please Note: this link is only accessible by UCF Microsoft accounts in the orginization __@ucf.edu -**

To compile this code please refer to the makefile.txt that was also delivered
with the report; or access the code base on GitHub @ https://github.com/Fugate-A/CacheSimulator 

----------------------------------------------------------------------------------------------------------------------------------

Output format (using: 32768 8 0 0 smallTest.t):

    Results:
    Miss ratio: 0.143000
    Writes: 392
    Reads: 143

    Extra Information:
            Hits: 857       Misses: 143

            Inputs:
            Cache Size: 32768       Associativity: 8
            Policy: LRU -- write_through

------------------------------------------------------------------------------------------------------------------------------