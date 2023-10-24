#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ "$h" = "gal-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use sbatch!
    exit 1
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

# import params
source ../params.sh

# bench
echo starting time is $(date)

for n in ${MERGESORT_NS};
do
    for t in ${THREADS};
    do
	./mergesort-task $n $t >/dev/null 2> ${RESULTDIR}/mergesort_task_${n}_${t}
    done
done
	     
echo ending time is $(date)
