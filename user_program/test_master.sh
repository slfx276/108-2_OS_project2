#! /bin/bash

# mmap vs mmap
sudo ./master ../data/file1_in mmap 
sudo ./master ../data/file2_in mmap 
sudo ./master ../data/file3_in mmap 
sudo ./master ../data/file4_in mmap 

# mmap vs fcntl
sudo ./master ../data/file1_in mmap 
sudo ./master ../data/file2_in mmap 
sudo ./master ../data/file3_in mmap 
sudo ./master ../data/file4_in mmap 

# fcntl vs mmap
sudo ./master ../data/file1_in fcntl 
sudo ./master ../data/file2_in fcntl 
sudo ./master ../data/file3_in fcntl 
sudo ./master ../data/file4_in fcntl 

# fcntl vs fcntl
sudo ./master ../data/file1_in fcntl 
sudo ./master ../data/file2_in fcntl 
sudo ./master ../data/file3_in fcntl 
sudo ./master ../data/file4_in fcntl 
