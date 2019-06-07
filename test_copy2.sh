#!/bin/sh
#Script for testing the copy program

echo "Testing the copy script"
echo "---------------------------"
echo "Giant file"
time './copy' 'test/in.dat' 'test/file_copy_rw'
time './copy' '-m' 'test/in.dat' 'test/file_copy_mm'
runtime_mm=`expr $t_end '-' $t_start`
echo "Runtime:"
echo "RW:" $runtime_rw "ns"
echo "MM:" $runtime_mm "ns"
echo "Hash sums:"
sha1sum 'test/in.dat'
sha1sum 'test/file_copy_rw'
sha1sum 'test/file_copy_mm'
rm 'test/file_copy_rw' 'test/file_copy_mm'
