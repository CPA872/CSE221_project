mkdir example_files
cd example_files
fallocate -l 0.125G 0_125GB.txt
fallocate -l 0.25G 0_25GB.txt
fallocate -l 0.5G 0_5GB.txt
fallocate -l 0.5G 0_5GBcopy.txt
fallocate -l 1G 1GB.txt
fallocate -l 2G 2GB.txt
fallocate -l 8G 8GB.txt
fallocate -l 1M 1M.txt
cd ..