#!/bin/bash
# run the server in the background
# and redirect its stdout and stderr to /dev/null
# instead of inheriting them from the parent process

./bin/server ./integration_test/integration_test.conf &>/dev/null &

# check server's process id so that we can kill it later
pid=$!

# number of errors
errors=0

#number of tests
num_tsts=9

# rubberduck image test
curl "http://localhost:8080/static/rubber_duck.jpg" -o rubber_duck.jpg

diff ./integration_test/public/rubber_duck.jpg rubber_duck.jpg
if [ "$?" -eq "0" ]; then
  echo "rubber_duck.jpg -> ok"
else
  echo "rubber_duck.jpg -> fail"
  errors=$((errors + 1))
fi

# rubberduck zip test
curl "http://localhost:8080/static/rubber_duck.zip" -o rubber_duck.zip

diff ./integration_test/public/rubber_duck.zip rubber_duck.zip
if [ "$?" -eq "0" ]; then
  echo "rubber_duck.zip -> ok"
else
  echo "rubber_duck.zip -> fail"
  errors=$((errors + 1))
fi

# explicit index html test
curl "http://localhost:8080/static/index.html" -o explicit-index.html


diff ./integration_test/public/index.html explicit-index.html
if [ "$?" -eq "0" ]; then
  echo "explicit-index.html -> ok"
else
  echo "explicit-index.html -> fail"
  errors=$((errors + 1))
fi

# implicit index html test
curl "http://localhost:8080/static/" -o implicit-index.html
diff ./integration_test/public/index.html implicit-index.html
if [ "$?" -eq "0" ]; then
  echo "implicit-index.html -> ok"
else
  echo "implicit-index.html -> fail"
  errors=$((errors + 1))
fi

# hone html test
curl "http://localhost:8080/static/home.html" -o home.html

diff ./integration_test/public/home.html home.html
if [ "$?" -eq "0" ]; then
  echo "home.html -> ok"
else
  echo "home.html -> fail"
  errors=$((errors + 1))
fi

# text txt test
curl "http://localhost:8080/static/text.txt" -o text.txt

diff ./integration_test/public/text.txt text.txt
if [ "$?" -eq "0" ]; then
  echo "text.txt -> ok"
else
  echo "text.txt -> fail"
  errors=$((errors + 1))
fi

# echo test
curl "http://localhost:8080/echo/" -o echo.out
expected=$'GET /echo/ HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\n'
echo -n "$expected" > expected.out

diff expected.out echo.out
if [ "$?" -eq "0" ]; then
  echo "echo.out -> ok"
else
  echo "echo.out -> fail"
  errors=$((errors + 1))
fi

# check the response for bad request
result=$(echo -n 'GET / HHTP/1.1\r\nHost: www.rubber-duck.com\r\nConnection: close\r\n\r\n' | nc localhost 8080)
echo -n "$result" > bad_request.out
expected=$'HTTP/1.1 400 Bad Request\r\nContent-Length: 89\r\nContent-Type: text/html\r\n\r\n<html><head><title>Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>'
echo -n "$expected" > expected.out
diff expected.out bad_request.out
if [ "$?" -eq "0" ]; then
  echo "bad_request.out -> ok"
else
  echo "bad_request.out -> fail"
  errors=$((errors + 1))
fi


# check the response for not found
curl "http://localhost:8080/" -o not_found.out
expected=$'<html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>'
echo -n "$expected" > expected.out
diff expected.out not_found.out
if [ "$?" -eq "0" ]; then
  echo "not_found.out -> ok"
else
  echo "not_found.out -> fail"
  errors=$((errors + 1))
fi

kill -2 $pid
rm rubber_duck.jpg rubber_duck.zip explicit-index.html implicit-index.html home.html text.txt echo.out bad_request.out expected.out not_found.out

# report the test results
# check if any error occur
if [ "$errors" -eq "0" ]; then
  echo "Passed all integration tests"
  exit 0
else
  echo "Failed $errors test case(s) out of total $num_tsts tests."
  exit 1
fi
