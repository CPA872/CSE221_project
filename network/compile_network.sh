echo "Compiling all files for network operation measurements"

g++ network1_rtt_client.cpp -o network1_rtt_client
g++ network1_rtt_server.cpp -o network1_rtt_server
g++ network2_peak_bw_client.cpp -o network2_peak_bw_client
g++ network2_peak_bw_server.cpp -o network2_peak_bw_server
g++ network3_setup_overhead.cpp -o network3_setup_overhead
g++ network3_teardown_overhead.cpp -o network3_teardown_overhead

echo "Done!"