echo Running application ..
make test_suite
cd tests/build
./suite
result=$?
echo Finished
exit $result
