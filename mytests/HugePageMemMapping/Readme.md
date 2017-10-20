### Set the hugepage number in the system

    echo 16 > /proc/sys/vm/nr_hugepages

### Mount the hugetlbfs by cmd: 

    mount -t hugetlbfs none /mnt/huge -o size=16M

    [root@localhost HugePageMemMapping]# mount |grep huge
    cgroup on /sys/fs/cgroup/hugetlb type cgroup (rw,nosuid,nodev,noexec,relatime,hugetlb)
    hugetlbfs on /dev/hugepages type hugetlbfs (rw,relatime)
    none on /mnt/huge type hugetlbfs (rw,relatime,size=16M)
    [root@localhost HugePageMemMapping]# 


### Then execute cmd:
    
    ./HugePageMemMapping /mnt/huge/test.txt

### From the /proc/meminfo, we can see the available Hugepages change

    HardwareCorrupted:     0 kB
    AnonHugePages:     14336 kB
    HugePages_Total:      16
    HugePages_Free:       14
    HugePages_Rsvd:        0
    HugePages_Surp:        0
    Hugepagesize:       2048 kB
    DirectMap4k:       88000 kB
    DirectMap2M:     4106240 kB


