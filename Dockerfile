FROM deltavoid/debian_cpp_basic:2022.01.13


ADD ./ /repos/CppProgramming

# RUN cd /repos/CppProgramming && git submodule update --init --recursive

RUN cd /repos/CppProgramming && ./scripts/install_dependencies.sh

RUN cd /repos/CppProgramming && ./scripts/build.sh
