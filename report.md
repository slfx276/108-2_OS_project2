## 1. Design

a. two user programs:
one for accessing the slave device (slave side) and the other for accessing the master device (master side),

> slave-side program :  
    it first uses ioctl to set the IP address of master, then receives the data from slave device and writes all of them to the output file by specified method.

> master-side program :  
read the input file by the specified method and send the data to the master device.

b. two kernel programs:
> one for receiving the data from master device, and transmit to user program (slave side),  
one for receive the data from user program, and send to master device (master side).

Once the transmission has completed, the programs will show the total transmission time, from device is opened to the device is closed. And display the size of the transferred file in both computers. All the parameters are passed to programs through standard input.



## 2. Results

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
Transmission time: 22.828100 ms, File size: 1502860 bytes
Slave:
segmentation fault QQ

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
Transmission time: 4.150200 ms, File size: 1502860 bytes
Slave:
Transmission time: 1985.619700 ms, File size: 1502860 bytes
```


## 3. Comparison



## 4. work list  
r07922099 – 研究 ioctl()、mmap()、trace code …
