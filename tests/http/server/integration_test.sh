#!/bin/bash

# run the server in the background
# and redirect its stdout and stderr to /dev/null
# instead of inheriting them from the parent process
./bin/server ./integration_test_config &>/dev/null &

# check server's process id so that we can kill it later
pid=$!

# number of errors
errors=0

# check the response to get request
curl "http://localhost:8080/" -o integration_test_get.out
expected=$'GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\n'
echo -n "$expected" > expected.out
diff expected.out integration_test_get.out

if [ "$?" -eq "0" ]; then
  echo "integration_test_get.out -> ok"
else
  echo "integration_test_get.out -> fail"
  errors+=1
fi

# clean up
kill $pid
rm integration_test_get.out expected.out

# report the test results
if [ "$errors" -eq "0" ]; then
  echo "Passed all integration tests"
  exit 0
else
  echo "Failed $errors test case(s)"
  exit 1
fi