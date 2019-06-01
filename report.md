# 1. Design

* a. two User Programs:  
one for accessing the slave device (slave side) and the other for accessing the master device (master side),

> slave-side program :  
    it first uses ioctl to set the IP address of master, then receives the data from slave device and writes all of them to the output file by specified method.

> master-side program :  
read the input file by the specified method and send the data to the master device.

* b. two Kernel Programs:  

> master-side kernel :  
    receive the data from master-side user program, and send to master device (master side).  
    
> slave-side kernel :  
    receive the data from master device, and transmit to user program (slave side), then slave-side user program will write to ouput file.  
  


Once the transmission has completed, the programs will show the total transmission time, from the time stamp when device is opened, to the time stamp when device is closed. And display the size of the transferred file in both computers. All the parameters are passed to programs through standard input.

* modify Master transmission time 
另外特別修改了 sample code 中 master.c 的 transmission time 計算方式，take off waiting time 以去除 master 和 slave 間隔執行時間的影響，因此這個 project 建議**先執行 master 再執行 slave**。

* Segmentation fault
有時候執行的時候 slave 端會出現 Segmentation fault，經過檢查 pointer 和 array 後仍偶爾會遇到，只能重新執行 master/slave，目前還沒找出確切原因 QQ

---

# 2. Results

### a. Master: mmap ,  Slave: mmap

```
// file1_in
Master:
Transmission time: 0.002800 ms, File size: 4 bytes
Slave:
Transmission time: 0.031800 ms, File size: 4 bytes

// file2_in
Master:
Transmission time: 0.003800 ms, File size: 577 bytes
Slave:
Transmission time: 0.168200 ms, File size: 577 bytes

// file3_in
Master:
Transmission time: 0.008400 ms, File size: 9695 bytes
Slave:
Transmission time: 0.145600 ms, File size: 9695 bytes

// file4_in
Master:
Transmission time: 0.692200 ms, File size: 1502860 bytes
Slave:
Transmission time: 1.672500 ms, File size: 1502860 bytes
```

### b. Master: mmap ,  Slave: fcntl

```
// file1_in
Master:
Transmission time: 0.003600 ms, File size: 4 bytes
Slave:
Transmission time: 0.051100 ms, File size: 4 bytes

// file2_in
Master:
Transmission time: 0.004900 ms, File size: 577 bytes
Slave:
Transmission time: 0.061000 ms, File size: 577 bytes

// file3_in
Master:
Transmission time: 0.008200 ms, File size: 9695 bytes
Slave:
Transmission time: 0.079900 ms, File size: 9695 bytes

// file4_in
Master:
Transmission time: 1.580700 ms, File size: 1502860 bytes
Slave:
Transmission time: 5.874400 ms, File size: 1502860 bytes
```


### c. Master: fcntl ,  Slave: mmap

```
// file1_in
Master:
Transmission time: 0.002400 ms, File size: 4 bytes
Slave:
Transmission time: 0.031900 ms, File size: 4 bytes

// file2_in
Master:
Transmission time: 0.008700 ms, File size: 577 bytes
Slave:
Transmission time: 0.042200 ms, File size: 577 bytes

// file3_in
Master:
Transmission time: 0.082200 ms, File size: 9695 bytes
Slave:
Transmission time: 0.116700 ms, File size: 9695 bytes

// file4_in
Master:
Transmission time: 3.697800 ms, File size: 1502860 bytes
Slave:
segmentation fault 3.901300 ms, File size: 1502860 bytes

```

### d. Master: fcntl ,  Slave: fcntl

```
// file1_in
Master:
Transmission time: 0.002500 ms, File size: 4 bytes
Slave:
Transmission time: 0.030300 ms, File size: 4 bytes

// file2_in
Master:
Transmission time: 0.013700 ms, File size: 577 bytes
Slave:
Transmission time: 0.086000 ms, File size: 577 bytes

// file3_in
Master:
Transmission time: 0.068700 ms, File size: 9695 bytes
Slave:
Transmission time: 0.096900 ms, File size: 9695 bytes

// file4_in
Master:
Transmission time: 3.556600 ms, File size: 1502860 bytes
Slave:
Transmission time: 7.888200 ms, File size: 1502860 bytes  
```

---

# 3. Comparison file I/O and memory-mapped I/O

觀察實驗結果發現：  
  
a.  兩邊都用 mmap 的時候傳輸最快。  
>  可能使用 memory-mapped I/O 不需要使用大量的I/O指令，尤其在檔案大的時候可以直接透過對記憶體存取來處理file。  

b.  檔案大時，Master side 使用 mmap 明顯較許多。  
>  可能原因如上，memory mapped 可把檔案當成記憶體來用，直接使用指標來操作，達成高速的檔案存取，以 kernel 讀寫取代 I/O 讀寫。  

c.  Master side 和 Slave side 都使用 fcntl 時，Slave 的時間大增。  
  
實驗結果可能跟不同的電腦設備有關，假如電腦的記憶體太小的話使用 memory mmaped 存取檔案時可能會造成大量的 page fault，多出許多處理 page fault 的時間而使得速度的快慢有不同的結果。

---

# 4. work list  
r07922099 – 研究 ioctl()、mmap()、trace code …
