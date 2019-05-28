#! /bin/bash

# mmap vs mmap
sudo ./slave ../data/file1_out mmap 127.0.0.1 
sudo ./slave ../data/file2_out mmap 127.0.0.1 
sudo ./slave ../data/file3_out mmap 127.0.0.1 
sudo ./slave ../data/file4_out mmap 127.0.0.1 

# mmap vs fcntl
sudo ./slave ../data/file1_out fcntl 127.0.0.1 
sudo ./slave ../data/file2_out fcntl 127.0.0.1 
sudo ./slave ../data/file3_out fcntl 127.0.0.1 
sudo ./slave ../data/file4_out fcntl 127.0.0.1 

# mmap vs mmap
sudo ./slave ../data/file1_out mmap 127.0.0.1 
sudo ./slave ../data/file2_out mmap 127.0.0.1 
sudo ./slave ../data/file3_out mmap 127.0.0.1 
sudo ./slave ../data/file4_out mmap 127.0.0.1 

# mmap vs fcntl
sudo ./slave ../data/file1_out fcntl 127.0.0.1 
sudo ./slave ../data/file2_out fcntl 127.0.0.1 
sudo ./slave ../data/file3_out fcntl 127.0.0.1 
sudo ./slave ../data/file4_out fcntl 127.0.0.1 
