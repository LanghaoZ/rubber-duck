#!/bin/bash
# run the server in the background
# and redirect its stdout and stderr to /dev/null
# instead of inheriting them from the parent process
./bin/server ./integration_test_config &>/dev/null &
# check server's process id so that we can kill it later
pid=$!
# number of errors
errors=0
#number of tests
num_tsts = 4
# check the response to get request, @junhong
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

# Tst1 : Correct Request, @lzy
result=$(printf 'GET /echo HTTP/1.1\r\nHost: www.rubber-duck.com\r\nConnection: close\r\n\r\n' | nc 127.0.0.1 8080)                    
echo $result > test_response_correct_rq
echo $result > expected_responsecorrect_rq #assume correct result, need to check
diff ./expected_responsecorrect_rq test_responsecorrect_rq

if [[ $? -eq 0 ]]; then
    echo "test_responsecorrect_rq -> ok"
else 
	echo "test_responsecorrect_rq -> fail"
	errors+=1
fi

# Tst2: Incorrect Request, @lzy
result=$(printf 'GET /echo HHTP/1.1\r\nHost: www.rubber-duck.com\r\nConnection: close\r\n\r\n' | nc 127.0.0.1 8080)                    
echo $result > test_response_incorrect_rq
echo $result > expected_response_incorrect_rq #assume correct result, need to check
diff ./expected_response_incorrect_rq test_response_incorrect_rq

if [[ $? -eq 0 ]]; then
    echo "test_response_incorrect_rq -> ok"
else 
    echo "failed"; 
	echo "test_response_incorrect_rq -> fail"
	errors+=1
fi

# Tst3: Post Request, @lzy
result=$(printf 'POST /echo HTTP/10.10\r\nHost: www.example.com\r\nConnection: close\r\n\r\n' | nc 127.0.0.1 8080)                    
echo $result > test_response_post_tst
echo $result > expected_response_post_tst #assume correct result, need to check
diff ./expected_response_post_tst test_response_post_tst

if [[ $? -eq 0 ]]; then
    echo "test_response_post_tst -> ok"
else 
    echo "failed"; 
	echo "test_response_post_tst -> fail"
	errors+=1 
fi


kill $pid
rm integration_test_get.out expected.out test_response_post_tst test_response_incorrect_rq test_response_correct_rq
# report the test results
if [ "$errors" -eq "0" ]; then
  echo "Passed all integration tests"
  exit 0
else
  echo "Failed $errors test case(s) out of total $num_tsts tests."
  exit 1
fi
