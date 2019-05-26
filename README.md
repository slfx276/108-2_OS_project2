This is the source code of operating system project2. [OS-project2](http://rswiki.csie.org/dokuwiki/courses:107_2:project_2)  
OS: Lunix 18.04 with **kernel version-4.14.25**

`./master_device `: the device moudule for master server  
`./slave_device ` : the device moudule for slave client  
`./ksocket  `: the device moudule including the funtions used for kernel socket  
`./data  ` : input/output data  
`./user_program `: the user program "master" and "slave"  

---

To use it, please:   
```
1.  change to super user   
2.  execute "$ sudo ./compile.sh" to compile codes and install modules  
3.  follow the input instrutions in the spec,  

i.e.  
$ sudo ./master ../data/file1_in mmap  
$ sudo ./slave ../data/file1_out fcntl 127.0.0.1  
```

Make sure that you are under the path "./user_program" when you execute user programs.  
Though the execution order of user program "master" and "slave" does not matter,  
it is suggested to execute "master" first to get more precise transmission time.  

## Project Notes:

### 1.  two user programs:
one for accessing the slave device (slave side) and the other for accessing the master device (master side),  
  
**slave-side program  :**
> it first uses ioctl to set the IP address of master, then receives the data from slave device and writes all of them to the output file by specified method.
  
**master-side program  :**
> read the input file by the specified method and send the data to the master device.

### 2.  two kernel programs:  
  
one for receiving the data from master device, and transmit to user program (slave side),   
one for receive the data from user program, and send to master device (master side).  
  
Once the transmission has completed, the programs will show the total transmission time, from device is opened to the device is closed. And display the size of the transferred file in both computers. All the parameters are passed to programs through standard input.

