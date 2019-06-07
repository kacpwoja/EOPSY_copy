#!/bin/sh
#Script for testing the copy program

echo "Testing the copy script"

echo "---------------------------"
echo "Very small file: (56B)"
t_start=`date +%N`
./copy 'test/file_vsmall' 'test/file_copy_rw'
t_end=`date +%N`
runtime_rw=`expr $t_end '-' $t_start`
t_start=`date +%N`
./copy '-m' 'test/file_vsmall' 'test/file_copy_mm'
t_end=`date +%N`
runtime_mm=`expr $t_end '-' $t_start`
echo "Runtime:"
echo "RW:" $runtime_rw "ns"
echo "MM:" $runtime_mm "ns"
echo "Hash sums:"
sha1sum 'test/file_vsmall'
sha1sum 'test/file_copy_rw'
sha1sum 'test/file_copy_mm'
rm 'test/file_copy_rw' 'test/file_copy_mm'

echo "---------------------------"
echo "Small file: (1.4kB)"
t_start=`date +%N`
./copy 'test/file_small' 'test/file_copy_rw'
t_end=`date +%N`
runtime_rw=`expr $t_end '-' $t_start`
t_start=`date +%N`
./copy '-m' 'test/file_small' 'test/file_copy_mm'
t_end=`date +%N`
runtime_mm=`expr $t_end '-' $t_start`
echo "Runtime:"
echo "RW:" $runtime_rw "ns"
echo "MM:" $runtime_mm "ns"
echo "Hash sums:"
sha1sum 'test/file_small'
sha1sum 'test/file_copy_rw'
sha1sum 'test/file_copy_mm'
rm 'test/file_copy_rw' 'test/file_copy_mm'

echo "---------------------------"
echo "Medium file: (11.2kB)"
t_start=`date +%N`
./copy 'test/file_medium' 'test/file_copy_rw'
t_end=`date +%N`
runtime_rw=`expr $t_end '-' $t_start`
t_start=`date +%N`
./copy '-m' 'test/file_medium' 'test/file_copy_mm'
t_end=`date +%N`
runtime_mm=`expr $t_end '-' $t_start`
echo "Runtime:"
echo "RW:" $runtime_rw "ns"
echo "MM:" $runtime_mm "ns"
echo "Hash sums:"
sha1sum 'test/file_medium'
sha1sum 'test/file_copy_rw'
sha1sum 'test/file_copy_mm'
rm 'test/file_copy_rw' 'test/file_copy_mm'

echo "---------------------------"
echo "Big file: (112kB)"
t_start=`date +%N`
./copy 'test/file_big' 'test/file_copy_rw'
t_end=`date +%N`
runtime_rw=`expr $t_end '-' $t_start`
t_start=`date +%N`
./copy '-m' 'test/file_big' 'test/file_copy_mm'
t_end=`date +%N`
runtime_mm=`expr $t_end '-' $t_start`
echo "Runtime:"
echo "RW:" $runtime_rw "ns"
echo "MM:" $runtime_mm "ns"
echo "Hash sums:"
sha1sum 'test/file_big'
sha1sum 'test/file_copy_rw'
sha1sum 'test/file_copy_mm'
rm 'test/file_copy_rw' 'test/file_copy_mm'

echo "---------------------------"
echo "Very big file: (2.9MB)"
t_start=`date +%N`
./copy 'test/file_vbig' 'test/file_copy_rw'
t_end=`date +%N`
runtime_rw=`expr $t_end '-' $t_start`
t_start=`date +%N`
./copy '-m' 'test/file_vbig' 'test/file_copy_mm'
t_end=`date +%N`
runtime_mm=`expr $t_end '-' $t_start`
echo "Runtime:"
echo "RW:" $runtime_rw "ns"
echo "MM:" $runtime_mm "ns"
echo "Hash sums:"
sha1sum 'test/file_vbig'
sha1sum 'test/file_copy_rw'
sha1sum 'test/file_copy_mm'
rm 'test/file_copy_rw' 'test/file_copy_mm'

echo "---------------------------"
echo "Giant file"
t_start=`date +%s.%N`
echo $t_start
./copy 'test/in.dat' 'test/file_copy_rw'
t_end=`date +%s.%N`
echo $t_end
runtime_rw=`echo "$t_end - $t_start" | bc`
t_start=`date +%s.%N`
./copy '-m' 'test/in.dat' 'test/file_copy_mm'
echo $t_start
t_end=`date +%s.%N`
echo $t_end
runtime_mm=`echo "$t_end - $t_start" | bc`
echo "Runtime:"
echo "RW:" $runtime_rw "s"
echo "MM:" $runtime_mm "s"
echo "Hash sums:"
sha1sum 'test/in.dat'
sha1sum 'test/file_copy_rw'
sha1sum 'test/file_copy_mm'
rm 'test/file_copy_rw' 'test/file_copy_mm'
