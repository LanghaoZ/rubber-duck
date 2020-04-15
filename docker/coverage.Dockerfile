FROM rubber-duck:base as builder

COPY . /usr/src/project
WORKDIR /usr/src/project/build_coverage

cmake -DCMAKE_BUILD_TYPE=Coverage ..
make coverage


