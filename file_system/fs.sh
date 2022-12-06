echo "Compiles questions 1-4 for file system"
g++ fs1_FileCache.cpp -o fs1_FileCache
echo "Compiled into fs1_FileCache: part 1 (File Cache) "

g++ fs2_FileRead.cpp -o fs2_FileRead
echo "Compiled into fs2_FileRead: part 2 (Sequential and Random File Read)"

g++ fs3_RemoteFileRead.cpp -o fs3_RemoteFileRead
echo "Compiled into fs3_RemoteFileRead: part 3 (Remote Sequential and Random File Read)"

g++ fs4_Contention.cpp -o fs4_Contention
echo "Compiled into fs4_Contention: part 4 (Contention)"