passed=0
tests=100
for i in $(seq 1 $tests)
do
  echo "Test($i) ................................................. :-" >> final_result.out

  test_result=$(./caltrain)
  echo "$test_result" >> final_result.out

  if [[ "$test_result" == *"Looks good!"* ]]; then
    ((passed++))
  fi

done
echo "Total tests passed: $passed"
echo "Total tests failed: $((tests - passed))"