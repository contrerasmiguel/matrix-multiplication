rm -rf temp_cmake-files\
&& mkdir temp_cmake-files\
&& cd temp_cmake-files\
&& ./../cmake-3.5.2-Linux-x86_64/bin/cmake ../ParallelMatrixMultiplication\
&& make && cd ..\
&& rm -rf temp_cmake-files\
&& cp -r --remove-destination ParallelMatrixMultiplication/bin/* .
