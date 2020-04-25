### Build/test container ###
# Define builder stage
FROM rubber-duck:base as builder

# Share work directory
COPY . /usr/src/project
WORKDIR /usr/src/project/build

# Build and test
RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
# Define deploy stage
FROM ubuntu:bionic as deploy

COPY --from=builder /usr/src/project/build/bin/server .
COPY --from=builder /usr/src/project/conftst/rubber-duck.conf .
COPY --from=builder /usr/src/project/build/public ./public

EXPOSE 80

ENTRYPOINT ["./server"]

CMD ["rubber-duck.conf"]
