echo "Compiles questions 1-4 for CPU operations..."
g++ ops1_MeasurementOverhead.cpp -o ops1 
echo "Compiled into ops1: part 1 (Measurement overhead)"

g++  ops2_ProcedureCallOverhead.c -o ops2
echo "Compiled into ops2: part 2 (Procedure call overhead)"

g++ -pthread ops345_SysCallTaskCreationContextS.cpp -o ops345 
echo "Compiled into ops345: part 3 (System call overhead), part 4 (Task creation time), and part 5 (Context switch time)"