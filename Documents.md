
# Manual document of OS project2

## 1.  mmap() function:
Linux提供了記憶體映射函數 mmap，它把文件內容映射到一段記憶體上(準確說是虛擬記憶體上)，
通過對這段記憶體的讀取和修改，實現對文件的讀取和修改。 

使用記憶體對映有許多好處：
高速檔案存取。可把檔案當成記憶體來用，直接使用指標來操作. 
對映的記憶體可當成 processes 間共享記憶體，該記憶體內容存在檔案中，因此與 processes 無關. 通常在需要對文件進行頻繁讀寫時使用，這樣用核心讀寫取代I/O讀寫，以獲得較高的性能.  

函數:
<pre><code>void *mmap(void *start , size_t length , int prot
                        , int flags, int fd, off_t offsize);</code></pre>  

* (Argument) start：  
指向欲 mapping 的 memory address，通常設為NULL，代表讓系統自動選定位址，核心會自己在進程位址空間中選擇合適的位址建立映射。映射成功後返回該位址。如果不是NULL，則給核心一個提示，應該從什麼位址開始映射，核心會選擇start之上的某個合適的位址開始映射。
建立 mapping 後，**真正的映射位址通過返回值可以得到**。 
<br>
* (Argument) length：
    將文件的多大長度映射到記憶體<br>
    <br>
* (Argument) prot: 
mapping 區域的保護方法，例如：r/w only 等等。 
    > PROT_EXEC 映射區域可被執行
    > PROT_READ 映射區域可被讀取
    > PROT_WRITE 映射區域可被寫入
    > PROT_NONE 映射區域不能存取 ...
<br>

* (Argument) flags: 
    影響映射區域的各種特性。在調用mmap()時必須要指定MAP_SHARED 或MAP_PRIVATE。
    > MAP_FIXED 如果參數start所指的位址無法成功建立映射時，則放棄映射，不對位址做修正。通常不鼓勵用此旗標。
MAP_SHARED 允許其他映射該文件的行程共享，對映射區域的寫入數據會複製回文件。
MAP_PRIVATE 不允許其他映射該文件的行程共享，對映射區域的寫入操作會產生一個映射的複製(copy-on-write)，對此區域所做的修改不會寫回原文件。
<br>
* (Argument) fd:
由 open() return 的文件描述符，代表要 mapping 到 memory 中的文件。
<br>
* (Argument) offset：
從文件映射開始處的偏移量，通常為0，代表從文件最前方開始映射。
offset必須是分頁大小的整數倍(在32位體系統結構上通常是4K)。
<br>
* return value:
若映射成功則返回映射區的核心起始位址，否則返回MAP_FAILED(-1)，錯誤原因存於 errno .


###  mmap simple example
<pre><code>
#include &ltsys/mman.h> 
#include &ltsys/types.h> 
#include &ltfcntl.h> 
#include &ltunistd.h> 

int main(int argc, char *argv[])
{
    unsigned char *ADDR; 
    int device_pointer;
    device_pointer = open("/dev/mem",O_RDWR);

    if (device_pointer < 0) // if open file failed
    {
    printf("device file open error !\n");
    return ;
    }

    // we map the file in memory area, whose memory address is given to ADDR.
    ADDR = mmap(0,0x1000,PROT_READ|PROT_WRITE,MAP_SHARED,device_pointer,0x37F80000);

    // do operations with the memory mapping address.
    *(volatile unsigned int *)(ADDR + 0x00) = 0x1;//操作實際位址0x37F80000，令其值為1
    *(volatile unsigned int *)(ADDR + 0x04) = 0x1;//操作實際位址0x37F80004，令其值為1

    munmap(ADDR,0x1000);
    close(device_pointer);
    return 0;
}</code></pre>

---
## 2. ioctl() function
<pre><code>
#include &lt;sys/ioctl.h>
int ioctl(int fd , int cmd , ... );
</code></pre>

<table>
    <tr>
        <td>fd</td>
        <td>file description (檔案描述符)</td>
    </tr>
    <tr>
        <td>cmd</td>
        <td>互動協議，裝置驅動將根據cmd執行對應操作</td>
    </tr>

</table>

* return value:
ioctl() 執行成功時返回 0，失敗則返回 -1，並設定全域性變數 errno 值，如下：

>EBADF ： fd is not a valid descriptor.   
EFAULT ： arg references an inaccessible memory area.   
EINVAL ： Request or arg is not valid. 
ENOTTY ： fd is not associated with a character special device. 
ENOTTY ： The specified request does not apply to the kind of object that the descriptor fd references.

因此，在使用者空間使用 ioctl 時，可以做如下的出錯判斷以及處理：
<pre><code>
int ret;
ret = ioctl(file_descryption, MY_CMD);

if (ret == -1) //  check ioctl() result.
{
    printf("ioctl: %s\n", strerror(errno));
}</code></pre>

#### function example can take [linux內核ioctl函數學習](https://blog.xuite.net/yang44/dd/49077684-linux%E5%85%A7%E6%A0%B8ioctl%E5%87%BD%E6%95%B8%E5%AD%B8%E7%BF%92) as reference

### 調節音量 simple example：
<pre><code>
#include &lt;sys/types.h>
#include &lt;sys/stat.h>
#include &lt;fcntl.h>
#include &lt;sys/ioctl.h>
#include &lt;sys/soundcard.h>
#include &lt;stdio.h>
#include &lt;unistd.h>
#include &lt;math.h>
#include &lt;string.h>
#include &lt;stdlib.h>
#define  BASE_VALUE 257

int main(int argc,char *argv[])
{
    int mixer_fd=0;
    char *names[SOUND_MIXER_NRDEVICES]=SOUND_DEVICE_LABELS;
    int value,i;
    printf("/nusage:%s dev_no.[0..24] value[0..100]/n/n",argv[0]);
    printf("eg. %s 0 100/n",argv[0]);
    printf("    will change the volume to MAX volume./n/n");
    printf("The dev_no. are as below:/n");
    for (i = 0 ; i &lt; SOUND_MIXER_NRDEVICES ; i++)
    {
            if (i%3==0) printf("/n");
            printf("%s:%d/t/t",names[i],i);
    }
    printf("/n/n");
    if (argc < 3)
        exit(1);

    if ((mixer_fd = open("/dev/mixer",O_RDWR)))
    {
        printf("Mixer opened successfully,working.../n");

        value=BASE_VALUE*atoi(argv[2]);

        if (ioctl(mixer_fd,MIXER_WRITE(atoi(argv[1])),&value)==0)
            printf("successfully.....");
        else    
            printf("unsuccessfully.....");
        
        printf("done./n");
    }
    else
        printf("can't open /dev/mixer error..../n");

    exit(0);
}
</code></pre>