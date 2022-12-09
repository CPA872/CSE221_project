echo "Compiles questions 1-3 for Memory..."
g++ mem1_RAMAccessTime.cpp -o mem1_RAMAccessTime
echo "Compiled into mem1_RAMAccessTime: part 1 (RAM access time)"

g++ mem2_RAMBandwidth.cpp -o mem2_RAMBandwidth
echo "Compiled into mem2_RAMBandwidth: part 2 (RAM bandwidth - first experiment)"

g++ mem2_RAMBandwidthP2.cpp -o mem2_RAMBandwidthP2 -mavx2
echo "Compiled into memory_bw with AVX2: part 2 (RAM bandwidth - second experiment)."
echo "To compile with SSE2 run the command: g++ mem2_RAMBandwidthP2.cpp -o mem2_RAMBandwidthP2 -msse2"

g++ mem3_PageFaultTime.cpp -o mem3_PageFaultTime
echo "Compiled into mem3_PageFaultTime: part 3 (Page Fault time)"