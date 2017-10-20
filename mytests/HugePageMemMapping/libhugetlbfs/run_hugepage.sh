#/bin/sh

export LD_PRELOAD=/usr/lib64/libhugetlbfs.so
export HUGETLB_MORECORE=yes

./malloc
